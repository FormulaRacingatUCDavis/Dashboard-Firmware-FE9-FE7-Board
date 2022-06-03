/*******************************************************************************
* File Name: WDT_Timer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "WDT_Timer.h"

static WDT_Timer_backupStruct WDT_Timer_backup;


/*******************************************************************************
* Function Name: WDT_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  WDT_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void WDT_Timer_SaveConfig(void) 
{
    #if (!WDT_Timer_UsingFixedFunction)
        WDT_Timer_backup.TimerUdb = WDT_Timer_ReadCounter();
        WDT_Timer_backup.InterruptMaskValue = WDT_Timer_STATUS_MASK;
        #if (WDT_Timer_UsingHWCaptureCounter)
            WDT_Timer_backup.TimerCaptureCounter = WDT_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!WDT_Timer_UDB_CONTROL_REG_REMOVED)
            WDT_Timer_backup.TimerControlRegister = WDT_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: WDT_Timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  WDT_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void WDT_Timer_RestoreConfig(void) 
{   
    #if (!WDT_Timer_UsingFixedFunction)

        WDT_Timer_WriteCounter(WDT_Timer_backup.TimerUdb);
        WDT_Timer_STATUS_MASK =WDT_Timer_backup.InterruptMaskValue;
        #if (WDT_Timer_UsingHWCaptureCounter)
            WDT_Timer_SetCaptureCount(WDT_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!WDT_Timer_UDB_CONTROL_REG_REMOVED)
            WDT_Timer_WriteControlRegister(WDT_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: WDT_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  WDT_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void WDT_Timer_Sleep(void) 
{
    #if(!WDT_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(WDT_Timer_CTRL_ENABLE == (WDT_Timer_CONTROL & WDT_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            WDT_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            WDT_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    WDT_Timer_Stop();
    WDT_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: WDT_Timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  WDT_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void WDT_Timer_Wakeup(void) 
{
    WDT_Timer_RestoreConfig();
    #if(!WDT_Timer_UDB_CONTROL_REG_REMOVED)
        if(WDT_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                WDT_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
