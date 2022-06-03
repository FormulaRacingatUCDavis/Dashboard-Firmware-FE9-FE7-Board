/*******************************************************************************
* File Name: CAN_Timer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "CAN_Timer.h"

static CAN_Timer_backupStruct CAN_Timer_backup;


/*******************************************************************************
* Function Name: CAN_Timer_SaveConfig
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
*  CAN_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void CAN_Timer_SaveConfig(void) 
{
    #if (!CAN_Timer_UsingFixedFunction)
        CAN_Timer_backup.TimerUdb = CAN_Timer_ReadCounter();
        CAN_Timer_backup.InterruptMaskValue = CAN_Timer_STATUS_MASK;
        #if (CAN_Timer_UsingHWCaptureCounter)
            CAN_Timer_backup.TimerCaptureCounter = CAN_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!CAN_Timer_UDB_CONTROL_REG_REMOVED)
            CAN_Timer_backup.TimerControlRegister = CAN_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: CAN_Timer_RestoreConfig
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
*  CAN_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void CAN_Timer_RestoreConfig(void) 
{   
    #if (!CAN_Timer_UsingFixedFunction)

        CAN_Timer_WriteCounter(CAN_Timer_backup.TimerUdb);
        CAN_Timer_STATUS_MASK =CAN_Timer_backup.InterruptMaskValue;
        #if (CAN_Timer_UsingHWCaptureCounter)
            CAN_Timer_SetCaptureCount(CAN_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!CAN_Timer_UDB_CONTROL_REG_REMOVED)
            CAN_Timer_WriteControlRegister(CAN_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: CAN_Timer_Sleep
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
*  CAN_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void CAN_Timer_Sleep(void) 
{
    #if(!CAN_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(CAN_Timer_CTRL_ENABLE == (CAN_Timer_CONTROL & CAN_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            CAN_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            CAN_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    CAN_Timer_Stop();
    CAN_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: CAN_Timer_Wakeup
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
*  CAN_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void CAN_Timer_Wakeup(void) 
{
    CAN_Timer_RestoreConfig();
    #if(!CAN_Timer_UDB_CONTROL_REG_REMOVED)
        if(CAN_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                CAN_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
