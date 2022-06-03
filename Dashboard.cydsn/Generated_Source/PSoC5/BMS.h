/*******************************************************************************
* File Name: BMS.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_BMS_H) /* Pins BMS_H */
#define CY_PINS_BMS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BMS_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BMS__PORT == 15 && ((BMS__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    BMS_Write(uint8 value) ;
void    BMS_SetDriveMode(uint8 mode) ;
uint8   BMS_ReadDataReg(void) ;
uint8   BMS_Read(void) ;
uint8   BMS_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define BMS_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define BMS_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define BMS_DM_RES_UP          PIN_DM_RES_UP
#define BMS_DM_RES_DWN         PIN_DM_RES_DWN
#define BMS_DM_OD_LO           PIN_DM_OD_LO
#define BMS_DM_OD_HI           PIN_DM_OD_HI
#define BMS_DM_STRONG          PIN_DM_STRONG
#define BMS_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define BMS_MASK               BMS__MASK
#define BMS_SHIFT              BMS__SHIFT
#define BMS_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BMS_PS                     (* (reg8 *) BMS__PS)
/* Data Register */
#define BMS_DR                     (* (reg8 *) BMS__DR)
/* Port Number */
#define BMS_PRT_NUM                (* (reg8 *) BMS__PRT) 
/* Connect to Analog Globals */                                                  
#define BMS_AG                     (* (reg8 *) BMS__AG)                       
/* Analog MUX bux enable */
#define BMS_AMUX                   (* (reg8 *) BMS__AMUX) 
/* Bidirectional Enable */                                                        
#define BMS_BIE                    (* (reg8 *) BMS__BIE)
/* Bit-mask for Aliased Register Access */
#define BMS_BIT_MASK               (* (reg8 *) BMS__BIT_MASK)
/* Bypass Enable */
#define BMS_BYP                    (* (reg8 *) BMS__BYP)
/* Port wide control signals */                                                   
#define BMS_CTL                    (* (reg8 *) BMS__CTL)
/* Drive Modes */
#define BMS_DM0                    (* (reg8 *) BMS__DM0) 
#define BMS_DM1                    (* (reg8 *) BMS__DM1)
#define BMS_DM2                    (* (reg8 *) BMS__DM2) 
/* Input Buffer Disable Override */
#define BMS_INP_DIS                (* (reg8 *) BMS__INP_DIS)
/* LCD Common or Segment Drive */
#define BMS_LCD_COM_SEG            (* (reg8 *) BMS__LCD_COM_SEG)
/* Enable Segment LCD */
#define BMS_LCD_EN                 (* (reg8 *) BMS__LCD_EN)
/* Slew Rate Control */
#define BMS_SLW                    (* (reg8 *) BMS__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BMS_PRTDSI__CAPS_SEL       (* (reg8 *) BMS__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BMS_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BMS__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BMS_PRTDSI__OE_SEL0        (* (reg8 *) BMS__PRTDSI__OE_SEL0) 
#define BMS_PRTDSI__OE_SEL1        (* (reg8 *) BMS__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BMS_PRTDSI__OUT_SEL0       (* (reg8 *) BMS__PRTDSI__OUT_SEL0) 
#define BMS_PRTDSI__OUT_SEL1       (* (reg8 *) BMS__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BMS_PRTDSI__SYNC_OUT       (* (reg8 *) BMS__PRTDSI__SYNC_OUT) 


#if defined(BMS__INTSTAT)  /* Interrupt Registers */

    #define BMS_INTSTAT                (* (reg8 *) BMS__INTSTAT)
    #define BMS_SNAP                   (* (reg8 *) BMS__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BMS_H */


/* [] END OF FILE */
