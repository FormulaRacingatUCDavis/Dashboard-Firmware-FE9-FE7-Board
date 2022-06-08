
#include <project.h>
#include <stdio.h>
#include "frucd_display.h"
#include "data.h"
#include "LED.h"
#include "can_manga.h"
#include "fonts.h"

// declared external in can_manga.c
volatile uint8_t PACK_TEMP = 0;
volatile uint8_t BSPD_CATCH = 0;
volatile uint16_t CURRENT = 0;
volatile uint8_t shutdown_flags = 0;
volatile int ERROR_NODE;
volatile int ERROR_IDX;
volatile uint32_t voltage = 0;
uint8_t charge = 0;
uint16_t curr_voltage = 0;

int firstStart = 0;
int firstLV = 0;
int firstHV = 0;
int firstDrive = 0;

#define PWM_PULSE_WIDTH_STEP        (10u)
#define SWITCH_PRESSED              (0u)
#define PWM_MESSAGE_ID              (0x1AAu)
#define PWM_MESSAGE_IDE             (0u)    /* Standard message */
#define PWM_MESSAGE_IRQ             (0u)    /* No transmit IRQ */
#define PWM_MESSAGE_RTR             (0u)    /* No RTR */
#define CAN_RX_MAILBOX_0_SHIFT      (1u)
#define CAN_RX_MAILBOX_1_SHIFT      (2u)
#define DATA_SIZE                   (6u)
#define ONE_BYTE_OFFSET             (8u)

#define PEDAL_TIMEOUT 100 // Timeout after (PEDAL_TIMEOUT * 10)ms

/* Function prototypes */
//CY_ISR_PROTO(ISR_CAN);

/* Global variables used to store configuration and data for BASIC CAN mailbox */
//CAN_DATA_BYTES_MSG dataPWM;
//CAN_TX_MSG messagePWM;

/* Global variable used to store PWM pulse width value */
//uint8 pulseWidthValue = 0u;

/* Global variable used to store receive message mailbox number */
//volatile uint8 receiveMailboxNumber = 0xFFu;

void nodeCheckStart()
{
    Node_Timer_Start();
    isr_nodeok_Start();
}

void displayData() {
    if(BSPD_CATCH == 1){
        //libTFT_DrawString("BSPD TRIGGERED",0,0,8,RED, Font16x16);
        UG_PutString(100, 5 , "test");
    }
}

CY_ISR(ISR_WDT){
    WDT_Timer_STATUS;
    WDT_Reset_Write(0);
    CyDelay(100);
    WDT_Reset_Write(1);
}

/* Switch debounce delay in milliseconds */
#define SWITCH_DEBOUNCE_UNIT   (1u)
/* Number of debounce units to count delay, before consider that switch is pressed */
#define SWITCH_DEBOUNCE_PERIOD (10u)
/* Global variable used to store switch state */
volatile uint8 switches = 0;

// vehicle state from VCU
volatile vcu_state state = LV;
volatile vcu_fault fault = NONE;

// Global variables used to track status of nodes
volatile uint32_t pedalOK = 0; // for pedal node

volatile int previous_state = -1; // used for SOC writing

// info from BMS
volatile uint8 soc = 0;
volatile BMS_STATUS bms_status = NO_ERROR;

// info from MC and motor
volatile uint16 mc_temp = 0;
volatile uint16 motor_temp = 0;

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  main() performs the following functions:
*  1: Initializes a structure for the Basic CAN mailbox to send messages.
*  2: Starts the CAN and LCD components.
*  3: When received Message 1, sends the PWM pulse width and displays
*     received switch status and value of PWM pulse width on an LCD; 
*     When received Message 2, display received ADC data on an LCD.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

