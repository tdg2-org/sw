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

#include "xilffs_func_adg.h"
//#include <malloc.h>
#include <xil_printf.h>
#include <xstatus.h>

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int FfsSdPolledExample(void);
static u64 div_round(u64 a, u64 b);

/************************** Variable Definitions *****************************/
static FIL fil;		/* File object */
static FATFS fatfs;
/*
 * To test logical drive 0, FileName should be "0:/<File name>" or
 * "<file_name>". For logical drive 1, FileName should be "1:/<file_name>"
 */
static char FileName[32] = "0:/RM0_RM_led_2_partial.bin"; 	// SD0 = emmc
static char FileName2[32] = "0:/RM1_RM_led2_2_partial.bin";
static char *SD_File;

//const u32 RM0_len = 135248;
//const u32 RM1_len = 287912;

u8 DestinationAddress[RM0_LEN] __attribute__ ((aligned(32)));
u8 DestinationAddress2[RM1_LEN] __attribute__ ((aligned(32)));

u8 SourceAddress[10*1024] __attribute__ ((aligned(32)));


#define TEST 7
MKFS_PARM mkfs_parm;
/*****************************************************************************/
static int read_bin(const char *srcFile, u8 *buf, u32 bufLen)
{
	FRESULT Res;
	UINT NumBytesRead;

	Res = f_open(&fil, srcFile, FA_READ);
	if (Res != FR_OK) {
		xil_printf("read_bin\r\n");
		return XST_FAILURE;
	}

	// Read data from file.
	Res = f_read(&fil, buf, bufLen, &NumBytesRead);
	if (Res != FR_OK) {
		xil_printf("f_read\r\n");
		return XST_FAILURE;
	}
	
	Res = f_close(&fil);//Close file.
	if (Res != FR_OK) {
		xil_printf("f_close\r\n");
		return XST_FAILURE;
	}
	xil_printf("File read %s, bufLen = %u, readLen = %u \r\n", srcFile,bufLen,NumBytesRead);
	return XST_SUCCESS;
}

static void print_bin_test(u8 *buf, u32 cntMin, u32 cntMax) 
{
	u32 Count;

	for (Count = cntMin; Count < cntMax; Count++) {
		xil_printf("buf(%d) = 0x%02x\n\r",Count,buf[Count]);
	}
	xil_printf("--\n\r");
}

int read_emmc(void)
{
	int status;
	
	mountVol(0); // mount SD0 emmc

	status = read_bin(FileName,DestinationAddress,RM0_LEN);
	if (status) return XST_FAILURE;
	status = read_bin(FileName2,DestinationAddress2,RM1_LEN);
	if (status) return XST_FAILURE;

	print_bin_test(DestinationAddress,  60, 	72);
	print_bin_test(DestinationAddress,  135168,	135180);
	print_bin_test(DestinationAddress,  135240,	135252);
	print_bin_test(DestinationAddress2, 60, 	84);
	print_bin_test(DestinationAddress2, 287832, 287844);
	print_bin_test(DestinationAddress2, 287904, 287916);

	umountVol(0); // unmount SD0 emmc

	xil_printf("read_emmc SUCCESS\r\n");
	return XST_SUCCESS;
}


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

	/* interface SD0 vs SD1 on zynqMP PS config
	 * To test logical drive 0, Path should be "0:/"
	 * For logical drive 1, Path should be "1:/"
	 */
	const TCHAR *Path = "1:/";

	//for(BuffCnt = 0; BuffCnt < FileSize; BuffCnt++){
	//	SourceAddress[BuffCnt] = TEST + BuffCnt;
	//}

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
	 * Path - Path to logical driver, 0 - FDISK format.
	 * 0 - Cluster size is automatically determined based on Vol size.
	 */
	 // THIS WILL FORMAT THE DRIVE
	//Res = f_mkfs(Path, &mkfs_parm , work, sizeof work);
	//if (Res != FR_OK) {
	//	return XST_FAILURE;
	//}

	/*
	 * Open file with required permissions.
	 * Here - Creating new file with read/write permissions. .
	 * To open file with write permissions, file system should not
	 * be in Read Only mode.
	 */
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
	//Res = f_lseek(&fil, 0);
	//if (Res) {
	//	return XST_FAILURE;
	//}

	/*
	 * Write data to file.
	 */
	//Res = f_write(&fil, (const void*)SourceAddress, FileSize,
	//		&NumBytesWritten);
	//if (Res) {
	//	return XST_FAILURE;
	//}

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
	if (Res != FR_OK) {
		xil_printf("f_read\r\n");
		return XST_FAILURE;
	}

	/*
	 * Data verification
	 */
	//for(BuffCnt = 0; BuffCnt < FileSize; BuffCnt++){
	//	if(SourceAddress[BuffCnt] != DestinationAddress[BuffCnt]){
	//		
	//		return XST_FAILURE;
	//	}
	//}

	/*
	 * Close file.
	 */
	Res = f_close(&fil);
	if (Res) {
		xil_printf("f_close\r\n");
		return XST_FAILURE;
	}

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
	
	//xil_printf("--2--\n\r");
	//for (Count = 60; Count < 84; Count++) {
	//	xil_printf("RX2(%d) = 0x%02x\n\r",Count,ReadBuffer2[Count]);
	//}
	//xil_printf("--\n\r");
	//for (Count = 287832; Count < 287844; Count++) {
	//	xil_printf("RX2(%d) = 0x%02x\n\r",Count,ReadBuffer2[Count]);
	//}
	//xil_printf("--\n\r");
	//for (Count = 287904; Count < 287916; Count++) {
	//	xil_printf("RX2(%d) = 0x%02x\n\r",Count,ReadBuffer2[Count]);
	//}
	//xil_printf("--\n\r");

	return XST_SUCCESS;
}

