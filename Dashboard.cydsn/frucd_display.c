/* ========================================
 *
 * APIs for updating FRUCD Dashboard display
 *
 * ========================================
*/
#include "frucd_display.h"

// Just a test function that displays elements at the supposed corners of the screen
void calibrateScreen() {
    UG_FillScreen(C_WHITE);
    //libTFT_FillRectangle(0, 0, 10, 10, RED);
    UG_FillFrame(0, 0, 10, 10, C_RED);
    //libTFT_FillRectangle(0, 262, 10, 10, BLUE);
    UG_FillFrame(0, 262, 10, 272, C_BLUE);
    //libTFT_FillRectangle(470, 0, 10, 10, GREEN);
    UG_FillFrame(470, 0, 480, 10, C_GREEN);
    //libTFT_FillRectangle(470, 262, 10, 10, YELLOW);
    UG_FillFrame(470, 262, 480, 272, C_YELLOW);
    CyDelay(300);
}

// Initializes dashboard layout/labels
void initDashTemplate() {
    UG_FillScreen(C_BLACK);
    UG_PutString(68, 10, "PACK SOC");
    UG_PutString(297, 10, "MAX PACK T");
    UG_PutString(10, 195, "STATE:");
    UG_PutString(10, 240, "GLV V:");
    UG_PutString(339, 195, "MC T:");
    UG_PutString(300, 240, "MOTOR T:");
}

/*
 * BEGIN: functions to show data on dash
 * - handle variety of digit counts
 * - color decision logic is in here
 */
void disp_SOC(uint8_t data) {
    UG_FontSelect(&FONT_32X53);
    UG_COLOR color;
    if (data == 100) {
        // handle 100% case
        color = C_GREEN;
        if (color != last_SOC_color) {
            // only draw rectangle if color changed
            UG_FillFrame(30, 35, 210, 170, color);
            last_SOC_color = color;
        }
        UG_PutColorString(55, 80, "100%", C_BLACK, C_GREEN);
    } else {
        // handle other cases
        char data_s[4];
        sprintf(data_s, "%d", data);
        if (data >= 10) {
            // handle 2 digit cases
            // choose color
            if (data > 75) {
                color = C_GREEN;
            } else if (data > 50) {
                color = C_YELLOW;
            } else if (data > 25) {
                color = C_ORANGE;
            } else {
                color = C_RED;
            }
            data_s[2] = '%';
            data_s[3] = '\0';
            if (color != last_SOC_color) {
                // only draw rectangle if color changed
                UG_FillFrame(30, 35, 210, 170, color);
                last_SOC_color = color;
            }
            UG_PutColorString(70, 80, data_s, C_BLACK, color);
        } else {
            // handle 1 digit cases
            data_s[1] = '%';
            data_s[2] = '\0';
            color = C_RED;
            UG_FillFrame(30, 35, 210, 170, C_BLACK);
            UG_FillFrame(30, 35, 210, 170, color);
            UG_PutColorString(90, 80, data_s, C_BLACK, color);
        }   
    }
    UG_FontSelect(&FONT_12X16);
}

void disp_max_pack_temp(uint8_t data) {
    // choose color
    UG_COLOR color;
    if (data < 45) {
        color = C_GREEN;
    } else if (data < 50) {
        color = C_YELLOW;
    } else if (data < 55) {
        color = C_ORANGE;
    } else {
        color = C_RED;
        UG_FillFrame(270, 35, 450, 170, C_BLACK);
        UG_FillFrame(270, 35, 450, 170, color);
    }
    
    if (color != last_max_pack_temp_color) {
        // only draw rectangle if color changed
        UG_FillFrame(270, 35, 450, 170, color);
        last_max_pack_temp_color = color;
    }
    UG_FontSelect(&FONT_32X53);
    char data_s[4];
    sprintf(data_s, "%d", data);
    data_s[2] = 'C';
    data_s[3] = '\0';
    UG_PutColorString(310, 80, data_s, C_BLACK, color);
    UG_FontSelect(&FONT_12X16);
}

