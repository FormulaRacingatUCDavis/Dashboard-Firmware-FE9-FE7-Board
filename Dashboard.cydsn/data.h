#ifndef DATA_H
#define DATA_H

#include <project.h>

#define CAN_QUEUE_LENGTH 1024
#define DATA_QUEUE_LENGTH 1024

typedef enum{
    LV,
    PRECHARGING,
    HV_ENABLED,
    DRIVE,
    FAULT = 0x80,
} vcu_state;

typedef enum {
    NONE,
    DRIVE_REQUEST_FROM_LV,
    CONSERVATIVE_TIMER_MAXED,
    BRAKE_NOT_PRESSED,
    HV_DISABLED_WHILE_DRIVING,
    SENSOR_DISCREPANCY,
    BRAKE_IMPLAUSIBLE,
    ESTOP
} vcu_fault;

typedef enum{
    NO_ERROR = 0x0000,
    CHARGEMODE = 0x0001,
    PACK_TEMP_OVER = 0x0002,
    FUSE_BLOWN = 0x0004,
    PACK_TEMP_UNDER = 0x0008,
    LOW_SOC = 0x0010,
    CRITICAL_SOC = 0x0020,
    IMBALANCE = 0x0040,
    COM_FAILURE = 0x0080,
    NEG_CONT_CLOSED = 0x0100,
    POS_CONT_CLOSED = 0x0200,
    ISO_FAULT = 0x0400,
    SPI_FAULT = 0x0400,
    CELL_VOLT_OVER = 0x0800,
    CELL_VOLT_UNDER = 0x1000,
    CHARGE_HAULT = 0x2000,
    FULL = 0x4000,
    PRECHARGE_CLOSED = 0x8000
} BMS_STATUS;

typedef struct
{
	uint32_t millicounter;
	uint16_t id; // id is for tracking CAN ID
	uint8_t length;
	uint8_t data[8];
} DataPacket;




#endif
