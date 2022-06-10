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
// indicator LEDs
#include "BMS_LED.h"
#include "IMD_LED.h"

extern volatile vcu_state state;
extern volatile uint32_t pedalOK;
extern volatile uint8_t PACK_TEMP;
extern volatile uint8_t BSPD_CATCH;
extern volatile uint16_t CURRENT;
//extern volatile int ERROR_NODE;
//extern volatile int ERROR_IDX;
extern volatile uint8_t soc;
extern volatile uint32_t voltage;
extern volatile BMS_STATUS bms_status;
extern volatile uint8_t shutdown_flags;

// info from MC and motor
extern volatile uint16_t mc_temp;
extern volatile uint16_t motor_temp;

volatile uint8_t CAPACITOR_VOLT = 0;
volatile uint8_t CURTIS_FAULT_CHECK = 0;
volatile uint8_t CURTIS_HEART_BEAT_CHECK = 0;
volatile uint8_t ACK_RX = 0;
volatile uint8_t ERROR_TOLERANCE = 0;
volatile uint8_t ABS_MOTOR_RPM = 0;
volatile uint8_t THROTTLE_HIGH = 0;
volatile uint8_t THROTTLE_LOW = 0;

//volatile uint8_t ESTOP; //Tehya test


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

//Tehya test
//will be a 1 if the estop was pressed (I'm making it a special case because I want to test rn)
uint8_t getEStop()
{
    return ESTOP;
}

// called from CAN_TX_RX_func.c in the generic RX func
/*/ tldr: part of an interrupt service routine
void can_receive(uint8_t *msg, int ID)
{
    uint8 InterruptState = CyEnterCriticalSection();
    
    switch (ID) 
    {
        case VEHICLE_STATE:
            state = msg[CAN_DATA_BYTE_1];
            break;
        case MC_PDO_SEND:
            CAPACITOR_VOLT = msg[CAN_DATA_BYTE_1];
            ABS_MOTOR_RPM = msg[CAN_DATA_BYTE_3];
            mc_temp = msg[CAN_DATA_BYTE_7] << 8;
            mc_temp += msg[CAN_DATA_BYTE_8];
            break;
        case MC_PDO_ACK:
            ACK_RX = msg[CAN_DATA_BYTE_1];
            motor_temp = msg[CAN_DATA_BYTE_5] << 8;
            motor_temp |= msg[CAN_DATA_BYTE_6];
            break;
        case 0xA6:  // errors sent from MC node
            CURTIS_FAULT_CHECK = 0x1;
            break;
        case 0x726:     // from motor controller to confirm that node is still active
            CURTIS_HEART_BEAT_CHECK = 0x1;
            break;
        case BMS_VOLTAGES:
            voltage = msg[CAN_DATA_BYTE_5] << 24;
            voltage |= msg[CAN_DATA_BYTE_6] << 16;
            voltage |= msg[CAN_DATA_BYTE_7] << 8;
            voltage |= msg[CAN_DATA_BYTE_8];
            break;
        case BMS_STATUS_MSG:
            soc = msg[CAN_DATA_BYTE_2];
            bms_status = msg[CAN_DATA_BYTE_3] << 8;    // bms error flags
            bms_status |= msg[CAN_DATA_BYTE_4];        // bms error flags
            break;
        case BMS_TEMPERATURES:
            PACK_TEMP = msg[CAN_DATA_BYTE_8];
            break;
        case PEI_CURRENT:
            CURRENT = msg[CAN_DATA_BYTE_1] << 8;
            CURRENT |= msg[CAN_DATA_BYTE_2];
            shutdown_flags = msg[CAN_DATA_BYTE_3];
            break;
        case 0x366: //Tehya test
            //ESTOP = msg[CAN_DATA_BYTE_1];
            break;
    }
    
    CyExitCriticalSection(InterruptState);
}*/

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

void can_send_switches(
    uint8_t sw_status)
{
        CAN_TX_DATA_BYTE1(CAN_TX_MAILBOX_DRIVER_SWITCHES) = sw_status; 
        CAN_SendMsgDRIVER_SWITCHES(); 
    
        /*
        uint8_t data[8];
        
        data[0] = sw_status;
        data[1] = 0;
        
        data[2] = 0;
        data[3] = 0;
        
        data[4] = 0;
        data[5] = 0;
        data[6] = 0;
        data[7] = 0;

        can_send(data, DRIVER_SWITCHES);
*/
        
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