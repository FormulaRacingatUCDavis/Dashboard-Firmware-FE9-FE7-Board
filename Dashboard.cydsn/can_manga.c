/* ========================================
 * Can API for FRUCD
 * Schuyler Alschuler 2016 - 2017
 * ========================================
*/

/*******************************************
 * Used to interface dashboard with CAN bus
 * some functinality can be found in CAN_TX_RX_func.c
 * which is a generated source file
 * see the can component of the top design for specific mailboxes
 ******************************************/

#include "CAN.h"
#include "can_manga.h"
#include "cytypes.h"
//#include "data.h"
#include "frucd_display.h"

extern volatile uint32_t pedalOK;
extern volatile double THROTTLE_MULTIPLIER;
extern const double THROTTLE_MAP[8];
extern volatile uint8_t PACK_TEMP;
extern volatile uint8_t BSPD_CATCH;
extern volatile int32 CURRENT;
extern volatile int ERROR_NODE;
extern volatile int ERROR_IDX;

volatile uint8_t CAPACITOR_VOLT = 0;
volatile uint8_t CURTIS_FAULT_CHECK = 0;
volatile uint8_t CURTIS_HEART_BEAT_CHECK = 0;
volatile uint8_t ACK_RX = 0;
volatile uint8_t ERROR_TOLERANCE = 0;
volatile uint8_t ABS_MOTOR_RPM = 0;
volatile uint8_t THROTTLE_HIGH = 0;
volatile uint8_t THROTTLE_LOW = 0;

volatile uint8_t ESTOP; //Tehya test

volatile uint8_t VOLT_B1;
volatile uint8_t VOLT_B2;
volatile uint8_t VOLT_B3;
volatile uint8_t VOLT_B4;
extern volatile uint32_t voltage;
extern volatile BMS_STATUS bms_status;


uint8 current_bytes[4] = {0};

// returns voltage of capacitor from motor controller
uint8_t getCapacitorVoltage()
{
    return CAPACITOR_VOLT;
}

// returns true if there is a fault in motor controller
uint8_t getCurtisFaultCheck()
{
    return CURTIS_FAULT_CHECK;
}

// returns true if motor controller node is still active
// if node is in active check the fault code using 1314
uint8_t getCurtisHeartBeatCheck()
{
    return CURTIS_HEART_BEAT_CHECK;
}

// returns 1 always... not sure whats up in the motor controller
uint8_t getAckRx()
{
    return ACK_RX;
}

// true if break depressed more than 10%
// this value typically used for drive request
uint8_t getErrorTolerance()
{
    return ERROR_TOLERANCE;
}

// from motor controller
// look this one up in 1239E manual, im not super sure
uint8_t getABSMotorRPM()
{
    return ABS_MOTOR_RPM;
}

// throttle is sent in two 8 bit values to create one 16 bit value
// hight and low refer to high and low bit sequences of the whole value
uint8_t getPedalLow()
{
    return THROTTLE_LOW;
}

uint8_t getPedalHigh()
{
    return THROTTLE_HIGH;
}

//Tehya test
//will be a 1 if the estop was pressed (I'm making it a special case because I want to test rn)
uint8_t getEStop()
{
    return ESTOP;
}

