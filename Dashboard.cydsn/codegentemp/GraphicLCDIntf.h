/*******************************************************************************
* File Name: GraphicLCDIntf.h
* Version 1.80
*
* Description:
*  This file provides the constants and parameter values and API definition for the 
*  GraphicLCDIntf Component
*
********************************************************************************
* Copyright 2008-2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_GraphicLCDIntf_GraphicLCDIntf_H)
#define CY_GraphicLCDIntf_GraphicLCDIntf_H

#include "cyfitter.h"

#if (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC6)
    #include "syslib/cy_syslib.h"
#elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3 || \
       CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4 || \
       CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5)
    #include "cytypes.h"
#endif

/*************************************** 
*   Conditional Compilation Parameters
***************************************/

#define GraphicLCDIntf_BUS_WIDTH (8u)

/***************************************
*        Function Prototypes            
***************************************/

void GraphicLCDIntf_Start(void) ;
void GraphicLCDIntf_Stop(void) ;
void GraphicLCDIntf_Write8(uint8 d_c, uint8 wrData) ;
void GraphicLCDIntf_WriteM8(uint8 d_c, uint8 wrData[], uint16 num) ;
uint8 GraphicLCDIntf_Read8(uint8 d_c) ;
void GraphicLCDIntf_ReadM8(uint8 d_c, uint8 rdData[], uint16 num) ;  
                                                
void GraphicLCDIntf_Write8_A0(uint8 wrData) ;
void GraphicLCDIntf_Write8_A1(uint8 wrData) ;
void GraphicLCDIntf_WriteM8_A0(uint8 wrData[], int num) ;
void GraphicLCDIntf_WriteM8_A1(uint8 wrData[], int num) ;
uint8 GraphicLCDIntf_Read8_A1(void) ;
void GraphicLCDIntf_ReadM8_A1(uint8 rdData[], int num) ;

void GraphicLCDIntf_Sleep(void) ;
void GraphicLCDIntf_Wakeup(void) ;
void GraphicLCDIntf_SaveConfig(void) ;   
void GraphicLCDIntf_RestoreConfig(void) ;
void GraphicLCDIntf_Init(void) ;
void GraphicLCDIntf_Enable(void) ;

#define CYGRAPHICS_START()                  GraphicLCDIntf_Start()
#define CYGRAPHICS_STOP()                   GraphicLCDIntf_Stop()
#define CYGRAPHICS_WRITE(addr, data)        GraphicLCDIntf_Write8(addr, data)
#define CYGRAPHICS_READ(addr)               GraphicLCDIntf_Read8(addr)
#define CYGRAPHICS_WRITEM(addr, data, num)  GraphicLCDIntf_WriteM8(addr, data, num)
#define CYGRAPHICS_READM(addr, data, num)   GraphicLCDIntf_ReadM8(addr, data, num)


/***************************************
*    Variables with External Linkage
***************************************/

extern uint8 GraphicLCDIntf_initVar;


/***************************************
*           API Constants               
***************************************/

#define GraphicLCDIntf_CMD_QUEUE_FULL           (0x01u)
#define GraphicLCDIntf_DATA_VALID               (0x02u)

#define GraphicLCDIntf_READ_COMMAND             (0x02u)

#define GraphicLCDIntf_DATA_ADDRESS             (0x01u)
#define GraphicLCDIntf_COMMAND_ADDRESS          (0x00u)


/***************************************
*    Enumerated Types and Parameters    
***************************************/


/***************************************
*    Initial Parameter Constants        
***************************************/

/* Set 2 smaller than a period because di must be sampled one cycle before
*  the end of the read Low intervals.
*/ 
#define GraphicLCDIntf_READ_LOW_PULSE           (10u - 2u)   

/* Set 1 smaller than a period */ 
#define GraphicLCDIntf_READ_HIGH_PULSE          (10u - 1u)


/***************************************
*             Registers                 
***************************************/

