#include "platform.h"
#include "xaxidma.h"
#include "xdebug.h"
#include "helpFunctions.h"

//XAxiDma AxiDma;

int init_dma_adg(void);
int dma2PL(u32 len);
int dmaFromPL(u32 len);
void printRXbuf(u32 len64);
void testDMA_READ(void);

#define MEM_BASE_ADDR   0x01000000
#define TX_BUFFER_BASE  (MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE  (MEM_BASE_ADDR + 0x00300000)
#define RX_BUFFER_HIGH  (MEM_BASE_ADDR + 0x004FFFFF)

#define TEST_START_VALUE	0xA5CADEAD00010000
#define POLL_TIMEOUT_COUNTER    1000000U

//#define PKT_2PL_LEN		  512*8
//#define PKT_FROM_PL_LEN 512*8
//static u64 TX_BUFFER_BASE [PKT_2PL_LEN];  
//static u64 RX_BUFFER_BASE [PKT_FROM_PL_LEN];

#define BD_REG32_ADDR   PL_REG32_ADDR


int init_dma_PL(void);
int dma2_BRAM(u32 len);
int dmaFrom_BRAM(u32 len);
void PL_2_BRAM_S2MM(u32 lenBytes);

#define PL_BRAM 0x0