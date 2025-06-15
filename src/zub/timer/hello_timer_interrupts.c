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
#include "xinterrupt_wrap.h"
#include "xscugic.h"


#define BD_REG32_ADDR   XPAR_AXIL_REG32_0_BASEADDR
#define TMRADDR         XPAR_XTMRCTR_0_BASEADDR
#define TIMER1      0
#define TIMER2      1
#define SEC1        100000000
#define RESET_VALUE 0xF0000000
#define RESET_VALUE2 0xC0000000

XTmrCtr TimerCounterInst; /* The instance of the Tmrctr Device */

static void TmrCtrDisableIntr( u16 IntrId, UINTPTR IntrParent);

static void TimerCounterHandler(void *CallBackRef, u8 TmrCtrNumber);

volatile int TimerExpired;

int main()
{
    init_platform();
    int Status;
    xil_printf("\n\rtesting adg0000\n\r");
    check0();    
    versionCtrl();

    int LastTimerExpired = 0;
    XTmrCtr *TmrCtrInstancePtr = &TimerCounterInst;
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, TMRADDR);
	if (Status != XST_SUCCESS) {
		xil_printf("Init fail\r\n");
	}
	Status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TIMER1);
	if (Status != XST_SUCCESS) {
		xil_printf("selfTest fail\r\n");
	}
	Status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TIMER2);
	if (Status != XST_SUCCESS) {
		xil_printf("selfTest fail\r\n");
	}

	Status = XSetupInterruptSystem(TmrCtrInstancePtr, (XInterruptHandler)XTmrCtr_InterruptHandler, \
				       TmrCtrInstancePtr->Config.IntrId, TmrCtrInstancePtr->Config.IntrParent, \
				       XINTERRUPT_DEFAULT_PRIORITY);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XTmrCtr_SetHandler(TmrCtrInstancePtr, TimerCounterHandler, TmrCtrInstancePtr);
	XTmrCtr_SetOptions(TmrCtrInstancePtr, TIMER1, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	XTmrCtr_SetOptions(TmrCtrInstancePtr, TIMER2, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	XTmrCtr_SetResetValue(TmrCtrInstancePtr, TIMER1, RESET_VALUE);
	XTmrCtr_SetResetValue(TmrCtrInstancePtr, TIMER2, RESET_VALUE2);
	XTmrCtr_Start(TmrCtrInstancePtr, TIMER1);
	XTmrCtr_Start(TmrCtrInstancePtr, TIMER2);

    TimerExpired = 0;

	while (1) {
		while (TimerExpired == LastTimerExpired) {
		}
		LastTimerExpired = TimerExpired;

		if (TimerExpired == 10) {
			XTmrCtr_Stop(TmrCtrInstancePtr, TIMER1);
			XTmrCtr_Stop(TmrCtrInstancePtr, TIMER2);
			break;
		}
	}
	TmrCtrDisableIntr(TmrCtrInstancePtr->Config.IntrId, TmrCtrInstancePtr->Config.IntrParent);



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


void TimerCounterHandler(void *CallBackRef, u8 TmrCtrNumber)
{
	XTmrCtr *InstancePtr = (XTmrCtr *)CallBackRef;

	if (XTmrCtr_IsExpired(InstancePtr, TIMER1)) {
		xil_printf("** Interrupt1! **\n\r");
        TimerExpired++;
		if (TimerExpired == 10) {
			XTmrCtr_SetOptions(InstancePtr, TmrCtrNumber, 0);
		}
	} else if (XTmrCtr_IsExpired(InstancePtr, TIMER2)) {
        xil_printf("** Interrupt2! **\n\r");
         TimerExpired++;
        if (TimerExpired == 10) {
        	XTmrCtr_SetOptions(InstancePtr, TmrCtrNumber, 0);
        }
    }
}

void TmrCtrDisableIntr( u16 IntrId, UINTPTR IntrParent)
{
	XDisableIntrId( IntrId, IntrParent);
}