#define GraphicLCDIntf_STATUS_REG                     (* (reg8 *)  GraphicLCDIntf_StsReg__STATUS_REG)
#define GraphicLCDIntf_STATUS_PTR                     (  (reg8 *)  GraphicLCDIntf_StsReg__STATUS_REG)

#define GraphicLCDIntf_DIN_LSB_DATA_REG               (* (reg8 *)  GraphicLCDIntf_LsbReg__STATUS_REG)
#define GraphicLCDIntf_DIN_LSB_DATA_PTR               (  (reg8 *)  GraphicLCDIntf_LsbReg__STATUS_REG)

#if (GraphicLCDIntf_BUS_WIDTH == 8u)   /* 8-bit interface */
    #define GraphicLCDIntf_CMD_FIFO_REG               (* (reg8 *)  GraphicLCDIntf_GraphLcd8_Lsb__F0_REG)
    #define GraphicLCDIntf_CMD_FIFO_PTR               (  (reg8 *)  GraphicLCDIntf_GraphLcd8_Lsb__F0_REG)
    #define GraphicLCDIntf_DATA_FIFO_REG              (* (reg8 *)  GraphicLCDIntf_GraphLcd8_Lsb__F1_REG)
    #define GraphicLCDIntf_DATA_FIFO_PTR              (  (reg8 *)  GraphicLCDIntf_GraphLcd8_Lsb__F1_REG)    
    #define GraphicLCDIntf_READ_LO_PULSE_REG          (* (reg8 *)  GraphicLCDIntf_GraphLcd8_Lsb__D0_REG)
    #define GraphicLCDIntf_READ_LO_PULSE_PTR          (  (reg8 *)  GraphicLCDIntf_GraphLcd8_Lsb__D0_REG)
    #define GraphicLCDIntf_READ_HI_PULSE_REG          (* (reg8 *)  GraphicLCDIntf_GraphLcd8_Lsb__D1_REG)
    #define GraphicLCDIntf_READ_HI_PULSE_PTR          (  (reg8 *)  GraphicLCDIntf_GraphLcd8_Lsb__D1_REG)
#else   /* 16-bit interface */   
    #define GraphicLCDIntf_CMD_FIFO_REG               (* (reg8 *)  GraphicLCDIntf_GraphLcd16_Lsb__F0_REG)
    #define GraphicLCDIntf_CMD_FIFO_PTR               (  (reg8 *)  GraphicLCDIntf_GraphLcd16_Lsb__F0_REG)
	
    #if (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4)
        #define GraphicLCDIntf_DATA_FIFO_PTR              (  (reg16 *) GraphicLCDIntf_GraphLcd16_Lsb__16BIT_F1_REG)
    #else
        #define GraphicLCDIntf_DATA_FIFO_PTR              (  (reg16 *) GraphicLCDIntf_GraphLcd16_Lsb__F1_REG)
    #endif
    #define GraphicLCDIntf_DIN_MSB_DATA_REG           (* (reg8 *)  GraphicLCDIntf_GraphLcd16_MsbReg__STATUS_REG)
    #define GraphicLCDIntf_DIN_MSB_DATA_PTR           (  (reg8 *)  GraphicLCDIntf_GraphLcd16_MsbReg__STATUS_REG)
    #define GraphicLCDIntf_READ_LO_PULSE_REG          (* (reg8 *)  GraphicLCDIntf_GraphLcd16_Lsb__D0_REG)
    #define GraphicLCDIntf_READ_LO_PULSE_PTR          (  (reg8 *)  GraphicLCDIntf_GraphLcd16_Lsb__D0_REG)
    #define GraphicLCDIntf_READ_HI_PULSE_REG          (* (reg8 *)  GraphicLCDIntf_GraphLcd16_Lsb__D1_REG)
    #define GraphicLCDIntf_READ_HI_PULSE_PTR          (  (reg8 *)  GraphicLCDIntf_GraphLcd16_Lsb__D1_REG)        
#endif /* GraphicLCDIntf_BUS_WIDTH == 8u */

#endif /* CY_GraphicLCDIntf_GraphicLCDIntf_H */


/* [] END OF FILE */