static u64 div_round(u64 a, u64 b) {
  return (a + b/2) / b;
}

int prnt_vol_info(int SDx) {
	FRESULT Res;

	const TCHAR *Path = NULL;

	switch (SDx) {
		case 0: 
			Path = "0:/";
			xil_printf("SD0\r\n");
			break;
		case 1: 
			Path = "1:/";
			xil_printf("SD1\r\n");
			break;
		default:
			xil_printf("ERROR: prnt_vol_info %x not valid. 0 or 1 only\r\n", SDx);
			return XST_FAILURE;
	}
	//const TCHAR *Path = "0:/"; // SD0 eMMC

	Res = f_mount(&fatfs, Path, 1);

	if (Res != FR_OK) {
		xil_printf("ERROR: f_mount Res = %x\r\n",Res);
		return XST_FAILURE;
	}

	DWORD fre_clust;
	FATFS *pfatfs;
	Res = f_getfree(Path, &fre_clust, &pfatfs);
	
	if (Res != FR_OK) {
		xil_printf("ERROR f_getfree\n\r");
		return XST_FAILURE;
	}

	DWORD total_clust = pfatfs->n_fatent - 2;
	DWORD sectors_per_clust = pfatfs->csize;
	uint64_t bytes_per_clust = (uint64_t)sectors_per_clust * 512;

	uint64_t total_bytes = (uint64_t)total_clust * bytes_per_clust;
	uint64_t free_bytes  = (uint64_t)fre_clust * bytes_per_clust;
		
	xil_printf("Clusters free: %lu\n\r", fre_clust);
	xil_printf("Cluster size (sectors): %u\n\r", pfatfs->csize);
	xil_printf("total_bytes: %llu\n\r", total_bytes);
	xil_printf("free_bytes: %llu\n\r", free_bytes);
	xil_printf("total_clusters: %lu\n\r", (u32)(pfatfs->n_fatent - 2));
	xil_printf("free_clusters:  %llu\n\r", (u64)(free_bytes / (uint64_t)32768));


	const u64 KiB = 1024ULL;
	const u64 MiB = 1024ULL * KiB;
	const u64 GiB = 1024ULL * MiB;

	xil_printf("\n\rCluster size: %u sectors (%u bytes)\n\r", pfatfs->csize, pfatfs->csize * 512);
	xil_printf("Total: %llu bytes (~%llu GiB)\n\r", total_bytes, div_round(total_bytes, GiB));
	xil_printf("Free : %llu bytes (~%llu GiB)\n\r", free_bytes, div_round(free_bytes, GiB));
	xil_printf("\n\rTotal: %llu bytes (~%llu MiB)\n\r", total_bytes, div_round(total_bytes, MiB));
	xil_printf("Free : %llu bytes (~%llu MiB)\n\r", free_bytes, div_round(free_bytes, MiB));

	list_dir("0:/");

	return XST_SUCCESS;
}

void list_dir(const char *path) {
  FRESULT r;
  DIR dir;
  FILINFO fno;

  r = f_opendir(&dir, path);  // e.g., "0:/", "1:/subdir"
  if (r != FR_OK) {
    xil_printf("f_opendir(%s) failed: %d\n\r", path, r);
    return;
  }

  xil_printf("Listing %s\n\r", path);

  for (;;) {
    r = f_readdir(&dir, &fno);
    if (r != FR_OK || fno.fname[0] == 0) break;  // end

    int is_dir = (fno.fattrib & AM_DIR) != 0;

#if FF_USE_LFN
    // Primary (long if available), and altname is 8.3 alias (may be empty)
    xil_printf("  %s%s  (%lu bytes)",
               fno.fname, is_dir ? "/" : "", (unsigned long)fno.fsize);
    if (fno.altname[0]) xil_printf("  [alias: %s]", fno.altname);
    xil_printf("\n\r");
#else
    xil_printf("  %s%s  (%lu bytes)\n\r",
               fno.fname, is_dir ? "/" : "", (unsigned long)fno.fsize);
#endif
  }

  f_closedir(&dir);
}

