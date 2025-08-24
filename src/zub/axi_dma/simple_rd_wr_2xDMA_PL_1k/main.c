
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

    status = init_dma_PL();
  	if (status != XST_SUCCESS) {
  	  xil_printf("BRAM DMA init failed: %d\r\n", status);
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
        } else if (Ch == 'c') {   //val = Xil_In32(BD_REG32_ADDR + 0x78); xil_printf("val=%x\r\n",val);
        } else if (Ch == 'd') {   testDMA_READ();
        } else if (Ch == 'e') {   dma2PL(64*8); // bytes
        } else if (Ch == 'f') {   dmaFromPL(256*8); //bytes
        } else if (Ch == 'g') {   printRXbuf(256); // 64-bit words
        } else if (Ch == 'h') {   dma2PL(256*8); // bytes: 256 64-bit words
        } else if (Ch == 'i') {   
        } else if (Ch == 'j') {   reg_write_field(BD_REG32_ADDR, 0x78, 24, 8, 0x1F);  // 31
        } else if (Ch == 'k') {   val = Xil_In32(BD_REG32_ADDR + 0x78); xil_printf("val=%x\r\n",val);
        } else if (Ch == 'l') {   PL_2_BRAM_S2MM(32*128);
        } else if (Ch == 'm') {   dmaFrom_BRAM(32*128);
        } else if (Ch == 'n') {   
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
        } else if (Ch == '4') {   reg_write_field(BD_REG32_ADDR, 0x78, 24, 8, 0xFF);  // 255
        } else if (Ch == '5') {   //dma2_BRAM(256);//S2MM
        } else if (Ch == '6') {   dmaFrom_BRAM(256*128);//MM2S  bytes: 256*(1024bit words) = 256*128 bytes
        } else if (Ch == '7') {   PL_2_BRAM_S2MM(256);
        } else if (Ch == '8') {   
        } else if (Ch == '9') {   PL_2_BRAM_S2MM(32); 
        }
      } // if (XUartPs_IsReceiveData(STDIN_BASEADDRESS))
      
    } // while
    xil_printf("\n\r----------------------------------------\n\r");
    xil_printf("** END **\n\r");
    xil_printf("----------------------------------------\n\r\n\r");

    cleanup_platform();
    
    return 0;
}
