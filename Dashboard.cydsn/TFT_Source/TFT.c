/*******************************************************************************
* File Name: TFT.c  
* Version 1.70
*
* Description:
*  This file contains the setup, control and status commands for the 
*  GraphicLCDIntf component.  
*
* Note: 
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "TFT.h"  

uint8 TFT_initVar = 0u;


/*******************************************************************************
* Function Name: TFT_Enable
********************************************************************************
*
* Summary:
*  There is no need to reset or enable this component.  This component is 
*  naturally a slave that waits for the API calls in order to perform any 
*  function and once a request is made that request will always complete in 
*  finite time as long as a clock is provided. The only thing that is provided
*  by this API is setting the Command FIFO to drive at least half empty status
*  from the FIFO.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void TFT_Enable(void) 
{   
    /* There aren't anything to enable */
}


/*******************************************************************************
* Function Name: TFT_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default GraphicLCDIntf configuration provided with customizer
*
* Parameters:
*  None.
*
* Return:
*  None.
* 
*******************************************************************************/
void TFT_Init(void) 
{
    /* Set low and high pulse widths for read transaction */
    TFT_READ_LO_PULSE_REG = TFT_READ_LOW_PULSE;
    TFT_READ_HI_PULSE_REG = TFT_READ_HIGH_PULSE;
}


/*******************************************************************************
* Function Name: TFT_Start
********************************************************************************
*
* Summary:
*  Starts the GraphicLCDIntf interface.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  TFT_initVar - used to check initial configuration, modified on 
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFT_Start(void) 
{
    if (TFT_initVar == 0u)
    {
        TFT_Init();
        TFT_initVar = 1u;
    }
      
    TFT_Enable();
}


/*******************************************************************************
* Function Name: TFT_Stop
********************************************************************************
*
* Summary:
*  Disables the GraphicLCDIntf interface.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void TFT_Stop(void) 
{
    /* There aren't anything to stop */
}


/*******************************************************************************
* Function Name: TFT_Write8
********************************************************************************
*
* Summary:
*  Initiates a write transaction on the 8-bit parallel
*  interface.
*  The write is a posted write, so this function will return before the write 
*  has actually completed on the interface. If the command queue is full, this 
*  function will not return until space is availale to queue this write request.
*
* Parameters:
*  d_c:  Data(1) or Command(0) indication. Passed to the d_c pin.
*
*  data: Data sent on the do_msb[7:0] (most significant byte) 
*        and do_lsb[7:0] (least significant byte) pins. do_msb[7:0]
*        presents only for 16-bit interface.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFT_Write8(uint8 d_c, uint8 wrData) \
                                                 
{
    while((TFT_STATUS_REG & TFT_CMD_QUEUE_FULL) != 0u)
    {
            /* Command queue is full */
    }   
    TFT_CMD_FIFO_REG = d_c;
    #if (TFT_BUS_WIDTH == 16u)
        CY_SET_REG16(TFT_DATA_FIFO_PTR, wrData);
    #else /* 8-bit interface */
        TFT_DATA_FIFO_REG = wrData;
    #endif /* TFT_BUS_WIDTH == 16u */
}
	

/*******************************************************************************
* Function Name: TFT_WriteM8
********************************************************************************
*
* Summary:
*  Initiates multiple write transactions on the 8-bit 
*  parallel interface. The write is a posted write, so this function will return
*  before the write has actually completed on the interface. If the command 
*  queue is full, this function will not return until space is availale to queue
*  this write request.
*
* Parameters:
*  d_c:  Data(1) or Command(0) indication. Passed to the d_c pin.
*
*  data: Pointer to the the first element of a data buffer sent on the do_msb[7:0]
*        (most significant byte) and do_lsb[7:0] (least significant byte) pins. 
*        do_msb[7:0] presents only for 16-bit interface.
*
*  num:  Number of words/bytes to write.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void TFT_WriteM8(uint8 d_c, uint8* wrData, uint16 num) \
                                                
{
    for(; num; num--)
    {
        while((TFT_STATUS_REG & TFT_CMD_QUEUE_FULL) != 0u)
        {
            /* Command queue is full */
        }   
        TFT_CMD_FIFO_REG = d_c;  
 
        #if (TFT_BUS_WIDTH == 16u)
            CY_SET_REG16(TFT_DATA_FIFO_PTR, *wrData++);
        #else /* 8-bit interface */
            TFT_DATA_FIFO_REG = *wrData++;
        #endif /* TFT_BUS_WIDTH == 16u */
    }
}


/*******************************************************************************
* Function Name: TFT_Read8
********************************************************************************
*
* Summary:
*  Initiates a read transaction on the 8-bit parallel interface.
*  The read will execute after all currently posted writes have completed. This 
*  function will wait until the read completes and then returns the read value.
*
* Parameters:
*  d_c: Data(1) or Command(0) indication. Passed to the d_c pin.
*
* Return:
*  16-bit read value from the di_msb[7:0] (most significant byte) and 
*  di_lsb[7:0] (least significant byte) pins for 16-bit interface or
*  8-bit read value from the di_lsb[7:0] for 8-bit interface.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 TFT_Read8(uint8 d_c) \
                                                  
{
    while((TFT_STATUS_REG & TFT_CMD_QUEUE_FULL) != 0u)
    {
        /* Command queue is full */
    }   
    TFT_CMD_FIFO_REG = d_c | TFT_READ_COMMAND;
    
    while((TFT_STATUS_REG & TFT_DATA_VALID) != TFT_DATA_VALID)
    {
        /* wait until input data are valid */    
    }
    
    #if (TFT_BUS_WIDTH == 8u)  /* 8-bit interface */
        return (TFT_DIN_LSB_DATA_REG);
    #else   /* 16-bit interface */
        return ((((uint16) TFT_DIN_MSB_DATA_REG) << 8u) | TFT_DIN_LSB_DATA_REG); 
    #endif /* TFT_BUS_WIDTH == 8u */
}


/* [] END OF FILE */
