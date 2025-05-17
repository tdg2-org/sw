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
#include "xgpio.h"


#define BD_REG32_ADDR   XPAR_AXIL_REG32_0_BASEADDR
//#define BD_REG32_2_ADDR 0xa0010000
#define	XGPIO_AXI_BASEADDRESS	    XPAR_XGPIO_0_BASEADDR
#define XGPIO_PL_AXI_BASEADDRESS    XPAR_GPIO_PL_BASEADDR

void ledCfg(void);
void ledToggle(int shift, int bank, int onOff);
void ledCfgBit(int shift, int bank);
void ledON(void);
void ledOff(void); 
void gitPrint (uint addr, const char *text);
void timeStampPrint (uint addr, const char *text);
void getTstamps(void);

XGpio Gpio; // gpio driver instance

#define GPIO_CH1 1
#define GPIO_CH2 2

int main()
{
    init_platform();

    int Status;

    xil_printf("\n\rtesting adg0000\n\r");
    
    check0();    
    versionCtrl();

	Status = XGpio_Initialize(&Gpio, XGPIO_AXI_BASEADDRESS);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	XGpio_SetDataDirection(&Gpio, GPIO_CH1, 0); // set all outputs 0 = out, channel 1
	XGpio_SetDataDirection(&Gpio, GPIO_CH2, 0); // set all outputs 0 = out, channel 2

    // For the PL GPIO, it won't initialize like the PS GPIO, as it doesn't know its the GPIO IP, and there's more info 
    // populated in xparameters for the PS GPIO. but low level writes/reads work fine
	XGpio_WriteReg((XGPIO_PL_AXI_BASEADDRESS),
		       ((GPIO_CH1 - 1) * XGPIO_CHAN_OFFSET) +
		       XGPIO_TRI_OFFSET, (0x0)); // all outputs ch1

	XGpio_WriteReg((XGPIO_PL_AXI_BASEADDRESS),
		       ((GPIO_CH2 - 1) * XGPIO_CHAN_OFFSET) +
		       XGPIO_TRI_OFFSET, (0xFFFFFFFF)); // all inputs ch2

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
      } else if (Ch == 'c') {   XGpio_DiscreteWrite(&Gpio, GPIO_CH1, 0);
      } else if (Ch == 'd') {   XGpio_DiscreteWrite(&Gpio, GPIO_CH1, 1);
      } else if (Ch == 'e') {   XGpio_DiscreteWrite(&Gpio, GPIO_CH1, 2);
      } else if (Ch == 'f') {   XGpio_DiscreteWrite(&Gpio, GPIO_CH1, 4);
      } else if (Ch == 'g') {   XGpio_DiscreteWrite(&Gpio, GPIO_CH2, 0);
      } else if (Ch == 'h') {   XGpio_DiscreteWrite(&Gpio, GPIO_CH2, 1);
      } else if (Ch == 'i') {   XGpio_DiscreteWrite(&Gpio, GPIO_CH2, 2);
      } else if (Ch == 'j') {   XGpio_DiscreteWrite(&Gpio, GPIO_CH2, 4);
      } else if (Ch == 'k') {   
      } else if (Ch == 'l') {   XGpio_WriteReg((XGPIO_PL_AXI_BASEADDRESS), ((GPIO_CH1 - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET, 0xA5FF0123);
      } else if (Ch == 'm') {   XGpio_WriteReg((XGPIO_PL_AXI_BASEADDRESS), ((GPIO_CH1 - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET, 0);
      } else if (Ch == 'n') {   XGpio_WriteReg((XGPIO_PL_AXI_BASEADDRESS), ((GPIO_CH1 - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET, 0xA5FF5566);
      } else if (Ch == 'o') {   
      } else if (Ch == 'q') {   
      } else if (Ch == 'r') {   
      } else if (Ch == 's') {   
      } else if (Ch == 't') {   val = XGpio_ReadReg((XGPIO_PL_AXI_BASEADDRESS), ((GPIO_CH2 - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET); xil_printf("val=%x\r\n",val);
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
