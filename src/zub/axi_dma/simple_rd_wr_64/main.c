
#include <stdio.h>
#include "xil_printf.h"
#include "xil_types.h"
#include <xil_io.h>
#include <unistd.h>
#include "xuartps.h"

#include "helpFunctions.h"
#include "axiDmaFunctions.h"

#define BD_REG32_ADDR   PL_REG32_ADDR

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
