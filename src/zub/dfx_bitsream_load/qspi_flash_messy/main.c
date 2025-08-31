

#include <stdio.h>
#include "xil_printf.h"
#include "xil_types.h"
#include <xil_io.h>
#include "xparameters.h"
#include "helpFunctions.h"
#include "sleep.h"
#include "platform.h"
#include "xuartps.h"

#include "xilfpgaFunc_adg.h"
#include "xqspipsu_generic_flash_polled_example.h"

#define BD_REG32_ADDR   PL_REG32_ADDR

const u32 RM0_RM_led_2_partial_bin_len = 135248;
const u32 RM1_RM_led2_2_partial_bin_len = 287912;

int main()
{
    init_platform();

    u32 rm0_len = RM0_RM_led_2_partial_bin_len;
    u32 rm1_len = RM1_RM_led2_2_partial_bin_len;

    xil_printf("\n\rtesting adg0000\n\r");
    
    check0();    
    getVersions();
    //versionCtrl();
    ledCfg();

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
        } else if (Ch == 'e') {   //loadPartialBit((UINTPTR)RM0_RM_led_2_partial_bin, RM0_RM_led_2_partial_bin_len);   
        } else if (Ch == 'f') {      
        } else if (Ch == 'g') {   //loadPartialBit((UINTPTR)RM1_RM_led2_2_partial_bin, RM1_RM_led2_2_partial_bin_len);   
        } else if (Ch == 'h') {   
        } else if (Ch == 'i') {   run_qspi_read_test();
        } else if (Ch == 'j') {   loadPartialBit((UINTPTR)ReadBuffer, rm0_len);
        } else if (Ch == 'k') {   loadPartialBit((UINTPTR)ReadBuffer2, rm1_len);
        } else if (Ch == 'l') {   
        } else if (Ch == 'm') {   
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

