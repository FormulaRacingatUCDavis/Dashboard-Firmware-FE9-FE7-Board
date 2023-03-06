/*******************************************************************************
* File Name: BMS_LED.c  
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
#include "BMS_LED.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 BMS_LED__PORT == 15 && ((BMS_LED__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: BMS_LED_Write
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
void BMS_LED_Write(uint8 value) 
{
    uint8 staticBits = (BMS_LED_DR & (uint8)(~BMS_LED_MASK));
    BMS_LED_DR = staticBits | ((uint8)(value << BMS_LED_SHIFT) & BMS_LED_MASK);
}


/*******************************************************************************
* Function Name: BMS_LED_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  BMS_LED_DM_STRONG     Strong Drive 
*  BMS_LED_DM_OD_HI      Open Drain, Drives High 
*  BMS_LED_DM_OD_LO      Open Drain, Drives Low 
*  BMS_LED_DM_RES_UP     Resistive Pull Up 
*  BMS_LED_DM_RES_DWN    Resistive Pull Down 
*  BMS_LED_DM_RES_UPDWN  Resistive Pull Up/Down 
*  BMS_LED_DM_DIG_HIZ    High Impedance Digital 
*  BMS_LED_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void BMS_LED_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(BMS_LED_0, mode);
}


/*******************************************************************************
* Function Name: BMS_LED_Read
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
*  Macro BMS_LED_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 BMS_LED_Read(void) 
{
    return (BMS_LED_PS & BMS_LED_MASK) >> BMS_LED_SHIFT;
}


/*******************************************************************************
* Function Name: BMS_LED_ReadDataReg
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
uint8 BMS_LED_ReadDataReg(void) 
{
    return (BMS_LED_DR & BMS_LED_MASK) >> BMS_LED_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(BMS_LED_INTSTAT) 

    /*******************************************************************************
    * Function Name: BMS_LED_ClearInterrupt
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
    uint8 BMS_LED_ClearInterrupt(void) 
    {
        return (BMS_LED_INTSTAT & BMS_LED_MASK) >> BMS_LED_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
