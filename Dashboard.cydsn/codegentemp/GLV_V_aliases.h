/*******************************************************************************
* File Name: GLV_V.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_GLV_V_ALIASES_H) /* Pins GLV_V_ALIASES_H */
#define CY_PINS_GLV_V_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define GLV_V_0			(GLV_V__0__PC)
#define GLV_V_0_INTR	((uint16)((uint16)0x0001u << GLV_V__0__SHIFT))

#define GLV_V_INTR_ALL	 ((uint16)(GLV_V_0_INTR))

#endif /* End Pins GLV_V_ALIASES_H */


/* [] END OF FILE */
