/*******************************************************************************
* File Name: GraphicLCDIntf.c  
* Version 1.80
*
* Description:
*  This file contains the setup, control and status commands for the 
*  GraphicLCDIntf component.   
*
*******************************************************************************
* Copyright 2008-2018, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "GraphicLCDIntf.h"  

uint8 GraphicLCDIntf_initVar = 0u;


/*******************************************************************************
* Function Name: GraphicLCDIntf_Enable
********************************************************************************
*
* Summary:
*  There is no need to reset or enable this component.  This component is 
*  naturally a slave that waits for the API calls to perform any 
*  function and after a request is made that request will always complete in 
*  finite time as long as a clock is provided. The only thing provided
*  by this API is setting the Command FIFO to drive at least the half-empty status
*  from the FIFO.
*
*******************************************************************************/
void GraphicLCDIntf_Enable(void) 
{   
    /* Nothing to enable */
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default GraphicLCDIntf configuration provided with the customizer.
* 
*******************************************************************************/
void GraphicLCDIntf_Init(void) 
{
    /* Set low and high pulse widths for a read transaction */
    GraphicLCDIntf_READ_LO_PULSE_REG = GraphicLCDIntf_READ_LOW_PULSE;
    GraphicLCDIntf_READ_HI_PULSE_REG = GraphicLCDIntf_READ_HIGH_PULSE;
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_Start
********************************************************************************
*
* Summary:
*  Starts the GraphicLCDIntf interface.
*
* Global Variables:
*  GraphicLCDIntf_initVar - Used to check the initial configuration, modified on 
*  the first function call.
*
*******************************************************************************/
void GraphicLCDIntf_Start(void) 
{
    if (GraphicLCDIntf_initVar == 0u)
    {
        GraphicLCDIntf_Init();
        GraphicLCDIntf_initVar = 1u;
    }
      
    GraphicLCDIntf_Enable();
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_Stop
********************************************************************************
*
* Summary:
*  Disables the GraphicLCDIntf interface.
*
*******************************************************************************/
void GraphicLCDIntf_Stop(void) 
{
    /* Nothing to stop */
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_Write8
********************************************************************************
*
* Summary:
*  Initiates a write transaction on the 8-bit parallel
*  interface.
*  The write is a posted write, so this function will return before the write 
*  has actually completed on the interface. If the command queue is full, this 
*  function will not return until space is available to queue this write request.
*
* Parameters:
*  d_c:  Data(1) or Command(0) indication. Passed to the d_c pin.
*
*  wrData: Data sent to the do_msb[7:0] (most significant byte) 
*          and do_lsb[7:0] (least significant byte) pins. do_msb[7:0]
*          presents only for the 16-bit interface.
*
*******************************************************************************/
void GraphicLCDIntf_Write8(uint8 d_c, uint8 wrData) 
{
    while((GraphicLCDIntf_STATUS_REG & GraphicLCDIntf_CMD_QUEUE_FULL) != 0u)
    {
        /* The command queue is full */
    }   
    GraphicLCDIntf_CMD_FIFO_REG = d_c;
    #if (GraphicLCDIntf_BUS_WIDTH == 16u)
        CY_SET_REG16(GraphicLCDIntf_DATA_FIFO_PTR, wrData);
    #else /* 8-bit interface */
        GraphicLCDIntf_DATA_FIFO_REG = wrData;
    #endif /* GraphicLCDIntf_BUS_WIDTH == 16u */
}
	

/*******************************************************************************
* Function Name: GraphicLCDIntf_WriteM8
********************************************************************************
*
* Summary:
*  Initiates multiple write transactions on the 8-bit 
*  parallel interface. The write is a posted write, so this function will return
*  before the write has actually completed on the interface. If the command 
*  queue is full, this function will not return until space is available to queue
*  this write request.
*
* Parameters:
*  d_c:    The Data(1) or Command(0) indication. Passed to the d_c pin.
*
*  wrData: The pointer to the the first element of a data buffer sent to the do_msb[7:0]
*          (most significant byte) and do_lsb[7:0] (least significant byte) pins. 
*          do_msb[7:0] presents only for the 16-bit interface.
*
*  num:    The number of words/bytes to write.
*
*******************************************************************************/
void GraphicLCDIntf_WriteM8(uint8 d_c, uint8 wrData[], uint16 num) 
{
    uint32 i;
    
    for(i = 0u; i < num; i++)
    {
        while((GraphicLCDIntf_STATUS_REG & GraphicLCDIntf_CMD_QUEUE_FULL) != 0u)
        {
            /* The command queue is full */
        }   
        GraphicLCDIntf_CMD_FIFO_REG = d_c;  
 
        #if (GraphicLCDIntf_BUS_WIDTH == 16u)
            CY_SET_REG16(GraphicLCDIntf_DATA_FIFO_PTR, wrData[i]);
        #else /* 8-bit interface */
            GraphicLCDIntf_DATA_FIFO_REG = wrData[i];
        #endif /* GraphicLCDIntf_BUS_WIDTH == 16u */
    }
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_Read8
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
*  The 16-bit read value from the di_msb[7:0] (most significant byte) and 
*  di_lsb[7:0] (least significant byte) pins for the 16-bit interface or the 
*  8-bit read value from the di_lsb[7:0] for 8-bit interface.
*
*******************************************************************************/
uint8 GraphicLCDIntf_Read8(uint8 d_c) 
{
    while((GraphicLCDIntf_STATUS_REG & GraphicLCDIntf_CMD_QUEUE_FULL) != 0u)
    {
        /* The command queue is full */
    }   
    GraphicLCDIntf_CMD_FIFO_REG = d_c | GraphicLCDIntf_READ_COMMAND;
    
    while((GraphicLCDIntf_STATUS_REG & GraphicLCDIntf_DATA_VALID) != GraphicLCDIntf_DATA_VALID)
    {
        /* Wait until input data is valid */
    }
    
    #if (GraphicLCDIntf_BUS_WIDTH == 8u)  /* 8-bit interface */
        return (GraphicLCDIntf_DIN_LSB_DATA_REG);
    #else   /* 16-bit interface */
        return (uint16)((uint16)GraphicLCDIntf_DIN_MSB_DATA_REG << 8u) | GraphicLCDIntf_DIN_LSB_DATA_REG; 
    #endif /* GraphicLCDIntf_BUS_WIDTH == 8u */
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_ReadM8
********************************************************************************
*
* Summary:
*  Initiates multiple read transactions on the 8-bit 
*  parallel interface. The read will execute after all currently posted writes 
*  have completed. This function will wait until the read completes and then 
*  returns the read value.
*
* Parameters:
*  d_c:    The Data(1) or Command(0) indication. Passed to the d_c pin.
*
*  rdData: The pointer to the the first element of a data buffer read from the do_msb[7:0]
*          (most significant byte) and do_lsb[7:0] (least significant byte) pins. 
*          do_msb[7:0] presents only for the 16-bit interface.
*
*  num:    The number of words/bytes to read.
*
*******************************************************************************/
void GraphicLCDIntf_ReadM8(uint8 d_c, uint8 rdData[], uint16 num) 
{
    uint32 i;
    
    for(i = 0u; i < num; i++)
    {
        rdData[i] = GraphicLCDIntf_Read8(d_c);
    }
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_Write8_A0
********************************************************************************
*
* Summary:
*  Initiates a write transaction on the 8-bit parallel
*  interface with the d_c pin set to 0.
*  The write is a posted write, so this function will return before the write 
*  has actually completed on the interface. If the command queue is full, this 
*  function will not return until space is availale to queue this write request.
*
* Parameters:
*  wrData: Data sent on the do_msb[7:0] (most significant byte) 
*         and do_lsb[7:0] (least significant byte) pins. do_msb[7:0]
*         presents only for 16-bit interface.
*
*******************************************************************************/
void GraphicLCDIntf_Write8_A0(uint8 wrData) 
{
    while((GraphicLCDIntf_STATUS_REG & GraphicLCDIntf_CMD_QUEUE_FULL) != 0u)
    {
        /* Command queue is full */
    }   
    GraphicLCDIntf_CMD_FIFO_REG = 0u;
    #if (GraphicLCDIntf_BUS_WIDTH == 16u)
        CY_SET_REG16(GraphicLCDIntf_DATA_FIFO_PTR, wrData);
    #else /* 8-bit interface */
        GraphicLCDIntf_DATA_FIFO_REG = wrData;
    #endif /* GraphicLCDIntf_BUS_WIDTH == 16u */
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_Write8_A1
********************************************************************************
*
* Summary:
*  Initiates a write transaction on the 8-bit 
*  parallel interface with the d_c pin set to 1. The write is a posted write, 
*  so this function will return before the write has actually completed on the 
*  interface. If the command queue is full, this function will not return until
*  space is availale to queue this write request.
*
* Parameters:
*  wrData: Data sent on the do_msb[7:0] (most significant byte) 
*          and do_lsb[7:0] (least significant byte) pins. do_msb[7:0]
*          presents only for 16-bit interface.
*
*******************************************************************************/
void GraphicLCDIntf_Write8_A1(uint8 wrData) 
{
    while((GraphicLCDIntf_STATUS_REG & GraphicLCDIntf_CMD_QUEUE_FULL) != 0u)
    {
        /* Command queue is full */
    }   
    GraphicLCDIntf_CMD_FIFO_REG = 1u;
    #if (GraphicLCDIntf_BUS_WIDTH == 16u)
        CY_SET_REG16(GraphicLCDIntf_DATA_FIFO_PTR, wrData);
    #else /* 8-bit interface */
        GraphicLCDIntf_DATA_FIFO_REG = wrData;
    #endif /* GraphicLCDIntf_BUS_WIDTH == 16u */
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_WriteM8_A0
********************************************************************************
*
* Summary:
*  Initiates multiple write transactions on the 8-bit 
*  parallel interface with the d_c pin set to 0. The write is a posted write, 
*  so this function will return before the write has actually completed on the 
*  interface. If the command queue is full, this function will not return until
*  space is availale to queue this write request.
*
* Parameters:
*  wrData: Pointer to the the first element of a data buffer sent on the 
*          do_msb[7:0] (most significant byte) and do_lsb[7:0] (least significant
*          byte) pins. do_msb[7:0] presents only for 16-bit interface.
*
*  num:  Number of words/bytes to write.
*
*******************************************************************************/
void GraphicLCDIntf_WriteM8_A0(uint8 wrData[], int num) 
{
    GraphicLCDIntf_WriteM8(0u, wrData, (uint16)num);
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_WriteM8_A1
********************************************************************************
*
* Summary:
*  Initiates multiple data write transactions on the 8-bit 
*  parallel interface with the d_c pin set to 1. The write is a posted write, 
*  so this function will return before the write has actually completed on the 
*  interface. If the command queue is full, this function will not return until
*  space is availale to queue this write request.
*
* Parameters:
*  wrData: Pointer to the the first element of a data buffer sent on the 
*          do_msb[7:0] (most significant byte) and do_lsb[7:0] (least significant
*          byte) pins. do_msb[7:0] presents only for 16-bit interface.
*
*  num:  Number of words/bytes to write.
*
*******************************************************************************/
void GraphicLCDIntf_WriteM8_A1(uint8 wrData[], int num) 
{
    GraphicLCDIntf_WriteM8(1u, wrData, (uint16)num);
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_Read8_A1
********************************************************************************
*
* Summary:
*  Initiates data read transaction on the 8-bit 
*  parallel interface with the d_c pin set to 1. The read will execute after 
*  all currently posted writes have completed. This function will wait until 
*  the read completes and then returns the read value.
*
* Return:
*  The 16-bit read value from the di_msb[7:0] (most significant byte) and 
*  di_lsb[7:0] (least significant byte) pins for the 16-bit interface or the 
*  8-bit read value from the di_lsb[7:0] for 8-bit interface.
*
*******************************************************************************/
uint8 GraphicLCDIntf_Read8_A1(void) 
{
    return GraphicLCDIntf_Read8(1u);
}


/*******************************************************************************
* Function Name: GraphicLCDIntf_ReadM8_A1
********************************************************************************
*
* Summary:
*  Initiates multiple data read transactions on the 8-bit 
*  parallel interface with the d_c pin set to 1. The read will execute after all 
*  currently posted writes have completed. This function will wait until the 
*  read completes and then returns the read value.
*
* Parameters:
*  rdData: Pointer to the the first element of a data buffer read from the do_msb[7:0]
*        (most significant byte) and do_lsb[7:0] (least significant byte) pins. 
*        do_msb[7:0] presents only for 16-bit interface.
*
*  num:  Number of words/bytes to read.
*
*******************************************************************************/
void GraphicLCDIntf_ReadM8_A1(uint8 rdData[], int num) 
{
    GraphicLCDIntf_ReadM8(1u, rdData, (uint16)num);
}


/* [] END OF FILE */
