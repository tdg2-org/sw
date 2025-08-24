
#include "axiDmaFunctions.h"

XAxiDma AxiDma;
XAxiDma AxiDma_PL_1k;

/*************************************************************************************************/
//
/*************************************************************************************************/
void testDMA_READ(void)
{
    reg_write_field(BD_REG32_ADDR, 0x7C, 24, 8, 0xFF);  // 255
	Xil_Out32(BD_REG32_ADDR + 0x7C, 0x0);
    Xil_Out32(BD_REG32_ADDR + 0x7C, 0x1);
    dmaFromPL(256*8); //bytes
    printRXbuf(256); // 64-bit words
}


void PL_2_BRAM_S2MM(u32 lenBytes) //S2MM
{
	reg_clear_bits(BD_REG32_ADDR, 0x78, 0,0);
	reg_set_bits(BD_REG32_ADDR, 0x78, 0);
    dma2_BRAM(lenBytes); //bytes
}

/*************************************************************************************************/
//
/*************************************************************************************************/

int init_dma_adg(void) 
{
  	XAxiDma_Config *CfgPtr;
  	int Status;

	/* Initialize the XAxiDma device.
	 */
	CfgPtr = XAxiDma_LookupConfig(XPAR_XAXIDMA_0_BASEADDR);
	if (!CfgPtr) {
		xil_printf("No config found for %d\r\n", XPAR_XAXIDMA_0_BASEADDR);
	}

  	Status = XAxiDma_CfgInitialize(&AxiDma, CfgPtr);
  	if (Status != XST_SUCCESS) {
  	  xil_printf("DMA init failed: %d\r\n", Status);
  	  return Status;
  	}

  	// Disable interrupts if polling
  	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
  	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

  	return XST_SUCCESS;
}

int init_dma_PL(void) 
{
  	XAxiDma_Config *CfgPtr;
  	int Status;

	/* Initialize the XAxiDma device.
	 */
	CfgPtr = XAxiDma_LookupConfig(XPAR_XAXIDMA_1_BASEADDR);
	if (!CfgPtr) {
		xil_printf("No config found for %d\r\n", XPAR_XAXIDMA_1_BASEADDR);
	}

  	Status = XAxiDma_CfgInitialize(&AxiDma_PL_1k, CfgPtr);
  	if (Status != XST_SUCCESS) {
  	  xil_printf("DMA init failed: %d\r\n", Status);
  	  return Status;
  	}

  	// Disable interrupts if polling
  	XAxiDma_IntrDisable(&AxiDma_PL_1k, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
  	XAxiDma_IntrDisable(&AxiDma_PL_1k, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

  	return XST_SUCCESS;
}


/*************************************************************************************************/
// READ from DDR and send to PL (MM2S)
/*************************************************************************************************/
int dma2PL(u32 lenBytes) 
{
	int Status; 
  u32 Index;
	u64 *TxBufferPtr;
  u64 Value;
  int TimeOut = POLL_TIMEOUT_COUNTER;

	TxBufferPtr = (u64 *)TX_BUFFER_BASE;

  Value = TEST_START_VALUE;

  u32 words = lenBytes / sizeof(u64);
	//for (Index = 0; Index < lenBytes; Index ++) {
	for (Index = 0; Index < words; Index ++) {
		TxBufferPtr[Index] = Value;
		Value = (Value + 1) & 0xFFFFFFFFFFFFFFFF;
	}

	//Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, lenBytes);
  Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, words * sizeof(u64));

	Status = XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR) TxBufferPtr, lenBytes, XAXIDMA_DMA_TO_DEVICE);
  
  if (Status != XST_SUCCESS) {
  	return XST_FAILURE;
  }

  	/*Wait till tranfer is done or 1usec * 10^6 iterations of timeout occurs*/
  while (TimeOut) {
  	if (!(XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE))) {
  		xil_printf("2PL TO = %d",TimeOut);
      break;
  	}
  	TimeOut--;
  	usleep(1U);
  }
  xil_printf("DMA 2 PL done. %d\r\n",TimeOut);
  return XST_SUCCESS;
}

/*************************************************************************************************/
// Read from BRAM and send to PL (MM2S)
/*************************************************************************************************/
int dmaFrom_BRAM(u32 lenBytes) 
{
	int Status; 
	u64 *TxBufferPtr;
  	int TimeOut = POLL_TIMEOUT_COUNTER;

	TxBufferPtr = (u64 *)PL_BRAM;

  	//u32 words = lenBytes / sizeof(u64);
  	//Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, words * sizeof(u64));

	Status = XAxiDma_SimpleTransfer(&AxiDma_PL_1k, (UINTPTR) TxBufferPtr, lenBytes, XAXIDMA_DMA_TO_DEVICE);
  
  	if (Status != XST_SUCCESS) {
  		return XST_FAILURE;
  	}

  	/*Wait till tranfer is done or 1usec * 10^6 iterations of timeout occurs*/
  	while (TimeOut) {
  		if (!(XAxiDma_Busy(&AxiDma_PL_1k, XAXIDMA_DMA_TO_DEVICE))) {
  			xil_printf("2PL TO = %d",TimeOut);
      	break;
  		}
  		TimeOut--;
  		usleep(1U);
  	}
  	xil_printf("DMA From BRAM MM2S done. %d\r\n",TimeOut);
  	return XST_SUCCESS;
}

