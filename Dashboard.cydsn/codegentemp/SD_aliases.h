/*******************************************************************************
* File Name: SD.h  
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

#if !defined(CY_PINS_SD_ALIASES_H) /* Pins SD_ALIASES_H */
#define CY_PINS_SD_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define SD_0			(SD__0__PC)
#define SD_0_INTR	((uint16)((uint16)0x0001u << SD__0__SHIFT))

#define SD_1			(SD__1__PC)
#define SD_1_INTR	((uint16)((uint16)0x0001u << SD__1__SHIFT))

#define SD_2			(SD__2__PC)
#define SD_2_INTR	((uint16)((uint16)0x0001u << SD__2__SHIFT))

#define SD_3			(SD__3__PC)
#define SD_3_INTR	((uint16)((uint16)0x0001u << SD__3__SHIFT))

#define SD_4			(SD__4__PC)
#define SD_4_INTR	((uint16)((uint16)0x0001u << SD__4__SHIFT))

#define SD_5			(SD__5__PC)
#define SD_5_INTR	((uint16)((uint16)0x0001u << SD__5__SHIFT))

#define SD_6			(SD__6__PC)
#define SD_6_INTR	((uint16)((uint16)0x0001u << SD__6__SHIFT))

#define SD_7			(SD__7__PC)
#define SD_7_INTR	((uint16)((uint16)0x0001u << SD__7__SHIFT))

#define SD_INTR_ALL	 ((uint16)(SD_0_INTR| SD_1_INTR| SD_2_INTR| SD_3_INTR| SD_4_INTR| SD_5_INTR| SD_6_INTR| SD_7_INTR))

#endif /* End Pins SD_ALIASES_H */


/* [] END OF FILE */
