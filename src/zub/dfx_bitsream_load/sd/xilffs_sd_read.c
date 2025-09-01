/*
 ADG NOTE. 

 from xilinx example xilffs_polled_example.c

 enable LFN in BSP
 	XILFFS_use_lfn = 1


*/
/***************************** Include Files *********************************/

//#include "xparameters.h"	/* SDK generated parameters */
//#include "xsdps.h"		/* SD device driver */
//#include "xil_printf.h"
//#include "ff.h"
//#include "xil_cache.h"
//#include "xplatform_info.h"

#include "xilffs_sd_read.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int FfsSdPolledExample(void);

/************************** Variable Definitions *****************************/
static FIL fil;		/* File object */
static FATFS fatfs;
/*
 * To test logical drive 0, FileName should be "0:/<File name>" or
 * "<file_name>". For logical drive 1, FileName should be "1:/<file_name>"
 */
static char FileName[32] = "1:/RM0_RM_led_2_partial.bin";
static char FileName2[32] = "1:/RM1_RM_led2_2_partial.bin";
static char *SD_File;


u8 DestinationAddress[RM0_LEN] __attribute__ ((aligned(32)));
u8 DestinationAddress2[RM1_LEN] __attribute__ ((aligned(32)));

u8 SourceAddress[10*1024] __attribute__ ((aligned(32)));


#define TEST 7
MKFS_PARM mkfs_parm;
/*****************************************************************************/
/**
*
* Main function to call the SD example.
*
* @param	None
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/

//int main(void)
int run_SD_read(void)
{
	int Status;

	xil_printf("SD Polled File System Example Test \r\n");

	Status = FfsSdPolledExample();
	if (Status != XST_SUCCESS) {
		xil_printf("SD Polled File System Example Test failed \r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran SD Polled File System Example Test \r\n");

	return XST_SUCCESS;

}

/*****************************************************************************/
/**
*
* File system example using SD driver to write to and read from an SD card
* in polled mode. This example creates a new file on an
* SD card (which is previously formatted with FATFS), write data to the file
* and reads the same data back to verify.
*
* @param	None
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int FfsSdPolledExample(void)
{
	FRESULT Res;
	UINT NumBytesRead;
	UINT NumBytesWritten;
	u32 BuffCnt;
	BYTE work[FF_MAX_SS];
	u32 FileSize = (RM0_LEN);
	u32 FileSize2 = (RM1_LEN);

	/* interface SD0 vs SD1 on zynqMP PS config
	 * To test logical drive 0, Path should be "0:/"
	 * For logical drive 1, Path should be "1:/"
	 */
	const TCHAR *Path = "1:/";

	/*
	 * Register volume work area, initialize device
	 */
	Res = f_mount(&fatfs, Path, 1);

	if (Res != FR_OK) {
		xil_printf("f_mount\r\n");
		return XST_FAILURE;
	}

	 mkfs_parm.fmt = FM_FAT32;

	/*
	 * Open file with required permissions.
	 * Here - Creating new file with read/write permissions. .
	 * To open file with write permissions, file system should not
	 * be in Read Only mode.
	 */
/* First file RM0 ******************************************************************/	
	SD_File = (char *)FileName;

	//Res = f_open(&fil, SD_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	Res = f_open(&fil, SD_File, FA_READ);
	if (Res) {
		xil_printf("f_open\r\n");
		return XST_FAILURE;
	}

	/*
	 * Pointer to beginning of file .
	 */
	Res = f_lseek(&fil, 0);
	if (Res) {
		xil_printf("f_lseek\r\n");
		return XST_FAILURE;
	}

	/*
	 * Read data from file.
	 */
	Res = f_read(&fil, (void*)DestinationAddress, FileSize,	&NumBytesRead);
	if (Res) {
		xil_printf("f_read\r\n");
		return XST_FAILURE;
	}

	/*
	 * Close file.
	 */
	Res = f_close(&fil);
	if (Res) {
		xil_printf("f_close\r\n");
		return XST_FAILURE;
	}

/* 2nd file RM1 ******************************************************************/	
	SD_File = (char *)FileName2;

	//Res = f_open(&fil, SD_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	Res = f_open(&fil, SD_File, FA_READ);
	if (Res) {
		xil_printf("f_open\r\n");
		return XST_FAILURE;
	}

	/*
	 * Pointer to beginning of file .
	 */
	Res = f_lseek(&fil, 0);
	if (Res) {
		xil_printf("f_lseek\r\n");
		return XST_FAILURE;
	}

	/*
	 * Read data from file.
	 */
	Res = f_read(&fil, (void*)DestinationAddress2, FileSize2,	&NumBytesRead);
	if (Res) {
		xil_printf("f_read\r\n");
		return XST_FAILURE;
	}

	/*
	 * Close file.
	 */
	Res = f_close(&fil);
	if (Res) {
		xil_printf("f_close\r\n");
		return XST_FAILURE;
	}


/* verify ******************************************************************/	
	u32 Count;

	for (Count = 60; Count < 72; Count++) {
		xil_printf("RX(%d) = 0x%02x\n\r",Count,DestinationAddress[Count]);
	}
	xil_printf("--\n\r");
	for (Count = 135168; Count < 135180; Count++) {
		xil_printf("RX(%d) = 0x%02x\n\r",Count,DestinationAddress[Count]);
	}
	xil_printf("--\n\r");
	for (Count = 135240; Count < 135252; Count++) {
		xil_printf("RX(%d) = 0x%02x\n\r",Count,DestinationAddress[Count]);
	}
	
	xil_printf("--2--\n\r");
	for (Count = 60; Count < 84; Count++) {
		xil_printf("RX2(%d) = 0x%02x\n\r",Count,DestinationAddress2[Count]);
	}
	xil_printf("--\n\r");
	for (Count = 287832; Count < 287844; Count++) {
		xil_printf("RX2(%d) = 0x%02x\n\r",Count,DestinationAddress2[Count]);
	}
	xil_printf("--\n\r");
	for (Count = 287904; Count < 287916; Count++) {
		xil_printf("RX2(%d) = 0x%02x\n\r",Count,DestinationAddress2[Count]);
	}
	xil_printf("--\n\r");


	return XST_SUCCESS;
}
