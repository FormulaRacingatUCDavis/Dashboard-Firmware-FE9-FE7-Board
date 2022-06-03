/*******************************************************************************
* File Name: BMS.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "BMS.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 BMS__PORT == 15 && ((BMS__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: BMS_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void BMS_Write(uint8 value) 
{
    uint8 staticBits = (BMS_DR & (uint8)(~BMS_MASK));
    BMS_DR = staticBits | ((uint8)(value << BMS_SHIFT) & BMS_MASK);
}


/*******************************************************************************
* Function Name: BMS_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  BMS_DM_STRONG     Strong Drive 
*  BMS_DM_OD_HI      Open Drain, Drives High 
*  BMS_DM_OD_LO      Open Drain, Drives Low 
*  BMS_DM_RES_UP     Resistive Pull Up 
*  BMS_DM_RES_DWN    Resistive Pull Down 
*  BMS_DM_RES_UPDWN  Resistive Pull Up/Down 
*  BMS_DM_DIG_HIZ    High Impedance Digital 
*  BMS_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void BMS_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(BMS_0, mode);
}


/*******************************************************************************
* Function Name: BMS_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro BMS_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 BMS_Read(void) 
{
    return (BMS_PS & BMS_MASK) >> BMS_SHIFT;
}


/*******************************************************************************
* Function Name: BMS_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 BMS_ReadDataReg(void) 
{
    return (BMS_DR & BMS_MASK) >> BMS_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(BMS_INTSTAT) 

    /*******************************************************************************
    * Function Name: BMS_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 BMS_ClearInterrupt(void) 
    {
        return (BMS_INTSTAT & BMS_MASK) >> BMS_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
