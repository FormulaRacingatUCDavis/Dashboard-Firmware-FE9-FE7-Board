/*******************************************************************************
* File Name: Node_Timer_PM.c
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

#include "Node_Timer.h"

static Node_Timer_backupStruct Node_Timer_backup;


/*******************************************************************************
* Function Name: Node_Timer_SaveConfig
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
*  Node_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Node_Timer_SaveConfig(void) 
{
    #if (!Node_Timer_UsingFixedFunction)
        Node_Timer_backup.TimerUdb = Node_Timer_ReadCounter();
        Node_Timer_backup.InterruptMaskValue = Node_Timer_STATUS_MASK;
        #if (Node_Timer_UsingHWCaptureCounter)
            Node_Timer_backup.TimerCaptureCounter = Node_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Node_Timer_UDB_CONTROL_REG_REMOVED)
            Node_Timer_backup.TimerControlRegister = Node_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Node_Timer_RestoreConfig
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
*  Node_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Node_Timer_RestoreConfig(void) 
{   
    #if (!Node_Timer_UsingFixedFunction)

        Node_Timer_WriteCounter(Node_Timer_backup.TimerUdb);
        Node_Timer_STATUS_MASK =Node_Timer_backup.InterruptMaskValue;
        #if (Node_Timer_UsingHWCaptureCounter)
            Node_Timer_SetCaptureCount(Node_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Node_Timer_UDB_CONTROL_REG_REMOVED)
            Node_Timer_WriteControlRegister(Node_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Node_Timer_Sleep
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
*  Node_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Node_Timer_Sleep(void) 
{
    #if(!Node_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Node_Timer_CTRL_ENABLE == (Node_Timer_CONTROL & Node_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Node_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Node_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Node_Timer_Stop();
    Node_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Node_Timer_Wakeup
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
*  Node_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Node_Timer_Wakeup(void) 
{
    Node_Timer_RestoreConfig();
    #if(!Node_Timer_UDB_CONTROL_REG_REMOVED)
        if(Node_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Node_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
