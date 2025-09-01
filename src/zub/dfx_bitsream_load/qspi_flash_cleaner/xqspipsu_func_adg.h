
#include "xqspipsu_flash_config.h"
#include "xil_printf.h"

#define PAGE_COUNT		529
#define PAGE_COUNT2		1125

#define MAX_PAGE_SIZE 256

extern u8 ReadBuffer[(PAGE_COUNT * MAX_PAGE_SIZE) + (DATA_OFFSET + DUMMY_SIZE) * 8];
extern u8 ReadBuffer2[(PAGE_COUNT2 * MAX_PAGE_SIZE) + (DATA_OFFSET + DUMMY_SIZE) * 8];

int run_qspi_read_test(void);
