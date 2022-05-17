/*******************************************************************************
* File Name: TFT.h
* Version 1.70
*
* Description:
*  This file provides constants and parameter values and API definition for the 
*  GraphicLCDIntf Component
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_GraphicLCDIntf_TFT_H)
#define CY_GraphicLCDIntf_TFT_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#ifndef CY_PSOC5LP
    #error Component GraphicLCDIntf_v1_70 requires cy_boot v3.0 or later
#endif /* CY_PSOCLP */


/*************************************** 
*   Conditional Compilation Parameters
***************************************/

#define TFT_BUS_WIDTH (8u)


/***************************************
*        Structure Definitions
***************************************/

#if (CY_UDB_V0)
    /* Sleep Mode API Support */
    typedef struct _TFT_backupStruct
    {
        uint8 lowPulseWidth;
        uint8 highPulseWidth;
    } TFT_BACKUP_STRUCT;
#endif /* (CY_UDB_V0) */


/***************************************
*        Function Prototypes            
***************************************/

void TFT_Start(void)           ;
void TFT_Stop(void)            ;
void TFT_Write8(uint8 d_c, uint8 wrData) \
                                            ;
void TFT_WriteM8(uint8 d_c, uint8* wrData, uint16 num) \
                                            ;
uint8 TFT_Read8(uint8 d_c) \
                                            ;

void TFT_Sleep(void)           ;
void TFT_Wakeup(void)          ;
void TFT_SaveConfig(void)      ;   
void TFT_RestoreConfig(void)   ;
void TFT_Init(void)            ;
void TFT_Enable(void)          ;

#define CYGRAPHICS_START()                  TFT_Start()
#define CYGRAPHICS_STOP()                   TFT_Stop()
#define CYGRAPHICS_WRITE(addr, data)        TFT_Write8(addr, data)
#define CYGRAPHICS_READ(addr)               TFT_Read8(addr)
#define CYGRAPHICS_WRITEM(addr, data, num)  TFT_WriteM8(addr, data, num)


/***************************************
*           API Constants               
***************************************/

#define TFT_CMD_QUEUE_FULL           (0x01u)
#define TFT_DATA_VALID               (0x02u)

#define TFT_READ_COMMAND             (0x02u)

#define TFT_DATA_ADDRESS             (0x01u)
#define TFT_COMMAND_ADDRESS          (0x00u)


/***************************************
*    Enumerated Types and Parameters    
***************************************/


/***************************************
*    Initial Parameter Constants        
***************************************/

/* Set 2 less than period because di must be sampled one cycle before
*  the end of Read Low intervals.
*/ 
#define TFT_READ_LOW_PULSE           (3u - 2u)   

/* Set one less than period */ 
#define TFT_READ_HIGH_PULSE          (11u - 1u)


/***************************************
*             Registers                 
***************************************/

#define TFT_STATUS_REG                     (* (reg8 *)  TFT_StsReg__STATUS_REG)
#define TFT_STATUS_PTR                     (  (reg8 *)  TFT_StsReg__STATUS_REG)

#define TFT_DIN_LSB_DATA_REG               (* (reg8 *)  TFT_LsbReg__STATUS_REG)
#define TFT_DIN_LSB_DATA_PTR               (  (reg8 *)  TFT_LsbReg__STATUS_REG)

#if (TFT_BUS_WIDTH == 8u)   /* 8-bit interface */
    #define TFT_CMD_FIFO_REG               (* (reg8 *)  TFT_GraphLcd8_Lsb__F0_REG)
    #define TFT_CMD_FIFO_PTR               (  (reg8 *)  TFT_GraphLcd8_Lsb__F0_REG)
    #define TFT_DATA_FIFO_REG              (* (reg8 *)  TFT_GraphLcd8_Lsb__F1_REG)
    #define TFT_DATA_FIFO_PTR              (  (reg8 *)  TFT_GraphLcd8_Lsb__F1_REG)    
    #define TFT_READ_LO_PULSE_REG          (* (reg8 *)  TFT_GraphLcd8_Lsb__D0_REG)
    #define TFT_READ_LO_PULSE_PTR          (  (reg8 *)  TFT_GraphLcd8_Lsb__D0_REG)
    #define TFT_READ_HI_PULSE_REG          (* (reg8 *)  TFT_GraphLcd8_Lsb__D1_REG)
    #define TFT_READ_HI_PULSE_PTR          (  (reg8 *)  TFT_GraphLcd8_Lsb__D1_REG)
#else   /* 16-bit interface */
    #define TFT_CMD_FIFO_REG               (* (reg8 *)  TFT_GraphLcd16_Lsb__F0_REG)
    #define TFT_CMD_FIFO_PTR               (  (reg8 *)  TFT_GraphLcd16_Lsb__F0_REG)
    #define TFT_DATA_FIFO_PTR              (  (reg16 *) TFT_GraphLcd16_Lsb__F1_REG)     
    #define TFT_DIN_MSB_DATA_REG           (* (reg8 *)  TFT_GraphLcd16_MsbReg__STATUS_REG)
    #define TFT_DIN_MSB_DATA_PTR           (  (reg8 *)  TFT_GraphLcd16_MsbReg__STATUS_REG)
    #define TFT_READ_LO_PULSE_REG          (* (reg8 *)  TFT_GraphLcd16_Lsb__D0_REG)
    #define TFT_READ_LO_PULSE_PTR          (  (reg8 *)  TFT_GraphLcd16_Lsb__D0_REG)
    #define TFT_READ_HI_PULSE_REG          (* (reg8 *)  TFT_GraphLcd16_Lsb__D1_REG)
    #define TFT_READ_HI_PULSE_PTR          (  (reg8 *)  TFT_GraphLcd16_Lsb__D1_REG) 
#endif /* TFT_BUS_WIDTH == 8u */

#endif /* CY_GraphicLCDIntf_TFT_H */


/* [] END OF FILE */
