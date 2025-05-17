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
#define BD_REG32_2_ADDR 0xa0010000

void ledCfg(void);
void ledToggle(int shift, int bank, int onOff);
void ledCfgBit(int shift, int bank);
void ledON(void);
void ledOff(void); 
void gitPrint (uint addr, const char *text);
void timeStampPrint (uint addr, const char *text);
void getTstamps(void);

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
      } else if (Ch == 'c') {   val = Xil_In32(BD_REG32_2_ADDR + 0x0); xil_printf("gh0=%d\r\n",val);
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
// git_hash[31: 0];    //  0x0
// git_hash[63:32];    //  0x4
// timestamp;          //  0x8
// git_hash_led[31: 0];//  0xC
// git_hash_led[63:32];//  0x10
// timestamp_led;      //  0x14




// functions for ZUBoard (NOT u96)
void getTstamps(void) 
{
    gitPrint(0x0,"Top");
    gitPrint(0xC,"Scripts");
    gitPrint(0x18,"Common");
    gitPrint(0x24,"BD");
    timeStampPrint(0x8,"Top");
    timeStampPrint(0x14,"Scripts");
    timeStampPrint(0x20,"Common");
    timeStampPrint(0x2C,"BD");
}


// LED MIO - 7,24,25,33
void ledON(void) 
{
    ledToggle(7,0,1);
    ledToggle(24,0,1);
    ledToggle(25,0,1);
    ledToggle(33,1,1);
}
void ledOff(void) 
{
    ledToggle(7,0,0);
    ledToggle(24,0,0);
    ledToggle(25,0,0);
    ledToggle(33,1,0);
}

void ledCfg(void) 
{
    ledCfgBit(7,0);
    ledCfgBit(24,0);
    ledCfgBit(25,0);
    ledCfgBit(33,1);
}

void ledCfgBit(int shift, int bank) 
{
    int mask, writeVal, regRD, regWR, dir_addr,oen_addr;

    switch (bank) {
        case 0: 
            dir_addr = MIO_DIRM_0;
            oen_addr = MIO_OEN_0;
            break;
        case 1:
            dir_addr = MIO_DIRM_1;
            oen_addr = MIO_OEN_1;
            break;
        default:   
            xil_printf("ERROR led cfg\n");
            return; 
    }

    //shift = 7; // MIO7, MIO bank0 pins 0:25
    mask = 0x1 << shift; // 1bits
    writeVal = 0x1 << shift; // set output and enable
    
    regRD = Xil_In32(GPIO_MIO_ADDR + dir_addr);
    regWR = (regRD & ~mask) | (writeVal & mask);
    Xil_Out32(GPIO_MIO_ADDR + dir_addr,regWR);

    regRD = Xil_In32(GPIO_MIO_ADDR + oen_addr);
    regWR = (regRD & ~mask) | (writeVal & mask);
    Xil_Out32(GPIO_MIO_ADDR + oen_addr,regWR);

}

void ledToggle(int shift, int bank, int onOff) 
{
    int writeVal, regRD, regWR, mioRD_addr, mioWR_addr, mask;

    switch (bank) {
        case 0: 
            mioRD_addr = MIO_RD_0_25_OFFSET;
            mioWR_addr = MIO_WR_0_25_OFFSET;
            break;
        case 1:
            mioRD_addr = MIO_RD_26_51_OFFSET;
            mioWR_addr = MIO_WR_26_51_OFFSET;
            break;
        default:
            xil_printf("ERROR led toggle\n");
            return; 
    }

    mask = 0x1 << shift; // 1bits
    writeVal = onOff << shift;
    regRD = Xil_In32(GPIO_MIO_ADDR + mioRD_addr);
    regWR = (regRD & ~mask) | (writeVal & mask);
    Xil_Out32(GPIO_MIO_ADDR + mioWR_addr,regWR);
}

void gitPrint (uint addr, const char *text) 
{
    uint gitLSB,gitMSB;

    gitLSB = Xil_In32(PL_REG32_ADDR + addr);
    gitMSB = Xil_In32(PL_REG32_ADDR + addr + 0x4);

    xil_printf("Git Hash (%s): %08x%08x\n\r",text,gitMSB,gitLSB);
}

void timeStampPrint (uint addr, const char *text) 
{
    uint tStamp = Xil_In32(PL_REG32_ADDR + addr);

    static unsigned sec,min,hr,yr,mon,day;
    //sec = (timeStamp & (((1 << numBits) - 1) << startBit)) >> startBit;   //  09B1219F  Fri Mar  1 18:06:31 2024
    sec = (tStamp & (((1 << 6) - 1) << 0)) >> 0;
    min = (tStamp & (((1 << 6) - 1) << 6)) >> 6;
    hr  = (tStamp & (((1 << 5) - 1) << 12)) >> 12;
    yr  = (tStamp & (((1 << 6) - 1) << 17)) >> 17;
    mon = (tStamp & (((1 << 4) - 1) << 23)) >> 23;
    day = (tStamp & (((1 << 5) - 1) << 27)) >> 27;

    xil_printf("TIMESTAMP (%s):%08x = %02d/%02d/%02d - %02d:%02d:%02d\n\r",text,tStamp,mon,day,yr,hr,min,sec); // 0's mean zero padded on left (UG643)
}