#include "xparameters.h"
#include "xil_types.h"
#include <unistd.h> // linux stuff including sleep/usleep etc.

#define PL_REG32_ADDR   XPAR_PL_AXIL_REG32_0_BASEADDR

// GPIO Module UG1087
#define GPIO_MIO_ADDR   XPAR_GPIO_BASEADDR
#define MIO_WR_0_25_OFFSET      0x40    
#define MIO_RD_0_25_OFFSET      0x60
#define MIO_DIRM_0              0x204
#define MIO_OEN_0               0x208

#define MIO_WR_26_51_OFFSET     0x44    
#define MIO_RD_26_51_OFFSET     0x64
#define MIO_DIRM_1              0x244
#define MIO_OEN_1               0x248

void versionCtrl(void);
void powerOff(void);
void check0(void);
void ledCfg(void);
void ledToggle(int shift, int bank, int onOff);
void ledCfgBit(int shift, int bank);
void ledON(void);
void ledOff(void); 
void gitPrint (uint addr, const char *text);
void timeStampPrint (uint addr, const char *text);
void getTstamps(void);
void getVersions(void);