int main()
{   
    EEPROM_1_Start();
    
    /*UG_COLOR color[3];
    color[0] = C_RED;
    color[1] = C_GREEN;
    color[2] = C_BLUE;*/
    
    D_BL_Write(1);
    GraphicLCDIntf_Start();
    Display_Init();
    
    UG_FontSelect(&FONT_12X16);
    UG_SetBackcolor(C_BLACK);
    UG_SetForecolor(C_WHITE);
    
    calibrateScreen();
    
    //Tach Meter Stuff
    //uint8_t value=0; // replace the value with 
    //int8_t direction=1;
    
    //precharging time counter
    //volatile uint32_t PrechargingTimeCount = 0;
    //uint32_t DriveTimeCount = 0;

    CyGlobalIntEnable;
    
    nodeCheckStart();
    
    // WatchDog Timer
    WDT_Timer_Start();
    isr_wdt_StartEx(ISR_WDT);
    
    int bms_error;
    int faulted = 0;
    
    //Initialize CAN
    CAN_GlobalIntEnable();
    //CAN_Init();
    CAN_Start();
    LED_color_wheel(200);
    
	ADC_GLV_V_Start();
    
    // initialize display layout
    initDashTemplate();
    disp_state(FAULT); // effectively STARTUP
    
    for(;;)
    {
        
        LED_Write(1);
        
        // Check if all nodes are OK
        if (pedalOK > PEDAL_TIMEOUT)
        {
            can_send_cmd(0, 0, 0, 0); // setInterlock. 
            state = FAULT;
            //fault = nodeFailure;
        }
        
        if(bms_status != NO_ERROR && state != LV) {
            state = FAULT;
            //fault = fromBMS;
            if(!faulted)
                bms_error = bms_status;
        }
        
        // check driver switches
        if (HV_Read()) {
            switches |= 0b10;
        } else {
            switches &= 0b11111101;
        }
        if (Drive_Read()) {
            switches |= 0b1;
        } else {
            switches &= 0b11111110;
        }
        // send driver switches
        can_send_switches(switches);
        
        // indicator LEDs (active low)
        BMS_LED_Write(!bms_status);
        IMD_LED_Write(shutdown_flags & 0b00100000); // bitmask for IMD_OK
        
        /*      START display latest data       */
        disp_SOC(soc);
        disp_max_pack_temp(PACK_TEMP);
        
        disp_state(state);
        uint32_t glv_v = (int32_t)ADC_GLV_V_CountsTo_mVolts(ADC_GLV_V_Read16());
        disp_glv_v(glv_v);
        
        disp_mc_temp(mc_temp);
        disp_motor_temp(motor_temp);
        /*      END display latest data         */
        
        if (state == DRIVE && previous_state == HV_ENABLED) {
            // entered drive; sound ready to drive buzzer
            Buzzer_Write(1);
            // EV.10.5.2: Sounded continuously for minimum 1 second
            // and a maximum of 3 seconds [we use 2 seconds]
            ReadyToDrive_Int_Start();
            ReadyToDrive_Timer_Init();
            ReadyToDrive_Timer_Enable();
        }
        previous_state = state;
        if (state != DRIVE) {
            // exited drive
            Buzzer_Write(0);
        }
        
        switch(state)
        {    

            // startup -- 
            case LV:

                
                //state = LV;
                
            break;
#if 0    
            case LV:
                if(firstLV == 0) {
                    UG_PutString(0, 0, "LV");
                    firstLV = 1;
                } else {
                    charge = SOC_LUT[(voltage - 93400) / 100] / 100;
                    displayData();
                }
                
                CAN_GlobalIntEnable();
                CAN_Init();
                CAN_Start();
                
                can_send_cmd(0, 0, 0, 0); // clear e_stop_check if need be
                //nodeCheckStart();
          
                can_send_status(state, error_state);

                Buzzer_Write(0);
                
                
            break;
                
            case Precharging:
                UG_PutString(0, 0, "PRECHARGE");
                
                CAN_GlobalIntEnable();
                CAN_Init();
                CAN_Start();
                //nodeCheckStart();
                
                can_send_status(state, error_state);
                
                LED_color(C_MAGENTA);
                Buzzer_Write(0);
                
            break;
	        
            case HV_Enabled:
                if(firstHV == 0) {
                    UG_PutString(0, 0, "HV");
                    firstHV = 1;
                } else {
                    displayData();
                }

                CAN_GlobalIntEnable();
                CAN_Init();
                CAN_Start();
                
                //nodeCheckStart();
                
                can_send_status(state, error_state);
                
                //
                // RGB code goes here
                // Blue
                LED_color(C_BLUE);
                
                /*
                RGB3_2_Write(1);
                RGB2_2_Write(1);
                RGB1_2_Write(1);
                */
                //CyDelay(5000); ///for debug
                
                Buzzer_Write(0);
                
                //charge = SOC_LUT[(voltage - 93400) / 100] / 100;
                //hex1Display(charge);
                
                if (Drive_Read())
                {
                    CyDelay(1000); // wait for the brake msg to be sent
                    if(getErrorTolerance() == 1) // 100 for error tolerance /// needs to be getErrorTolerance
                    {
                        Buzzer_Write(1);
                        CyDelay(1000);
                        Buzzer_Write(0);
                        state = Drive;
                        break;
                    }
                    else
                    {
                        state = Fault;
                        error_state = fromHV_Enabled;
                        break;
                    }                     
                }
                
                // if capacitor voltage is undervoltage, change the threshold 0x16
                if(!HV_Read() | (getCapacitorVoltage() < 0x16))
                {
                    state = LV;
                    DriveTimeCount = 0;
                    break;
                }
                
            break;
                
	        case Drive:
                if(firstDrive == 0) {
                    UG_PutString(0, 0, "DRIVE");
                    firstDrive = 1;
                } else {
                    // calcualte SOC
                    if(CURRENT < 2500) {
                        charge = SOC_LUT[(voltage - 93400) / 100] / 100;
                    } 
                    /*if(getEStop() ==1){ //Tehya check
                        GLCD_Clear_Frame();
                        GLCD_DrawString(0,0,"ESTOP",8);
                        GLCD_Write_Frame();
                    }*/
                    displayData();
                }
                
                if(getEStop() == 1){ //Tehya check
                    while(Drive_Read() || HV_Read()){ // stuck here until both switches are turned off
                    }
                    state = LV;
                    can_send_cmd(0, 0, 0, 1); 
                    break;
                }
                
                can_send_charge(charge, 0);
                
                can_send_status(state, error_state);
                //
                // RGB code goes here
                // Green
                LED_color(C_GREEN);
                   
                uint8_t ACK = 0xFF;
                
                DriveTimeCount++;
                if (DriveTimeCount > 100) //EDIT: was 100!
                {
                    DriveTimeCount = 0; 
                    ACK = getAckRx();
                }
   
                uint8_t ABS_Motor_RPM = getABSMotorRPM();
                
                // check if everything is going well
                // probably kyle just turing the car off wrong
                if (!HV_Read()) {
                    can_send_cmd(0, 0, 0, 0);   
                    state = LV;
                }
                // exiting drive mode, likely that car is about to shut down
                if (!Drive_Read()) {
                    state = HV_Enabled;
                    can_send_cmd(1, 0, 0, 0);
                }
                if ((ACK != 0xFF) | 
                    (!getCurtisHeartBeatCheck())) // TODO: Heart beat message is never cleared
                {
                    can_send_cmd(0, 0, 0, 0);
                    state = Fault;
                    error_state = fromDrive;
                    DriveTimeCount = 0;
                    break;
                }
                
#endif
                
            default:
                RGB3_1_Write(1);
                RGB2_1_Write(1);
                RGB1_1_Write(1);
           
        }// end of switch
        

        
    } 
}

/*******************************************************************************
* Function Name: ISR_CAN
********************************************************************************
*
* Summary:
*  This ISR is executed at a Receive Message event and set receiveMailboxNumber
*  global variable with receive message mailbox number.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(ISR_CAN)
{   
    /* Clear Receive Message flag */
    CAN_INT_SR_REG.byte[1u] = CAN_RX_MESSAGE_MASK;

    /* Set the isrFlag */
    //isrFlag = 1u;    

    /* Acknowledges receipt of new message */
    CAN_RX_ACK_MESSAGE(CAN_RX_MAILBOX_0);
}



/* [] END OF FILE */
