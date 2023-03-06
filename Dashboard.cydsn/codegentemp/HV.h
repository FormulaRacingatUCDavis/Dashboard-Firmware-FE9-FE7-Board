/*******************************************************************************
* File Name: HV.h  
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

#if !defined(CY_PINS_HV_H) /* Pins HV_H */
#define CY_PINS_HV_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "HV_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 HV__PORT == 15 && ((HV__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    HV_Write(uint8 value);
void    HV_SetDriveMode(uint8 mode);
uint8   HV_ReadDataReg(void);
uint8   HV_Read(void);
void    HV_SetInterruptMode(uint16 position, uint16 mode);
uint8   HV_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the HV_SetDriveMode() function.
     *  @{
     */
        #define HV_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define HV_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define HV_DM_RES_UP          PIN_DM_RES_UP
        #define HV_DM_RES_DWN         PIN_DM_RES_DWN
        #define HV_DM_OD_LO           PIN_DM_OD_LO
        #define HV_DM_OD_HI           PIN_DM_OD_HI
        #define HV_DM_STRONG          PIN_DM_STRONG
        #define HV_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define HV_MASK               HV__MASK
#define HV_SHIFT              HV__SHIFT
#define HV_WIDTH              1u

/* Interrupt constants */
#if defined(HV__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in HV_SetInterruptMode() function.
     *  @{
     */
        #define HV_INTR_NONE      (uint16)(0x0000u)
        #define HV_INTR_RISING    (uint16)(0x0001u)
        #define HV_INTR_FALLING   (uint16)(0x0002u)
        #define HV_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define HV_INTR_MASK      (0x01u) 
#endif /* (HV__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define HV_PS                     (* (reg8 *) HV__PS)
/* Data Register */
#define HV_DR                     (* (reg8 *) HV__DR)
/* Port Number */
#define HV_PRT_NUM                (* (reg8 *) HV__PRT) 
/* Connect to Analog Globals */                                                  
#define HV_AG                     (* (reg8 *) HV__AG)                       
/* Analog MUX bux enable */
#define HV_AMUX                   (* (reg8 *) HV__AMUX) 
/* Bidirectional Enable */                                                        
#define HV_BIE                    (* (reg8 *) HV__BIE)
/* Bit-mask for Aliased Register Access */
#define HV_BIT_MASK               (* (reg8 *) HV__BIT_MASK)
/* Bypass Enable */
#define HV_BYP                    (* (reg8 *) HV__BYP)
/* Port wide control signals */                                                   
#define HV_CTL                    (* (reg8 *) HV__CTL)
/* Drive Modes */
#define HV_DM0                    (* (reg8 *) HV__DM0) 
#define HV_DM1                    (* (reg8 *) HV__DM1)
#define HV_DM2                    (* (reg8 *) HV__DM2) 
/* Input Buffer Disable Override */
#define HV_INP_DIS                (* (reg8 *) HV__INP_DIS)
/* LCD Common or Segment Drive */
#define HV_LCD_COM_SEG            (* (reg8 *) HV__LCD_COM_SEG)
/* Enable Segment LCD */
#define HV_LCD_EN                 (* (reg8 *) HV__LCD_EN)
/* Slew Rate Control */
#define HV_SLW                    (* (reg8 *) HV__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define HV_PRTDSI__CAPS_SEL       (* (reg8 *) HV__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define HV_PRTDSI__DBL_SYNC_IN    (* (reg8 *) HV__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define HV_PRTDSI__OE_SEL0        (* (reg8 *) HV__PRTDSI__OE_SEL0) 
#define HV_PRTDSI__OE_SEL1        (* (reg8 *) HV__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define HV_PRTDSI__OUT_SEL0       (* (reg8 *) HV__PRTDSI__OUT_SEL0) 
#define HV_PRTDSI__OUT_SEL1       (* (reg8 *) HV__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define HV_PRTDSI__SYNC_OUT       (* (reg8 *) HV__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(HV__SIO_CFG)
    #define HV_SIO_HYST_EN        (* (reg8 *) HV__SIO_HYST_EN)
    #define HV_SIO_REG_HIFREQ     (* (reg8 *) HV__SIO_REG_HIFREQ)
    #define HV_SIO_CFG            (* (reg8 *) HV__SIO_CFG)
    #define HV_SIO_DIFF           (* (reg8 *) HV__SIO_DIFF)
#endif /* (HV__SIO_CFG) */

/* Interrupt Registers */
#if defined(HV__INTSTAT)
    #define HV_INTSTAT            (* (reg8 *) HV__INTSTAT)
    #define HV_SNAP               (* (reg8 *) HV__SNAP)
    
	#define HV_0_INTTYPE_REG 		(* (reg8 *) HV__0__INTTYPE)
#endif /* (HV__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_HV_H */


/* [] END OF FILE */