// called from CAN_TX_RX_func.c in the generic RX func
// tldr: part of an interrupt service routine
void can_receive(uint8_t *msg, int ID)
{
    uint8 InterruptState = CyEnterCriticalSection();
    disp_mc_temp(69);
    
    
    uint8_t data[8];
    int i = 0;
    for (i = 0; i < 8; i++)
        data[i] = msg[i];
        
    switch (ID) 
    {
        case 0x0566: // Curtis Status
            CAPACITOR_VOLT = msg[CAN_DATA_BYTE_1];
            ABS_MOTOR_RPM = msg[CAN_DATA_BYTE_5];
            break;
        case 0xA6:  // errors sent from MC node
            CURTIS_FAULT_CHECK = 0x1;
            break;
        case 0x726:     // from motor controller to confirm that node is still active
            CURTIS_HEART_BEAT_CHECK = 0x1;
            break;
        case 0x0666:    // pdoAwk from motor controller
            ACK_RX = msg[CAN_DATA_BYTE_1];
            break;
        case 0x0201:    // BSPD (brake position from pedal node)
            ERROR_TOLERANCE = msg[CAN_DATA_BYTE_1];
            BSPD_CATCH = msg[CAN_DATA_BYTE_5];
            break;
        case 0x0200:    // throttle
            pedalOK = 0x0;
            THROTTLE_HIGH = data[CAN_DATA_BYTE_2];
            THROTTLE_LOW = data[CAN_DATA_BYTE_3];
            break;
        case 0x388:    // BMS voltage data 
            VOLT_B4 = data[CAN_DATA_BYTE_5];
            VOLT_B3 = data[CAN_DATA_BYTE_6];
            VOLT_B2 = data[CAN_DATA_BYTE_7];
            VOLT_B1 = data[CAN_DATA_BYTE_8];
            voltage = (VOLT_B4 << 24) | (VOLT_B3 << 16) | (VOLT_B2 << 8) | VOLT_B1; 
            break;
        case 0x389:   // BMS Temp data
            //ERROR_IDX = data[CAN_DATA_BYTE_6];
            //ERROR_NODE = data[CAN_DATA_BYTE_7];
            PACK_TEMP = data[CAN_DATA_BYTE_8];
            disp_max_pack_temp(PACK_TEMP);
            //tempAttenuate();
            break;
        case 0x521: // current data from IVT
            current_bytes[0] = data[CAN_DATA_BYTE_3];
            current_bytes[1] = data[CAN_DATA_BYTE_4];
            current_bytes[2] = data[CAN_DATA_BYTE_5];
            current_bytes[3] = data[CAN_DATA_BYTE_6];
            CURRENT = (current_bytes[0] << 24) + (current_bytes[1] << 16) + (current_bytes[2] << 8) + current_bytes[3]; // check CAN to be sure
            break;
        case 0x188:
            bms_status = (data[CAN_DATA_BYTE_3] << 8) | data[CAN_DATA_BYTE_4];  
            break;
        case 0x366: //Tehya test
            ESTOP = data[CAN_DATA_BYTE_1];
            break;
    }
    
    CyExitCriticalSection(InterruptState);
}

void can_test_send()
{
    //CAN_1_DATA_BYTES_MSG msg;
    CAN_TX_MSG HeartB;
    
    HeartB.id = 0x300;
    HeartB.rtr = 0;
    HeartB.ide = 0;
    HeartB.dlc = 1;
    
    HeartB.irq = 0;
    
    HeartB.msg->byte[0] = (uint8) 1;
    HeartB.msg->byte[1] = (uint8) 1;
    HeartB.msg->byte[2] = (uint8) 1;
    HeartB.msg->byte[3] = (uint8) 1;
    HeartB.msg->byte[4] = (uint8) 1;
    HeartB.msg->byte[5] = (uint8) 1;
    HeartB.msg->byte[6] = (uint8) 1;
    HeartB.msg->byte[7] = (uint8) 1;
    
    CAN_SendMsg(&HeartB);
}

void can_send(uint8_t data[8], uint32_t ID)
{
    uint8_t i;
	CAN_TX_MSG message;
	CAN_DATA_BYTES_MSG payload;
	message.id = ID; 
	message.rtr = 0;
	message.ide = 0;
	message.dlc = 0x08;
	message.irq = 0;
	message.msg = &payload;
	for(i=0;i<8;i++)
		payload.byte[i] = data[i];
	CAN_SendMsg(&message); 
}

void can_send_status(
    uint8_t state,
    uint8_t errorState)
{
    //max and min voltage means the voltage of single cell
        uint8_t data[8];
        
        data[0] = state;
        data[1] = errorState;
        
        data[2] = 0;
        data[3] = 0;
        
        data[4] = 0;
        data[5] = 0;
        data[6] = 0;
        data[7] = 0;

        can_send(data, 0x626);

        
} // can_send_status()

void can_send_cmd(
    uint8_t SetInterlock,
    uint16_t VCL_Throttle_High,
    uint16_t VCL_Throttle_Low,
    uint8_t E_Stop_Check
)
{
    //max and min voltage means the voltage of single cell
        
        uint8_t data[8];
        
        data[0] = SetInterlock;
        
        data[1] = VCL_Throttle_High;
        data[2] = VCL_Throttle_Low;
        
        data[3] = 0;
        data[4] = E_Stop_Check;
        
        data[5] = 0;
        data[6] = 0;
        data[7] = 0;

        can_send(data, 0x766);
        CyDelay(1); // Wtf is this shit?

} // can_send_cmd()

void can_send_charge(uint8_t charge, uint8_t save_soc) {
    uint8_t data[8] = {0};
    data[0] = charge;
    data[1] = save_soc;
    
    can_send(data, 0x389);
}
/* MOVE TO VCU
void tempAttenuate() {
    int t = PACK_TEMP - 50;
    if (t < 0) {
        THROTTLE_MULTIPLIER = 1;   
    } else if (t < 8) {
        THROTTLE_MULTIPLIER = THROTTLE_MAP[t] / 100.0; 
    } else if (t >= 8) {
        THROTTLE_MULTIPLIER = THROTTLE_MAP[7] / 100.0; 
    }
}*/

