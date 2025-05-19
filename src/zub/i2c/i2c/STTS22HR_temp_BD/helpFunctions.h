#include "xparameters.h"

#define PL_REG32_ADDR   XPAR_AXIL_REG32_0_BASEADDR

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