int mountVol(int SDx) {
	FRESULT Res;
	const TCHAR *Path = NULL;

	switch (SDx) {
		case 0: 
			Path = "0:/";
			xil_printf("Mounted SD0\r\n");
			break;
		case 1: 
			Path = "1:/";
			xil_printf("Mounted SD1\r\n");
			break;
		default:
			xil_printf("ERROR: prnt_vol_info %x not valid. 0 or 1 only\r\n", SDx);
			return XST_FAILURE;
	}

	Res = f_mount(&fatfs, Path, 1);

	if (Res != FR_OK) {
		xil_printf("ERROR: mountVol Res = %x\r\n",Res);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int umountVol(int SDx) {
	FRESULT Res;
	const TCHAR *Path = NULL;

	switch (SDx) {
		case 0: 
			Path = "0:/";
			xil_printf("Unmounted SD0\r\n");
			break;
		case 1: 
			Path = "1:/";
			xil_printf("Unmounted SD1\r\n");
			break;
		default:
			xil_printf("ERROR: prnt_vol_info %x not valid. 0 or 1 only\r\n", SDx);
			return XST_FAILURE;
	}

	Res = f_mount(NULL, Path, 0);

	if (Res != FR_OK) {
		xil_printf("ERROR: umountVol Res = %x\r\n",Res);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int delFile(const TCHAR *filename) {
	FRESULT r;

	r = f_unlink(filename);
	if (r == FR_OK) {
	  xil_printf("File deleted.\n\r");
	} else {
	  xil_printf("Delete failed: %d\n\r", r);
	  return XST_FAILURE;
	}
	return XST_SUCCESS;
}

/**********************************************************************************************/

#define MIN(a,b) ((a)<(b)?(a):(b))
#define BASE_CHUNK   (256*1024)

static u8 g_buf[BASE_CHUNK] __attribute__((aligned(32)));

static FATFS fs_sd, fs_emmc;

static int pick_chunk(const char *drive) {
  DWORD fre_clust; FATFS *p;
  if (f_getfree(drive, &fre_clust, &p) == FR_OK) {
    /* cluster size in bytes (FF_MIN_SS=FF_MAX_SS=512) */
    u32 cl_bytes = p->csize * 512U;
    /* Use a few clusters per transfer; cap to BASE_CHUNK */
    u32 chunk = (u32)MIN(BASE_CHUNK, cl_bytes * 8U);
    /* keep >= 32 KiB */
    if (chunk < 32*1024U) chunk = 32*1024U;
    return (int)chunk;
  }
  return BASE_CHUNK;
}

static int copy_one(const char *src_path, const char *dst_path, u32 chunk_sz) {
  FIL src, dst;
  FRESULT r;
  UINT rd=0, wr=0;
  u8 *buf = g_buf;  /* no heap allocation */

  r = f_open(&src, src_path, FA_READ);
  if (r != FR_OK) { xil_printf("open src %s fail %d\n\r", src_path, r); return -1; }

  const u32 fsz = f_size(&src);

  r = f_open(&dst, dst_path, FA_CREATE_ALWAYS | FA_WRITE);
  if (r != FR_OK) { xil_printf("open dst %s fail %d\n\r", dst_path, r); f_close(&src); return -1; }

  u32 left = fsz, step = 0;
  while (1) {
    u32 ask = left ? MIN(left, chunk_sz) : chunk_sz;
    r = f_read(&src, buf, ask, &rd);
    if (r != FR_OK) { xil_printf("read fail %d\n\r", r); break; }
    if (rd == 0) break;

    r = f_write(&dst, buf, rd, &wr);
    if (r != FR_OK || wr != rd) { xil_printf("write fail %d wr=%u rd=%u\n\r", r, wr, rd); break; }

    left -= rd;
    step += rd;
    if (step >= 4*1024*1024) { xil_printf("."); step = 0; }
  }

  f_sync(&dst);
  f_close(&dst);
  f_close(&src);

  if (r == FR_OK) { xil_printf(" OK %s -> %s (%lu bytes)\n\r", src_path, dst_path, (unsigned long)fsz); return 0; }
  return -1;
}


int copy_partials_sd1_to_emmc0(void) {
  FRESULT r;
  int chunk_sd = pick_chunk("1:/");
  int chunk_em = pick_chunk("0:/");
  int chunk = (chunk_sd < chunk_em) ? chunk_sd : chunk_em;

  r = f_mount(&fs_emmc, "0:/", 1);
  if (r == FR_NO_FILESYSTEM) {
    xil_printf("No FS on 0:/, mkfs...\n\r");
    MKFS_PARM p = { FM_FAT32, 0, 0, 0, 0 };
    BYTE work[FF_MAX_SS];
    if (f_mkfs("0:/", &p, work, sizeof work) == FR_OK) r = f_mount(&fs_emmc, "0:/", 1);
  }
  if (r != FR_OK) { xil_printf("mount 0:/ fail %d\n\r", r); return -1; }

  r = f_mount(&fs_sd, "1:/", 1);
  if (r != FR_OK) { xil_printf("mount 1:/ fail %d\n\r", r); return -1; }

  xil_printf("Using chunk = %d bytes\n\r", chunk);

  if (copy_one("1:/RM0_RM_led_2_partial.bin", "0:/RM0_RM_led_2_partial.bin", (u32)chunk)) return -1;
  if (copy_one("1:/RM1_RM_led2_2_partial.bin", "0:/RM1_RM_led2_2_partial.bin", (u32)chunk)) return -1;

  xil_printf("\n\rCopy complete.\n\r");
  return 0;
}