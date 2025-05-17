/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_types.h"
#include <xil_io.h>
#include "xparameters.h"
#include "helpFunctions.h"
#include <unistd.h>


#define BD_REG32_ADDR   XPAR_AXIL_REG32_0_BASEADDR
//#define BD_REG32_2_ADDR 0xa0010000
#define BD_REG32_2_ADDR 0xa0001000

int main()
{
    init_platform();

    xil_printf("\n\rtesting adg0000\n\r");
    
    check0();    
    versionCtrl();

    unsigned int val;
    //static unsigned gitLBD,gitMBD;
    //gitLBD = Xil_In32(BD_REG32_ADDR + 0x18); // gitl
    //gitMBD = Xil_In32(BD_REG32_ADDR + 0x1C); // gitm
    //xil_printf("  Scripts Git Hash: %0x%0x\n\r",gitMBD,gitLBD);

    //int val = 11;
    //val = Xil_In32(0xa0010000);
    //Xil_Out32(0xa0010000,0x5);
    //xil_printf("Val = %d\n\r",val);
    //xil_printf("Val = %x\n\r",val);
    
    xil_printf("Running...\r\n");
    s8 Ch;
    while (1) {
      Ch = inbyte();
      if (Ch == '\r') {
          outbyte('\n');
      }
      outbyte(Ch);
      xil_printf("\r\n");

      if (Ch == 'p') {
        xil_printf("\r\n POWER OFF\r\n");
        usleep(10000);
        powerOff();
      } else if (Ch == 'b') {break;
      } else if (Ch == 'a') {   xil_printf("NULL\r\n");
      } else if (Ch == 'c') {
      } else if (Ch == 'd') {
      } else if (Ch == 'e') {
      } else if (Ch == 'f') {
      } else if (Ch == 'g') {
      } else if (Ch == 'h') {
      } else if (Ch == 'i') {
      } else if (Ch == 'j') {
      } else if (Ch == 'k') {
      } else if (Ch == 'l') {
      } else if (Ch == 'm') {
      } else if (Ch == 'n') {
        //val = Xil_In32(BD_REG32_2_ADDR + 0x0); xil_printf("gh0=%d\r\n",val);
        //val = Xil_In32(BD_REG32_2_ADDR + 0x4); xil_printf("gh1=%d\r\n",val);
        //val = Xil_In32(BD_REG32_2_ADDR + 0x8); xil_printf("ts=%d\r\n",val);
        //val = Xil_In32(BD_REG32_2_ADDR + 0xC); xil_printf("const=%x\r\n",val);
        //val = Xil_In32(BD_REG32_2_ADDR + 0x10); xil_printf("const=%x\r\n",val);
        //Xil_Out32(BD_REG32_2_ADDR + 0x1C, 0x5);
        //Xil_Out32(BD_REG32_2_ADDR + 0x18, 0x7);
      }
      //} else if (Ch == '0') {Xil_Out32(BD_REG32_ADDR + 0x2C, 0x0);
      //} else if (Ch == '1') {Xil_Out32(BD_REG32_ADDR + 0x2C, 0x1);
      //} else if (Ch == '2') {Xil_Out32(BD_REG32_ADDR + 0x2C, 0x2);
      //} else if (Ch == '3') {Xil_Out32(BD_REG32_ADDR + 0x2C, 0x3);
      //} else if (Ch == '4') {Xil_Out32(BD_REG32_ADDR + 0x2C, 0x4);
      //} else if (Ch == '5') {Xil_Out32(BD_REG32_ADDR + 0x2C, 0x5);
      //}
    }
    xil_printf("\n\r----------------------------------------\n\r");
    xil_printf("** END **\n\r");
    xil_printf("----------------------------------------\n\r\n\r");

    cleanup_platform();
    
    return 0;
}
