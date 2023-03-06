/*******************************************************************************
* File Name: ReadyToDrive_Timer_PM.c
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

#include "ReadyToDrive_Timer.h"

static ReadyToDrive_Timer_backupStruct ReadyToDrive_Timer_backup;


/*******************************************************************************
* Function Name: ReadyToDrive_Timer_SaveConfig
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
*  ReadyToDrive_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void ReadyToDrive_Timer_SaveConfig(void) 
{
    #if (!ReadyToDrive_Timer_UsingFixedFunction)
        ReadyToDrive_Timer_backup.TimerUdb = ReadyToDrive_Timer_ReadCounter();
        ReadyToDrive_Timer_backup.InterruptMaskValue = ReadyToDrive_Timer_STATUS_MASK;
        #if (ReadyToDrive_Timer_UsingHWCaptureCounter)
            ReadyToDrive_Timer_backup.TimerCaptureCounter = ReadyToDrive_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!ReadyToDrive_Timer_UDB_CONTROL_REG_REMOVED)
            ReadyToDrive_Timer_backup.TimerControlRegister = ReadyToDrive_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: ReadyToDrive_Timer_RestoreConfig
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
*  ReadyToDrive_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void ReadyToDrive_Timer_RestoreConfig(void) 
{   
    #if (!ReadyToDrive_Timer_UsingFixedFunction)

        ReadyToDrive_Timer_WriteCounter(ReadyToDrive_Timer_backup.TimerUdb);
        ReadyToDrive_Timer_STATUS_MASK =ReadyToDrive_Timer_backup.InterruptMaskValue;
        #if (ReadyToDrive_Timer_UsingHWCaptureCounter)
            ReadyToDrive_Timer_SetCaptureCount(ReadyToDrive_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!ReadyToDrive_Timer_UDB_CONTROL_REG_REMOVED)
            ReadyToDrive_Timer_WriteControlRegister(ReadyToDrive_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: ReadyToDrive_Timer_Sleep
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
*  ReadyToDrive_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void ReadyToDrive_Timer_Sleep(void) 
{
    #if(!ReadyToDrive_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(ReadyToDrive_Timer_CTRL_ENABLE == (ReadyToDrive_Timer_CONTROL & ReadyToDrive_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            ReadyToDrive_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            ReadyToDrive_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    ReadyToDrive_Timer_Stop();
    ReadyToDrive_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: ReadyToDrive_Timer_Wakeup
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
*  ReadyToDrive_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void ReadyToDrive_Timer_Wakeup(void) 
{
    ReadyToDrive_Timer_RestoreConfig();
    #if(!ReadyToDrive_Timer_UDB_CONTROL_REG_REMOVED)
        if(ReadyToDrive_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                ReadyToDrive_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
