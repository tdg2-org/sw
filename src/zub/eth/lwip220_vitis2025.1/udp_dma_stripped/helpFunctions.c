
#include "helpFunctions.h"
#include <stdio.h>
#include "xil_printf.h"
#include <xil_io.h>


void versionCtrl(void) {

    static unsigned gitLtop,gitLscr,gitLBD,gitLcom,gitLsw,gitLip;
    static unsigned gitMtop,gitMscr,gitMBD,gitMcom,gitMsw,gitMip;
    static unsigned tsTop,tsBD,tsScripts,tsCom,tsSw,tsIp; // TimeStamp

    //top
    gitLtop = Xil_In32(PL_REG32_ADDR + 0x0);
    gitMtop = Xil_In32(PL_REG32_ADDR + 0x4);   
    tsTop = Xil_In32(PL_REG32_ADDR + 0x8); 

    //scripts
    gitLscr = Xil_In32(PL_REG32_ADDR + 0xC); 
    gitMscr = Xil_In32(PL_REG32_ADDR + 0x10); 
    tsScripts = Xil_In32(PL_REG32_ADDR + 0x14);

    //common
    gitLcom = Xil_In32(PL_REG32_ADDR + 0x18);
    gitMcom = Xil_In32(PL_REG32_ADDR + 0x1C);
    tsCom = Xil_In32(PL_REG32_ADDR + 0x20); 

    //bd
    gitLBD = Xil_In32(PL_REG32_ADDR + 0x24);
    gitMBD = Xil_In32(PL_REG32_ADDR + 0x28);
    tsBD = Xil_In32(PL_REG32_ADDR + 0x2C); 

    //sw
    gitLsw = Xil_In32(PL_REG32_ADDR + 0x30);
    gitMsw = Xil_In32(PL_REG32_ADDR + 0x34);
    tsSw = Xil_In32(PL_REG32_ADDR + 0x38); 

    //ip
    gitLip = Xil_In32(PL_REG32_ADDR + 0x3c);
    gitMip = Xil_In32(PL_REG32_ADDR + 0x40);
    tsIp = Xil_In32(PL_REG32_ADDR + 0x44); 

    static unsigned sec,min,hr,yr,mon,day;
    //sec = (tsTop & (((1 << numBits) - 1) << startBit)) >> startBit;   //  09B1219F  Fri Mar  1 18:06:31 2024

    /* top */
    sec = (tsTop & (((1 << 6) - 1) << 0)) >> 0;
    min = (tsTop & (((1 << 6) - 1) << 6)) >> 6;
    hr  = (tsTop & (((1 << 5) - 1) << 12)) >> 12;
    yr  = (tsTop & (((1 << 6) - 1) << 17)) >> 17;
    mon = (tsTop & (((1 << 4) - 1) << 23)) >> 23;
    day = (tsTop & (((1 << 5) - 1) << 27)) >> 27;

    xil_printf("\n\r************ TimeStamps ****************\n\r");
    xil_printf("  PL      : %08x = %02d/%02d/%02d - %02d:%02d:%02d\n\r",tsTop,mon,day,yr,hr,min,sec); // 0's mean zero padded on left (UG643)
    //xil_printf("  PL Git Hash: %08x%08x\n\r",gitMtop,gitLtop);

    /* BD */
    sec = (tsBD & (((1 << 6) - 1) << 0)) >> 0;
    min = (tsBD & (((1 << 6) - 1) << 6)) >> 6;
    hr  = (tsBD & (((1 << 5) - 1) << 12)) >> 12;
    yr  = (tsBD & (((1 << 6) - 1) << 17)) >> 17;
    mon = (tsBD & (((1 << 4) - 1) << 23)) >> 23;
    day = (tsBD & (((1 << 5) - 1) << 27)) >> 27;

    xil_printf("  BD      : %08x = %02d/%02d/%02d - %02d:%02d:%02d\n\r",tsBD,mon,day,yr,hr,min,sec); // 0's mean zero padded on left (UG643)
    //xil_printf("  BD Git Hash: %08x%08x\n\r",gitMBD,gitLBD);
    
    /* Scripts */
    sec = (tsScripts & (((1 << 6) - 1) << 0)) >> 0;
    min = (tsScripts & (((1 << 6) - 1) << 6)) >> 6;
    hr  = (tsScripts & (((1 << 5) - 1) << 12)) >> 12;
    yr  = (tsScripts & (((1 << 6) - 1) << 17)) >> 17;
    mon = (tsScripts & (((1 << 4) - 1) << 23)) >> 23;
    day = (tsScripts & (((1 << 5) - 1) << 27)) >> 27;

    xil_printf("  Scripts : %08x = %02d/%02d/%02d - %02d:%02d:%02d\n\r",tsScripts,mon,day,yr,hr,min,sec); // 0's mean zero padded on left (UG643)
    //xil_printf("  Scripts Git Hash: %08x%08x\n\r",gitMscr,gitLscr);
    
    /* Common */
    sec = (tsCom & (((1 << 6) - 1) << 0)) >> 0;
    min = (tsCom & (((1 << 6) - 1) << 6)) >> 6;
    hr  = (tsCom & (((1 << 5) - 1) << 12)) >> 12;
    yr  = (tsCom & (((1 << 6) - 1) << 17)) >> 17;
    mon = (tsCom & (((1 << 4) - 1) << 23)) >> 23;
    day = (tsCom & (((1 << 5) - 1) << 27)) >> 27;

    xil_printf("  Common  : %08x = %02d/%02d/%02d - %02d:%02d:%02d\n\r",tsCom,mon,day,yr,hr,min,sec); // 0's mean zero padded on left (UG643)
    //xil_printf("  Common Git Hash: %08x%08x\n\r",gitMcom,gitLcom);

    /* SW */
    sec = (tsSw & (((1 << 6) - 1) << 0)) >> 0;
    min = (tsSw & (((1 << 6) - 1) << 6)) >> 6;
    hr  = (tsSw & (((1 << 5) - 1) << 12)) >> 12;
    yr  = (tsSw & (((1 << 6) - 1) << 17)) >> 17;
    mon = (tsSw & (((1 << 4) - 1) << 23)) >> 23;
    day = (tsSw & (((1 << 5) - 1) << 27)) >> 27;

    xil_printf("  SW      : %08x = %02d/%02d/%02d - %02d:%02d:%02d\n\r",tsSw,mon,day,yr,hr,min,sec); // 0's mean zero padded on left (UG643)
    //xil_printf("  SW Git Hash: %08x%08x\n\r",gitMsw,gitLsw);

    /* IP */
    sec = (tsIp & (((1 << 6) - 1) << 0)) >> 0;
    min = (tsIp & (((1 << 6) - 1) << 6)) >> 6;
    hr  = (tsIp & (((1 << 5) - 1) << 12)) >> 12;
    yr  = (tsIp & (((1 << 6) - 1) << 17)) >> 17;
    mon = (tsIp & (((1 << 4) - 1) << 23)) >> 23;
    day = (tsIp & (((1 << 5) - 1) << 27)) >> 27;

    xil_printf("  IP      : %08x = %02d/%02d/%02d - %02d:%02d:%02d\n\r",tsIp,mon,day,yr,hr,min,sec); // 0's mean zero padded on left (UG643)
    //xil_printf("  IP Git Hash: %08x%08x\n\r",gitMip,gitLip);


    xil_printf("\n\r************ Git Hashes ****************\n\r");
    xil_printf("  PL      : %08x%08x\n\r",gitMtop,gitLtop);
    xil_printf("  BD      : %08x%08x\n\r",gitMBD,gitLBD);
    xil_printf("  Scripts : %08x%08x\n\r",gitMscr,gitLscr);
    xil_printf("  Common  : %08x%08x\n\r",gitMcom,gitLcom);
    xil_printf("  SW      : %08x%08x\n\r",gitMsw,gitLsw);
    xil_printf("  IP      : %08x%08x\n\r",gitMip,gitLip);


    /*** END ***/    
    xil_printf("\n\r  %08x_%08x\n\r",tsTop,gitMtop);
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
        xil_printf("SDT Flow\n\r");
    #else 
        xil_printf("Classic. NOT SDT Flow\n\r");
    #endif

    #if defined (__arm__)
        xil_printf("__arm__\r\n");
    #else
        xil_printf("NOT __arm__\r\n");
    #endif 

    #if defined (ARMR5)
        xil_printf("ARMR5\r\n");
    #else
        xil_printf("NOT ARMR5\r\n");
    #endif 
}

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
