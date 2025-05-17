
#include "helpFunctions.h"
#include <stdio.h>
#include "xil_printf.h"
#include <xil_io.h>


void versionCtrl(void) {

    static unsigned gitL,gitM,timeStamp,gitLscr,gitMscr,gitLBD,gitMBD;

    gitLscr = Xil_In32(PL_REG32_ADDR + 0xC); // gitl
    gitMscr = Xil_In32(PL_REG32_ADDR + 0x10); // gitm

    gitL = Xil_In32(PL_REG32_ADDR + 0x0); // gitl
    gitM = Xil_In32(PL_REG32_ADDR + 0x4); // gitm
    timeStamp = Xil_In32(PL_REG32_ADDR + 0x8); // timestamp

    static unsigned sec,min,hr,yr,mon,day;
    //sec = (timeStamp & (((1 << numBits) - 1) << startBit)) >> startBit;   //  09B1219F  Fri Mar  1 18:06:31 2024
    sec = (timeStamp & (((1 << 6) - 1) << 0)) >> 0;
    min = (timeStamp & (((1 << 6) - 1) << 6)) >> 6;
    hr  = (timeStamp & (((1 << 5) - 1) << 12)) >> 12;
    yr  = (timeStamp & (((1 << 6) - 1) << 17)) >> 17;
    mon = (timeStamp & (((1 << 4) - 1) << 23)) >> 23;
    day = (timeStamp & (((1 << 5) - 1) << 27)) >> 27;

    xil_printf("\n\r************ PL VERSION ****************\n\r");
    xil_printf("  Scripts Git Hash: %08x%08x\n\r",gitMscr,gitLscr);
    xil_printf("  TIMESTAMP:%08x = %02d/%02d/%02d - %02d:%02d:%02d\n\r",timeStamp,mon,day,yr,hr,min,sec); // 0's mean zero padded on left (UG643)
    xil_printf("  Git Hash: %08x%08x\n\r",gitM,gitL);
    xil_printf("  %08x_%08x\n\r",timeStamp,gitM);
    xil_printf("****************************************\n\r\n\r");

}

// power_kill = pin MIO34, left shift 8 for register MIO pins 26-51
// same for Ultra96v2 and ZUBoard
void powerOff(void) {
    int shift, mask, val, pwrRD, pwrWR, pwrOFF;
    
    shift = 8;  // MIO34, 8 from 26, MIO bank1 pins 26:51
    mask = 0x1 << shift; // 1bits
    val = 0x1 << shift; // set output and enable
    pwrOFF = 0x0 << shift;
    pwrRD = Xil_In32(GPIO_MIO_ADDR + MIO_DIRM_1);
    pwrWR = (pwrRD & ~mask) | (val & mask);
    Xil_Out32(GPIO_MIO_ADDR + MIO_DIRM_1,pwrWR);

    pwrRD = Xil_In32(GPIO_MIO_ADDR + MIO_OEN_1);
    pwrWR = (pwrRD & ~mask) | (val & mask);
    Xil_Out32(GPIO_MIO_ADDR + MIO_OEN_1,pwrWR);

    pwrRD = Xil_In32(GPIO_MIO_ADDR + MIO_RD_26_51_OFFSET);
    pwrWR = (pwrRD & ~mask) | (pwrOFF & mask);
    Xil_Out32(GPIO_MIO_ADDR + MIO_WR_26_51_OFFSET,pwrWR);

}

void check0(void) {
    
    #if defined (__aarch64__)
        xil_printf("__aarch64__\r\n");
    #else 
        xil_printf("NOT aarch64 \r\n");
    #endif

    #ifdef SDT 
        xil_printf("\n\rSDT\n\r");
    #else 
        xil_printf("\n\rNOT SDT\n\r");
    #endif

}