void disp_state(uint8_t state, BMS_STATUS bms_status) { // TODO
    UG_COLOR color;
    
    switch(bms_status){        //BMS faults more important than VCU faults
        case PACK_TEMP_OVER: 
        case PACK_TEMP_UNDER:
            color = C_RED;
            state_rect(color);
            UG_PutColorString(100, 195, "BMS TEMP", C_BLACK, color);
            break;
        case LOW_SOC: 
            color = C_RED;
            state_rect(color);
            UG_PutColorString(100, 195, "LOW SOC", C_BLACK, color);
            break;
        case IMBALANCE:
            color = C_RED;
            state_rect(color);
            UG_PutColorString(100, 195, "IMBALANCE", C_BLACK, color);
            break;
        case SPI_FAULT:
            color = C_RED;
            state_rect(color);
            UG_PutColorString(100, 195, "SPI FAULT", C_BLACK, color);
            break;
        case CELL_VOLT_OVER:
            color = C_RED;
            state_rect(color);
            UG_PutColorString(100, 195, "OVERVOLT", C_BLACK, color);
            break;
        case CELL_VOLT_UNDER:
            color = C_RED;
            state_rect(color);
            UG_PutColorString(100, 195, "UNDERVOLT", C_BLACK, color);
            break;
        default:
            // check fault bit
            if (state & 0x80) {
                // *************** FAULTS ***************
                uint8 fault = state & 0x7f; // mask off fault bit
                switch(fault) {
                    case NONE: // STARTUP (effectively)
                        // not obtainable via CAN
                        // would only show when hardcoded on startup
                        color = C_YELLOW;
                        state_rect(color);
                        UG_PutColorString(100, 195, " STARTUP  ", C_BLACK, color);
                        break;
                    case DRIVE_REQUEST_FROM_LV:
                        color = C_RED;
                        state_rect(color);
                        UG_PutColorString(100, 195, "DRV FRM LV", C_BLACK, color);
                        break;
                    case CONSERVATIVE_TIMER_MAXED:
                        color = C_RED;
                        state_rect(color);
                        UG_PutColorString(100, 195, "PRE TM OUT", C_BLACK, color);
                        break;
                    case BRAKE_NOT_PRESSED:
                        color = C_RED;
                        state_rect(color);
                        UG_PutColorString(100, 195, "BR NOT PRS", C_BLACK, color);
                        break;
                    case HV_DISABLED_WHILE_DRIVING:
                        color = C_RED;
                        state_rect(color);
                        UG_PutColorString(100, 195, "HV OFF DRV", C_BLACK, color);
                        break;
                    case SENSOR_DISCREPANCY:
                        color = C_RED;
                        state_rect(color);
                        UG_PutColorString(100, 195, "SNSR DSCRP", C_BLACK, color);
                        break;
                    case BRAKE_IMPLAUSIBLE:
                        color = C_YELLOW;
                        state_rect(color);
                        UG_PutColorString(100, 195, "BSPD TRIPD", C_BLACK, color); //whitespace to clear
                        break;
                    case SHUTDOWN_CIRCUIT_OPEN:
                        color = C_RED;
                        state_rect(color);
                        UG_PutColorString(100, 195, "SHTDWN OPN", C_BLACK, color); //whitespace to clear
                        break;
                    case UNCALIBRATED:
                        color = C_RED;
                        state_rect(color);
                        UG_PutColorString(100, 195, "UNCALIBRATED", C_BLACK, color); //whitespace to clear
                        break;
                }
                
            } else {
                // *************** NO FAULTS ***************
                color = C_GREEN;
                state_rect(color);
                switch(state) {
                    case LV:
                        UG_PutColorString(100, 195, "    LV    ", C_BLACK, color);
                        break;
                    case PRECHARGING:
                        UG_PutColorString(100, 195, "PRECHARGE ", C_BLACK, color);
                        break;
                    case HV_ENABLED:
                        UG_PutColorString(100, 195, "HV ENABLED", C_BLACK, color);
                        break;
                    case DRIVE:
                        UG_PutColorString(100, 195, "  DRIVE   ", C_BLACK, color);
                        break;
                }
            }
            break; 
    }
}

void state_rect(UG_COLOR color){
    if (color != last_state_color) {
        // only draw rectangle if color changed
        UG_FillFrame(95, 185, 240, 215, color);
        last_state_color = color;
    }
}

void disp_glv_v(uint32_t data) {
    // translate from voltage divider measurement to true voltage
    // y = 0.4295x + 18.254
    data *= 859;
    data /= 2000; // 0.4295
    data += 18;
    UG_COLOR color;
    if (data > 1150) {
        color = C_GREEN;
    } else if (data > 1100) {
        color = C_YELLOW;
    } else if (data > 1050) {
        color = C_ORANGE;
    } else {
        color = C_RED;
    }
    if (color != last_glv_v_color) {
        // only draw rectangle if color changed
        UG_FillFrame(95, 230, 182, 260, color);
        last_glv_v_color = color;
    }
    char data_s[7];
    sprintf(data_s, "%d", data);
    // handle 3 digit cases; 2 and 1 are practically impossible
    if (data < 1000) {
        data_s[3] = data_s[2];
        data_s[2] = data_s[1];
        data_s[1] = data_s[0];  
        data_s[0] = ' ';
    }
    // shift over fractional part
    data_s[4] = data_s[3];
    data_s[3] = data_s[2];
    // add syntax
    data_s[2] = '.';
    data_s[5] = 'V';
    data_s[6] = '\0';
    UG_PutColorString(100, 240, data_s, C_BLACK, color);
}

void disp_mc_temp(uint16_t data) {
    if (data >= 100) {
        // handle 3 digit cases
        UG_FillFrame(410, 185, 470, 215, C_GREEN);
        char data_s[5];
        sprintf(data_s, "%d", data);
        data_s[3] = 'C';
        data_s[4] = '\0';
        UG_PutColorString(415, 195, data_s, C_BLACK, C_GREEN);
    } else {
        // handle 2 digit cases
        UG_FillFrame(410, 185, 470, 215, C_GREEN);
        char data_s[4];
        sprintf(data_s, "%d", data);
        data_s[2] = 'C';
        data_s[3] = '\0';
        UG_PutColorString(422, 195, data_s, C_BLACK, C_GREEN);
    }
}

void disp_motor_temp(uint16_t data) {
    if (data >= 100) {
        // handle 3 digit cases
        UG_FillFrame(410, 230, 470, 260, C_GREEN);
        char data_s[5];
        sprintf(data_s, "%d", data);
        data_s[3] = 'C';
        data_s[4] = '\0';
        UG_PutColorString(415, 240, data_s, C_BLACK, C_GREEN);
    } else {
        // handle 2 digit cases
        UG_FillFrame(410, 230, 470, 260, C_GREEN);
        char data_s[4];
        sprintf(data_s, "%d", data);
        data_s[2] = 'C';
        data_s[3] = '\0';
        UG_PutColorString(422, 240, data_s, C_BLACK, C_GREEN);
    }
}

/* [] END OF FILE */
