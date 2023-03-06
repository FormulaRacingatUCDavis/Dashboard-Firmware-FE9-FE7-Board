
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


void displayData() {
    if(BSPD_CATCH == 1){
        //libTFT_DrawString("BSPD TRIGGERED",0,0,8,RED, Font16x16);
        UG_PutString(100, 5 , "test");
    }
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

uint8_t logo[] = {255, 255, 175, 66, 255, 241, 86, 255, 131, 66, 255, 241, 86, 255, 131, 66, 255, 203, 0, 164, 86, 255, 131, 66, 236, 73, 211, 1, 164, 67, 255, 150, 66, 232, 79, 209, 0, 165, 67, 255, 150, 66, 230, 83, 206, 1, 165, 67, 255, 150, 66, 229, 85, 204, 1, 166, 67, 255, 150, 66, 227, 70, 139, 70, 201, 2, 166, 67, 255, 150, 66, 226, 69, 143, 69, 199, 2, 167, 67, 255, 150, 66, 225, 68, 146, 68, 198, 3, 167, 67, 255, 150, 66, 225, 67, 149, 67, 197, 2, 168, 67, 255, 150, 66, 224, 67, 150, 68, 195, 2, 169, 67, 255, 150, 66, 224, 67, 151, 67, 194, 3, 169, 67, 159, 73, 149, 69, 140, 71, 135, 72, 139, 67, 141, 67, 136, 66, 141, 73, 199, 67, 153, 67, 192, 3, 170, 67, 157, 77, 139, 67, 130, 70, 132, 66, 130, 74, 132, 75, 138, 67, 141, 67, 136, 66, 138, 77, 198, 66, 154, 67, 191, 4, 170, 67, 155, 80, 139, 66, 128, 72, 132, 66, 129, 76, 130, 77, 137, 67, 141, 67, 136, 66, 136, 80, 196, 67, 155, 66, 190, 4, 171, 84, 137, 69, 135, 68, 138, 72, 136, 71, 133, 73, 134, 68, 136, 67, 141, 67, 136, 66, 136, 69, 134, 68, 195, 66, 156, 67, 188, 5, 171, 85, 135, 68, 138, 68, 137, 69, 139, 69, 136, 71, 136, 67, 136, 67, 141, 67, 136, 66, 137, 65, 138, 67, 195, 66, 157, 66, 188, 4, 172, 85, 134, 68, 141, 67, 136, 68, 140, 68, 138, 69, 138, 66, 136, 67, 141, 67, 136, 66, 151, 67, 194, 66, 157, 66, 187, 5, 172, 85, 134, 67, 142, 67, 136, 67, 141, 68, 138, 68, 139, 67, 135, 67, 141, 67, 136, 66, 151, 67, 193, 67, 157, 66, 186, 5, 173, 84, 134, 67, 144, 67, 135, 67, 141, 67, 139, 68, 139, 67, 135, 67, 141, 67, 136, 66, 152, 66, 193, 67, 157, 66, 185, 6, 173, 67, 151, 67, 144, 67, 135, 67, 141, 67, 139, 67, 140, 67, 135, 67, 141, 67, 136, 66, 152, 66, 193, 66, 158, 66, 184, 6, 174, 67, 151, 66, 146, 66, 135, 66, 142, 66, 141, 66, 140, 67, 135, 67, 141, 67, 136, 66, 152, 66, 193, 66, 158, 66, 183, 7, 174, 67, 150, 67, 146, 67, 134, 66, 142, 66, 141, 66, 141, 66, 135, 67, 141, 67, 136, 66, 143, 75, 193, 66, 158, 66, 182, 7, 175, 67, 150, 67, 146, 67, 134, 66, 142, 66, 141, 66, 141, 66, 135, 67, 141, 67, 136, 66, 139, 79, 193, 66, 158, 66, 181, 8, 175, 67, 150, 67, 146, 67, 134, 66, 142, 66, 141, 66, 141, 66, 135, 67, 141, 67, 136, 66, 137, 81, 193, 66, 158, 66, 181, 7, 176, 67, 150, 67, 146, 67, 134, 66, 142, 66, 141, 66, 141, 66, 135, 67, 141, 67, 136, 66, 136, 70, 136, 66, 193, 66, 158, 66, 180, 8, 176, 67, 150, 67, 146, 67, 134, 66, 142, 66, 141, 66, 141, 66, 135, 67, 141, 67, 136, 66, 135, 68, 139, 66, 193, 66, 158, 66, 179, 8, 177, 67, 151, 66, 146, 66, 135, 66, 142, 66, 141, 66, 141, 66, 135, 67, 141, 67, 136, 66, 135, 67, 140, 66, 193, 100, 178, 9, 177, 67, 151, 67, 144, 67, 135, 66, 142, 66, 141, 66, 141, 66, 135, 67, 141, 67, 136, 66, 135, 66, 140, 67, 193, 100, 177, 9, 178, 67, 151, 67, 144, 67, 135, 66, 142, 66, 141, 66, 141, 66, 135, 67, 140, 68, 136, 66, 134, 67, 140, 67, 193, 100, 176, 10, 178, 67, 152, 67, 142, 67, 136, 66, 142, 66, 141, 66, 141, 66, 135, 67, 140, 68, 136, 66, 134, 67, 140, 67, 193, 100, 175, 10, 179, 67, 152, 68, 141, 67, 136, 66, 142, 66, 141, 66, 141, 66, 136, 67, 138, 69, 136, 66, 134, 67, 139, 68, 255, 150, 10, 179, 67, 153, 68, 139, 67, 137, 66, 142, 66, 141, 66, 141, 66, 136, 67, 137, 70, 136, 66, 135, 67, 137, 69, 255, 149, 10, 180, 67, 154, 68, 136, 68, 138, 66, 142, 66, 141, 66, 141, 66, 137, 68, 134, 71, 136, 66, 135, 68, 135, 70, 255, 148, 11, 180, 67, 155, 81, 138, 66, 142, 66, 141, 66, 141, 66, 137, 78, 128, 67, 136, 66, 136, 82, 255, 147, 11, 181, 67, 156, 78, 140, 66, 142, 66, 141, 66, 141, 66, 138, 76, 129, 67, 136, 66, 137, 76, 129, 66, 255, 146, 12, 181, 67, 159, 74, 141, 66, 142, 66, 141, 66, 141, 66, 140, 72, 131, 67, 136, 66, 139, 72, 131, 66, 255, 145, 12, 255, 255, 255, 130, 11, 255, 255, 213, 64, 171, 12, 134, 72, 255, 255, 165, 65, 155, 67, 169, 12, 133, 76, 255, 255, 162, 67, 154, 67, 168, 13, 132, 78, 255, 255, 161, 66, 156, 67, 166, 13, 132, 80, 255, 255, 160, 66, 156, 67, 165, 14, 130, 83, 255, 255, 158, 67, 157, 66, 164, 14, 131, 84, 255, 255, 157, 66, 158, 67, 163, 14, 130, 85, 255, 255, 157, 66, 158, 67, 162, 14, 131, 86, 255, 255, 156, 66, 158, 67, 161, 15, 131, 86, 255, 255, 156, 66, 158, 67, 160, 16, 130, 87, 255, 255, 156, 66, 158, 67, 159, 17, 130, 87, 255, 255, 156, 66, 158, 67, 159, 17, 130, 87, 255, 255, 156, 66, 158, 67, 158, 18, 130, 87, 255, 255, 156, 66, 158, 67, 157, 19, 130, 87, 255, 255, 156, 67, 157, 67, 158, 18, 131, 86, 255, 255, 156, 67, 157, 66, 159, 19, 130, 86, 137, 63, 63, 63, 63, 16, 130, 66, 156, 67, 160, 18, 131, 84, 138, 63, 63, 63, 63, 16, 130, 67, 155, 67, 160, 18, 131, 84, 138, 63, 63, 63, 63, 16, 131, 66, 154, 67, 162, 18, 131, 82, 139, 63, 63, 63, 63, 16, 131, 67, 153, 67, 163, 17, 132, 80, 140, 63, 63, 63, 63, 16, 132, 67, 151, 67, 164, 18, 132, 78, 255, 255, 163, 68, 149, 68, 165, 18, 132, 76, 255, 255, 165, 68, 147, 68, 166, 19, 133, 72, 255, 255, 168, 69, 143, 69, 168, 19, 255, 255, 183, 70, 139, 70, 169, 21, 255, 255, 182, 73, 133, 71, 171, 20, 149, 127, 127, 127, 127, 87, 138, 84, 172, 19, 150, 127, 127, 127, 127, 87, 140, 80, 175, 17, 151, 127, 127, 127, 127, 87, 143, 75, 177, 16, 255, 255, 255, 15, 255, 255, 255, 14, 255, 255, 255, 14, 255, 255, 255, 128, 13, 255, 255, 255, 128, 14, 255, 255, 255, 128, 13, 255, 255, 255, 128, 13, 255, 255, 255, 128, 13, 255, 255, 184, 92, 171, 12, 255, 255, 185, 95, 167, 13, 255, 255, 185, 96, 166, 12, 255, 255, 213, 70, 164, 12, 255, 255, 217, 68, 163, 11, 255, 255, 219, 68, 161, 12, 255, 255, 220, 67, 161, 11, 255, 255, 222, 67, 159, 11, 255, 255, 223, 67, 158, 11, 255, 166, 81, 255, 168, 66, 158, 11, 255, 166, 83, 192, 67, 225, 67, 157, 10, 255, 167, 85, 189, 68, 225, 67, 156, 10, 255, 168, 67, 138, 71, 188, 68, 225, 67, 155, 10, 255, 169, 67, 141, 68, 189, 67, 225, 67, 155, 9, 255, 170, 67, 142, 68, 255, 162, 67, 154, 10, 255, 170, 67, 143, 67, 255, 162, 67, 154, 9, 255, 171, 67, 143, 67, 255, 162, 67, 153, 9, 255, 172, 67, 143, 68, 255, 161, 66, 158, 4, 255, 173, 67, 143, 68, 255, 160, 67, 161, 0, 255, 174, 67, 143, 68, 255, 159, 67, 255, 210, 67, 143, 67, 255, 160, 67, 255, 210, 67, 143, 67, 140, 71, 148, 71, 159, 70, 148, 71, 168, 68, 154, 71, 255, 176, 67, 142, 68, 137, 76, 144, 75, 136, 66, 136, 66, 131, 74, 144, 75, 131, 66, 157, 70, 153, 73, 255, 175, 67, 141, 68, 136, 79, 141, 79, 134, 66, 136, 66, 129, 77, 141, 79, 129, 66, 129, 97, 153, 75, 255, 174, 67, 140, 69, 135, 70, 133, 68, 139, 70, 133, 68, 133, 66, 136, 73, 132, 68, 139, 71, 132, 72, 129, 95, 154, 76, 255, 174, 85, 137, 66, 137, 67, 138, 68, 137, 67, 133, 66, 136, 70, 136, 67, 138, 69, 137, 70, 129, 93, 156, 76, 255, 174, 84, 152, 67, 136, 68, 139, 65, 134, 66, 136, 69, 137, 68, 136, 68, 140, 69, 188, 76, 255, 174, 82, 154, 67, 136, 67, 149, 66, 136, 68, 139, 67, 136, 67, 142, 68, 188, 76, 255, 174, 80, 156, 67, 135, 67, 150, 66, 136, 67, 140, 67, 135, 68, 143, 67, 188, 76, 255, 174, 67, 134, 67, 159, 66, 135, 67, 150, 66, 136, 67, 140, 67, 135, 67, 144, 67, 188, 76, 255, 174, 67, 135, 67, 158, 66, 134, 67, 151, 66, 136, 67, 141, 66, 135, 67, 144, 67, 189, 75, 255, 174, 67, 135, 68, 152, 71, 134, 67, 151, 66, 136, 67, 141, 66, 135, 66, 146, 66, 185, 0, 131, 73, 255, 175, 67, 136, 67, 145, 78, 134, 67, 151, 66, 136, 66, 142, 66, 134, 67, 146, 66, 185, 1, 132, 70, 255, 176, 67, 137, 67, 142, 80, 134, 67, 151, 66, 136, 66, 142, 66, 134, 67, 146, 66, 184, 3, 255, 187, 67, 137, 68, 140, 71, 134, 66, 134, 67, 151, 66, 136, 66, 142, 66, 134, 67, 146, 66, 184, 4, 255, 186, 67, 138, 67, 139, 68, 138, 66, 134, 67, 151, 66, 136, 66, 142, 66, 135, 66, 146, 66, 183, 4, 255, 187, 67, 138, 68, 137, 67, 140, 66, 134, 67, 151, 66, 136, 66, 142, 66, 135, 67, 144, 67, 182, 5, 255, 187, 67, 139, 67, 137, 67, 139, 67, 135, 66, 151, 66, 136, 66, 142, 66, 135, 67, 144, 67, 182, 4, 255, 188, 67, 139, 68, 135, 67, 140, 67, 135, 67, 150, 66, 136, 66, 142, 66, 135, 67, 144, 67, 181, 4, 255, 189, 67, 140, 68, 134, 67, 140, 67, 135, 67, 150, 66, 136, 66, 142, 66, 136, 67, 142, 68, 181, 3, 255, 190, 67, 141, 67, 134, 67, 140, 67, 136, 67, 149, 66, 136, 66, 142, 66, 136, 68, 140, 69, 180, 4, 255, 190, 67, 141, 68, 134, 67, 138, 68, 136, 68, 139, 65, 134, 66, 136, 66, 142, 66, 137, 68, 138, 70, 180, 3, 255, 191, 67, 142, 67, 134, 68, 136, 69, 137, 68, 137, 67, 133, 66, 136, 66, 142, 66, 138, 69, 134, 72, 179, 3, 255, 192, 67, 142, 68, 134, 82, 138, 81, 133, 66, 136, 66, 142, 66, 139, 79, 129, 66, 179, 2, 255, 193, 67, 143, 68, 134, 76, 129, 66, 139, 79, 134, 66, 136, 66, 142, 66, 141, 76, 130, 66, 178, 3, 255, 193, 67, 144, 67, 135, 74, 130, 67, 140, 75, 136, 66, 136, 66, 142, 66, 143, 72, 132, 66, 178, 2, 255, 230, 69, 153, 69, 201, 66, 177, 2, 255, 255, 215, 66, 177, 1, 255, 255, 215, 67, 176, 1, 255, 255, 216, 67, 176, 1, 255, 255, 216, 67, 175, 1, 255, 255, 217, 66, 175, 1, 255, 255, 199, 65, 143, 67, 175, 0, 255, 255, 198, 68, 141, 67, 175, 1, 255, 255, 198, 69, 139, 68, 255, 255, 250, 71, 133, 69, 175, 0, 255, 255, 203, 81, 255, 255, 255, 77, 255, 255, 255, 133, 71, 255, 255, 255, 152};

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
    
    UG_FillScreen(C_WHITE);
    HW_DrawImage_Special(40,68,399+40,68+135,logo,1970);
    CyDelay(2000);
    
    //Tach Meter Stuff
    //uint8_t value=0; // replace the value with 
    //int8_t direction=1;
    
    //precharging time counter
    //volatile uint32_t PrechargingTimeCount = 0;
    //uint32_t DriveTimeCount = 0;

    CyGlobalIntEnable;
    
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
        BMS_LED_Write((shutdown_flags >> 4) & 0x01); //bitmask for BMS_OK
        IMD_LED_Write((shutdown_flags >> 5) & 0x01); // bitmask for IMD_OK 
        
        /*      START display latest data       */
        disp_SOC(soc);
        
        if(PACK_TEMP > 99) PACK_TEMP = 99;  //make sure temp is no more than 2 digits```
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



/* [] END OF FILE */
