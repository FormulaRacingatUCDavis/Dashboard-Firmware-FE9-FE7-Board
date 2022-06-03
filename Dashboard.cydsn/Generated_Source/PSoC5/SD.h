/*******************************************************************************
* File Name: SD.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_SD_H) /* Pins SD_H */
#define CY_PINS_SD_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SD_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SD__PORT == 15 && ((SD__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    SD_Write(uint8 value);
void    SD_SetDriveMode(uint8 mode);
uint8   SD_ReadDataReg(void);
uint8   SD_Read(void);
void    SD_SetInterruptMode(uint16 position, uint16 mode);
uint8   SD_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the SD_SetDriveMode() function.
     *  @{
     */
        #define SD_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define SD_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define SD_DM_RES_UP          PIN_DM_RES_UP
        #define SD_DM_RES_DWN         PIN_DM_RES_DWN
        #define SD_DM_OD_LO           PIN_DM_OD_LO
        #define SD_DM_OD_HI           PIN_DM_OD_HI
        #define SD_DM_STRONG          PIN_DM_STRONG
        #define SD_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define SD_MASK               SD__MASK
#define SD_SHIFT              SD__SHIFT
#define SD_WIDTH              8u

/* Interrupt constants */
#if defined(SD__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SD_SetInterruptMode() function.
     *  @{
     */
        #define SD_INTR_NONE      (uint16)(0x0000u)
        #define SD_INTR_RISING    (uint16)(0x0001u)
        #define SD_INTR_FALLING   (uint16)(0x0002u)
        #define SD_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define SD_INTR_MASK      (0x01u) 
#endif /* (SD__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SD_PS                     (* (reg8 *) SD__PS)
/* Data Register */
#define SD_DR                     (* (reg8 *) SD__DR)
/* Port Number */
#define SD_PRT_NUM                (* (reg8 *) SD__PRT) 
/* Connect to Analog Globals */                                                  
#define SD_AG                     (* (reg8 *) SD__AG)                       
/* Analog MUX bux enable */
#define SD_AMUX                   (* (reg8 *) SD__AMUX) 
/* Bidirectional Enable */                                                        
#define SD_BIE                    (* (reg8 *) SD__BIE)
/* Bit-mask for Aliased Register Access */
#define SD_BIT_MASK               (* (reg8 *) SD__BIT_MASK)
/* Bypass Enable */
#define SD_BYP                    (* (reg8 *) SD__BYP)
/* Port wide control signals */                                                   
#define SD_CTL                    (* (reg8 *) SD__CTL)
/* Drive Modes */
#define SD_DM0                    (* (reg8 *) SD__DM0) 
#define SD_DM1                    (* (reg8 *) SD__DM1)
#define SD_DM2                    (* (reg8 *) SD__DM2) 
/* Input Buffer Disable Override */
#define SD_INP_DIS                (* (reg8 *) SD__INP_DIS)
/* LCD Common or Segment Drive */
#define SD_LCD_COM_SEG            (* (reg8 *) SD__LCD_COM_SEG)
/* Enable Segment LCD */
#define SD_LCD_EN                 (* (reg8 *) SD__LCD_EN)
/* Slew Rate Control */
#define SD_SLW                    (* (reg8 *) SD__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SD_PRTDSI__CAPS_SEL       (* (reg8 *) SD__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SD_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SD__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SD_PRTDSI__OE_SEL0        (* (reg8 *) SD__PRTDSI__OE_SEL0) 
#define SD_PRTDSI__OE_SEL1        (* (reg8 *) SD__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SD_PRTDSI__OUT_SEL0       (* (reg8 *) SD__PRTDSI__OUT_SEL0) 
#define SD_PRTDSI__OUT_SEL1       (* (reg8 *) SD__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SD_PRTDSI__SYNC_OUT       (* (reg8 *) SD__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(SD__SIO_CFG)
    #define SD_SIO_HYST_EN        (* (reg8 *) SD__SIO_HYST_EN)
    #define SD_SIO_REG_HIFREQ     (* (reg8 *) SD__SIO_REG_HIFREQ)
    #define SD_SIO_CFG            (* (reg8 *) SD__SIO_CFG)
    #define SD_SIO_DIFF           (* (reg8 *) SD__SIO_DIFF)
#endif /* (SD__SIO_CFG) */

/* Interrupt Registers */
#if defined(SD__INTSTAT)
    #define SD_INTSTAT            (* (reg8 *) SD__INTSTAT)
    #define SD_SNAP               (* (reg8 *) SD__SNAP)
    
	#define SD_0_INTTYPE_REG 		(* (reg8 *) SD__0__INTTYPE)
	#define SD_1_INTTYPE_REG 		(* (reg8 *) SD__1__INTTYPE)
	#define SD_2_INTTYPE_REG 		(* (reg8 *) SD__2__INTTYPE)
	#define SD_3_INTTYPE_REG 		(* (reg8 *) SD__3__INTTYPE)
	#define SD_4_INTTYPE_REG 		(* (reg8 *) SD__4__INTTYPE)
	#define SD_5_INTTYPE_REG 		(* (reg8 *) SD__5__INTTYPE)
	#define SD_6_INTTYPE_REG 		(* (reg8 *) SD__6__INTTYPE)
	#define SD_7_INTTYPE_REG 		(* (reg8 *) SD__7__INTTYPE)
#endif /* (SD__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SD_H */


/* [] END OF FILE */
