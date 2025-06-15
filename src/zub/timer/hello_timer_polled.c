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
#include "xtmrctr.h"


#define BD_REG32_ADDR   XPAR_AXIL_REG32_0_BASEADDR
#define TMRADDR         XPAR_XTMRCTR_0_BASEADDR
#define TIMER1  0
#define TIMER2  1
#define SEC1    100000000

XTmrCtr TimerCounter; /* The instance of the Tmrctr Device */


int main()
{
    init_platform();
    int Status;
    xil_printf("\n\rtesting adg0000\n\r");
    check0();    
    versionCtrl();

    u32 cnt1,cnt2,exp1,exp2;

    XTmrCtr *TmrCtrInstancePtr = &TimerCounter;
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, TMRADDR);
	if (Status != XST_SUCCESS) {
		xil_printf("Init failed\r\n");
	}
	Status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TIMER1);
	if (Status != XST_SUCCESS) {
		xil_printf("SelfTest failed\r\n");
	}
	Status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TIMER2);
	if (Status != XST_SUCCESS) {
		xil_printf("SelfTest failed\r\n");
	}
	XTmrCtr_SetOptions(TmrCtrInstancePtr, TIMER1, XTC_AUTO_RELOAD_OPTION); // autoreload mode
	XTmrCtr_SetOptions(TmrCtrInstancePtr, TIMER2, XTC_AUTO_RELOAD_OPTION); // autoreload mode

    XTmrCtr_SetResetValue(TmrCtrInstancePtr, TIMER1, 0);
    XTmrCtr_SetResetValue(TmrCtrInstancePtr, TIMER2, 0);
    XTmrCtr_Reset(TmrCtrInstancePtr, TIMER1);
    XTmrCtr_Reset(TmrCtrInstancePtr, TIMER2);
    XTmrCtr_Start(TmrCtrInstancePtr, TIMER1); // start timer
    XTmrCtr_Start(TmrCtrInstancePtr, TIMER2); // start timer

	while (1) {
		cnt1 = XTmrCtr_GetValue(TmrCtrInstancePtr, TIMER1);
		cnt2 = XTmrCtr_GetValue(TmrCtrInstancePtr, TIMER2);
		if (cnt1 >= SEC1) {
			xil_printf("TIMER!\r\n");
            XTmrCtr_Reset(TmrCtrInstancePtr, TIMER1);
        } else if (cnt2 >= 3*SEC1) {
            xil_printf("TIMER2!\r\n");
            XTmrCtr_Reset(TmrCtrInstancePtr, TIMER2);
        } else if (XTmrCtr_IsExpired(TmrCtrInstancePtr, TIMER1)) {
        //if (XTmrCtr_IsExpired(TmrCtrInstancePtr, TIMER1)) {
            xil_printf("Expired!\r\n");
            XTmrCtr_Reset(TmrCtrInstancePtr, TIMER1);
        }
	}


    unsigned int val;
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
      } else if (Ch == 'o') {   
      } else if (Ch == 'q') {   
      } else if (Ch == 'r') {   
      } else if (Ch == 's') {   
      } else if (Ch == 't') {
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
