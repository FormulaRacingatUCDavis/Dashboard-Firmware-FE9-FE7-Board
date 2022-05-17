/* ========================================
 *
 * APIs for updating FRUCD Dashboard display
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>
#include "data.h"
#include "ugui.h"
#include "ugui_SSD1963.h"

// color caching variables
//  - avoid redrawing rectangle if color didn't change
UG_COLOR last_SOC_color;
UG_COLOR last_max_pack_temp_color;
UG_COLOR last_state_color;
UG_COLOR last_glv_v_color;
UG_COLOR last_mc_temp_color;
UG_COLOR last_motor_temp_color;

// Just a test function that displays elements at the supposed corners of the screen
void calibrateScreen();

// Initializes dashboard layout/labels
void initDashTemplate();

/*
 * BEGIN: functions to show data on dash
 * - color decision logic is in here
 */
void disp_SOC(int data);
void disp_max_pack_temp(int data);
void disp_state(uint8 state); // TODO
void disp_glv_v(int data); // TODO
void disp_mc_temp(int data);
void disp_motor_temp(int data);

/* [] END OF FILE */
