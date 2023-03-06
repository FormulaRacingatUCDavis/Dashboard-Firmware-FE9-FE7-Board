/*******************************************************************************
* File Name: GLV_V.h  
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

#if !defined(CY_PINS_GLV_V_H) /* Pins GLV_V_H */
#define CY_PINS_GLV_V_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "GLV_V_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 GLV_V__PORT == 15 && ((GLV_V__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    GLV_V_Write(uint8 value);
void    GLV_V_SetDriveMode(uint8 mode);
uint8   GLV_V_ReadDataReg(void);
uint8   GLV_V_Read(void);
void    GLV_V_SetInterruptMode(uint16 position, uint16 mode);
uint8   GLV_V_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the GLV_V_SetDriveMode() function.
     *  @{
     */
        #define GLV_V_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define GLV_V_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define GLV_V_DM_RES_UP          PIN_DM_RES_UP
        #define GLV_V_DM_RES_DWN         PIN_DM_RES_DWN
        #define GLV_V_DM_OD_LO           PIN_DM_OD_LO
        #define GLV_V_DM_OD_HI           PIN_DM_OD_HI
        #define GLV_V_DM_STRONG          PIN_DM_STRONG
        #define GLV_V_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define GLV_V_MASK               GLV_V__MASK
#define GLV_V_SHIFT              GLV_V__SHIFT
#define GLV_V_WIDTH              1u

/* Interrupt constants */
#if defined(GLV_V__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in GLV_V_SetInterruptMode() function.
     *  @{
     */
        #define GLV_V_INTR_NONE      (uint16)(0x0000u)
        #define GLV_V_INTR_RISING    (uint16)(0x0001u)
        #define GLV_V_INTR_FALLING   (uint16)(0x0002u)
        #define GLV_V_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define GLV_V_INTR_MASK      (0x01u) 
#endif /* (GLV_V__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define GLV_V_PS                     (* (reg8 *) GLV_V__PS)
/* Data Register */
#define GLV_V_DR                     (* (reg8 *) GLV_V__DR)
/* Port Number */
#define GLV_V_PRT_NUM                (* (reg8 *) GLV_V__PRT) 
/* Connect to Analog Globals */                                                  
#define GLV_V_AG                     (* (reg8 *) GLV_V__AG)                       
/* Analog MUX bux enable */
#define GLV_V_AMUX                   (* (reg8 *) GLV_V__AMUX) 
/* Bidirectional Enable */                                                        
#define GLV_V_BIE                    (* (reg8 *) GLV_V__BIE)
/* Bit-mask for Aliased Register Access */
#define GLV_V_BIT_MASK               (* (reg8 *) GLV_V__BIT_MASK)
/* Bypass Enable */
#define GLV_V_BYP                    (* (reg8 *) GLV_V__BYP)
/* Port wide control signals */                                                   
#define GLV_V_CTL                    (* (reg8 *) GLV_V__CTL)
/* Drive Modes */
#define GLV_V_DM0                    (* (reg8 *) GLV_V__DM0) 
#define GLV_V_DM1                    (* (reg8 *) GLV_V__DM1)
#define GLV_V_DM2                    (* (reg8 *) GLV_V__DM2) 
/* Input Buffer Disable Override */
#define GLV_V_INP_DIS                (* (reg8 *) GLV_V__INP_DIS)
/* LCD Common or Segment Drive */
#define GLV_V_LCD_COM_SEG            (* (reg8 *) GLV_V__LCD_COM_SEG)
/* Enable Segment LCD */
#define GLV_V_LCD_EN                 (* (reg8 *) GLV_V__LCD_EN)
/* Slew Rate Control */
#define GLV_V_SLW                    (* (reg8 *) GLV_V__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define GLV_V_PRTDSI__CAPS_SEL       (* (reg8 *) GLV_V__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define GLV_V_PRTDSI__DBL_SYNC_IN    (* (reg8 *) GLV_V__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define GLV_V_PRTDSI__OE_SEL0        (* (reg8 *) GLV_V__PRTDSI__OE_SEL0) 
#define GLV_V_PRTDSI__OE_SEL1        (* (reg8 *) GLV_V__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define GLV_V_PRTDSI__OUT_SEL0       (* (reg8 *) GLV_V__PRTDSI__OUT_SEL0) 
#define GLV_V_PRTDSI__OUT_SEL1       (* (reg8 *) GLV_V__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define GLV_V_PRTDSI__SYNC_OUT       (* (reg8 *) GLV_V__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(GLV_V__SIO_CFG)
    #define GLV_V_SIO_HYST_EN        (* (reg8 *) GLV_V__SIO_HYST_EN)
    #define GLV_V_SIO_REG_HIFREQ     (* (reg8 *) GLV_V__SIO_REG_HIFREQ)
    #define GLV_V_SIO_CFG            (* (reg8 *) GLV_V__SIO_CFG)
    #define GLV_V_SIO_DIFF           (* (reg8 *) GLV_V__SIO_DIFF)
#endif /* (GLV_V__SIO_CFG) */

/* Interrupt Registers */
#if defined(GLV_V__INTSTAT)
    #define GLV_V_INTSTAT            (* (reg8 *) GLV_V__INTSTAT)
    #define GLV_V_SNAP               (* (reg8 *) GLV_V__SNAP)
    
	#define GLV_V_0_INTTYPE_REG 		(* (reg8 *) GLV_V__0__INTTYPE)
#endif /* (GLV_V__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_GLV_V_H */


/* [] END OF FILE */