/*************************************************************************************************/
// Write to DDR from PL (S2MM)
/*************************************************************************************************/
int dmaFromPL(u32 lenBytes) 
{
  int Status; 
  //u64 *RxBufferPtr;
  u64 *RxBufferPtr = (u64 *)RX_BUFFER_BASE;
  int TimeOut = POLL_TIMEOUT_COUNTER;

	//RxBufferPtr = (u64 *)RX_BUFFER_BASE;

  Xil_DCacheInvalidateRange((UINTPTR)RxBufferPtr, lenBytes);

  Status = XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR) RxBufferPtr, lenBytes, XAXIDMA_DEVICE_TO_DMA);

  if (Status != XST_SUCCESS) {
  	return XST_FAILURE;
  }

  	/*Wait till tranfer is done or 1usec * 10^6 iterations of timeout occurs*/
  while (TimeOut) {
  	if (!(XAxiDma_Busy(&AxiDma, XAXIDMA_DEVICE_TO_DMA))) {
  		//xil_printf("FromPL TO = %d",TimeOut);
      break;
  	}
  	TimeOut--;
  	usleep(1U);
  }

  xil_printf("DMA From PL done. %d\r\n",TimeOut);
  return XST_SUCCESS;

}

/*************************************************************************************************/
// WRITE to BRAM from PL (S2MM)
/*************************************************************************************************/
int dma2_BRAM(u32 lenBytes) 
{
  	int Status; 
  	u64 *RxBufferPtr = (u64 *)PL_BRAM;
  	int TimeOut = POLL_TIMEOUT_COUNTER;

  	//Xil_DCacheInvalidateRange((UINTPTR)RxBufferPtr, lenBytes);

  	Status = XAxiDma_SimpleTransfer(&AxiDma_PL_1k, (UINTPTR) RxBufferPtr, lenBytes, XAXIDMA_DEVICE_TO_DMA);

  	if (Status != XST_SUCCESS) {
  		return XST_FAILURE;
  	}

  		/*Wait till tranfer is done or 1usec * 10^6 iterations of timeout occurs*/
  	while (TimeOut) {
  		if (!(XAxiDma_Busy(&AxiDma_PL_1k, XAXIDMA_DEVICE_TO_DMA))) {
  			//xil_printf("FromPL TO = %d",TimeOut);
  	    break;
  		}
  		TimeOut--;
  		usleep(1U);
  	}

  	xil_printf("DMA 2 BRAM S2MM done. %d\r\n",TimeOut);
  	return XST_SUCCESS;

}


/*************************************************************************************************/
// Print RX buffer 
/*************************************************************************************************/
void printRXbuf(u32 len64)
{
  u32 idx = 0;
  u64 *RxPacket;
  u32 rxlo, rxhi;
  RxPacket = (u64 *) RX_BUFFER_BASE;

  for (idx = 0; idx < len64; idx = idx + 0x4) {
  	rxlo = (u32)(RxPacket[idx] & 0xFFFFFFFF);
  	rxhi = (u32)(RxPacket[idx] >> 32);
  	xil_printf("%08x%08x, ",rxhi,rxlo);
  	rxlo = (u32)(RxPacket[idx+1] & 0xFFFFFFFF);
  	rxhi = (u32)(RxPacket[idx+1] >> 32);
  	xil_printf("%08x%08x, ",rxhi,rxlo);
  	rxlo = (u32)(RxPacket[idx+2] & 0xFFFFFFFF);
  	rxhi = (u32)(RxPacket[idx+2] >> 32);
  	xil_printf("%08x%08x, ",rxhi,rxlo);
  	rxlo = (u32)(RxPacket[idx+3] & 0xFFFFFFFF);
  	rxhi = (u32)(RxPacket[idx+3] >> 32);
  	xil_printf("%08x%08x\r\n",rxhi,rxlo);
  }
  return;
}

//void printRXbuf(u32 len64)
//{
//  u32 idx = 0;
//  u64 *RxPacket;
//  u32 rxlo, rxhi;
//  RxPacket = (u64 *) RX_BUFFER_BASE;
//
//  for (idx = 0; idx < len64; idx++) {
//  	rxlo = (u32)(RxPacket[idx] & 0xFFFFFFFF);
//  	rxhi = (u32)(RxPacket[idx] >> 32);
//  	xil_printf("Rx data %d = %08x%08x\r\n", idx, rxhi,rxlo);
//  }
//  return;
//}
