

#include <stdio.h>
#include "xil_printf.h"
#include "xil_types.h"
#include <xil_io.h>
#include "xparameters.h"
#include "helpFunctions.h"
#include <unistd.h>
#include "xuartps.h"

#include "platform.h"
#include "xaxidma.h"
#include "xdebug.h"
//#include "sleep.h"

#define BD_REG32_ADDR   PL_REG32_ADDR

#define MEM_BASE_ADDR   0x01000000
#define TX_BUFFER_BASE  (MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE  (MEM_BASE_ADDR + 0x00300000)
#define RX_BUFFER_HIGH  (MEM_BASE_ADDR + 0x004FFFFF)


//#define PKT_2PL_LEN		  512*8
//#define PKT_FROM_PL_LEN 512*8
//static u64 TX_BUFFER_BASE [PKT_2PL_LEN];  
//static u64 RX_BUFFER_BASE [PKT_FROM_PL_LEN];


#define TEST_START_VALUE	0xA5CADEAD00010000
#define POLL_TIMEOUT_COUNTER    1000000U


XAxiDma AxiDma;

int init_dma_adg(void);
int dma2PL(u32 len);
int dmaFromPL(u32 len);
void printRXbuf(u32 len64);
void testDMA_READ(void);


int main()
{
    init_platform();
    int status;

    xil_printf("\n\rtesting adg0000\n\r");
    
    //check0();    
    getVersions();
    //versionCtrl();
    ledCfg();
    
    status = init_dma_adg();
  	if (status != XST_SUCCESS) {
  	  xil_printf("DMA init failed: %d\r\n", status);
  	}


    unsigned int val, led=0;
    //xil_printf("  Scripts Git Hash: %0x%0x\n\r",gitMBD,gitLBD);
    //val = Xil_In32(BD_REG32_2_ADDR + 0x4); xil_printf("gh1=%d\r\n",val);
    //Xil_Out32(BD_REG32_2_ADDR + 0x18, 0x7);

    xil_printf("Running...\r\n");
    s8 Ch;
    while (1) {
      //Ch = inbyte();
      //if (Ch == '\r') {
      //    outbyte('\n');
      //}
      if (XUartPs_IsReceiveData(STDIN_BASEADDRESS)) {
        Ch = XUartPs_ReadReg(STDIN_BASEADDRESS, XUARTPS_FIFO_OFFSET);
        outbyte(Ch);
        xil_printf("\r\n");

        if (Ch == 'p') {
          xil_printf("\r\n POWER OFF\r\n");
          usleep(10000);
          powerOff();
        } else if (Ch == 'b') {break;
        } else if (Ch == 'a') {   xil_printf("NULL\r\n");
        } else if (Ch == 'c') {   //val = Xil_In32(BD_REG32_2_ADDR + 0x0); xil_printf("gh0=%d\r\n",val);
        } else if (Ch == 'd') {   
        } else if (Ch == 'e') {   
        } else if (Ch == 'f') {   dma2PL(64*8); // bytes
        } else if (Ch == 'g') {   dmaFromPL(256*8); //bytes
        } else if (Ch == 'h') {   printRXbuf(256); // 64-bit words
        } else if (Ch == 'i') {   dma2PL(256*8); // bytes: 256 64-bit words
        } else if (Ch == 'j') {   
        } else if (Ch == 'k') {   Xil_Out32(BD_REG32_ADDR + 0x7C, 0x0);
        } else if (Ch == 'l') {   Xil_Out32(BD_REG32_ADDR + 0x7C, 0x1);
        } else if (Ch == 'm') {   
        } else if (Ch == 'n') {   testDMA_READ();
        } else if (Ch == 'o') {   
        } else if (Ch == 'q') {   
        } else if (Ch == 'r') {   
        } else if (Ch == 's') {   getVersions();
        } else if (Ch == 't') {   getTstamps();
        } else if (Ch == 'u') {   
        } else if (Ch == 'v') {   versionCtrl();

        } else if (Ch == '0') {   ledCfg();
        } else if (Ch == '1') {   ledON();
        } else if (Ch == '2') {   ledOff();
        } else if (Ch == '3') {
        } else if (Ch == '4') {
        } else if (Ch == '5') {
        } else if (Ch == '6') {
        } else if (Ch == '7') {
        } else if (Ch == '8') {
        } else if (Ch == '9') {
        }
      } // if (XUartPs_IsReceiveData(STDIN_BASEADDRESS))
      
    } // while
    xil_printf("\n\r----------------------------------------\n\r");
    xil_printf("** END **\n\r");
    xil_printf("----------------------------------------\n\r\n\r");

    cleanup_platform();
    
    return 0;
}
/*************************************************************************************************/
//
/*************************************************************************************************/
void testDMA_READ(void)
{
    Xil_Out32(BD_REG32_ADDR + 0x7C, 0x0);
    Xil_Out32(BD_REG32_ADDR + 0x7C, 0x1);
    dmaFromPL(256*8); //bytes
    printRXbuf(256); // 64-bit words
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

/*************************************************************************************************/
// WRITE to PL
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
// READ from PL
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
