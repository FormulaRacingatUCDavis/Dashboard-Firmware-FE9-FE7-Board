/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"

#if !defined(LED_H)
#define LED_H


typedef enum color {
    LEDWHITE,
    LEDRED,
    LEDYELLOW,
    LEDGREEN,
    LEDCYAN,
    LEDBLUE,
    LEDMAGENTA,
    LEDOFF
} color_t;

void LED_color(color_t color) {
    switch(color){
        case LEDWHITE:
            RGB1_1_Write(0);
            RGB2_1_Write(0);
            RGB3_1_Write(0);
        break;
        case LEDRED:
            RGB1_1_Write(0);
            RGB2_1_Write(1);
            RGB3_1_Write(1);
        break;
        case LEDYELLOW:
            RGB1_1_Write(0);
            RGB2_1_Write(1);
            RGB3_1_Write(0);
        break;
        case LEDGREEN:
            RGB1_1_Write(1);
            RGB2_1_Write(1);
            RGB3_1_Write(0);
        break;
        case LEDCYAN:
            RGB1_1_Write(1);
            RGB2_1_Write(0);
            RGB3_1_Write(0);
        break;
        case LEDBLUE:
            RGB1_1_Write(1);
            RGB2_1_Write(0);
            RGB3_1_Write(1);
        break;
        case LEDMAGENTA:
            RGB1_1_Write(0);
            RGB2_1_Write(0);
            RGB3_1_Write(1); 
        break;
        case LEDOFF:
            RGB1_1_Write(1);
            RGB2_1_Write(1);
            RGB3_1_Write(1);
        break;
    }
}

void LED_color_wheel(int ms) {
    for (int i = 0; i < 8; i++) {
        LED_color(i);
        CyDelay(ms);
    }
}

#endif
/* [] END OF FILE */
