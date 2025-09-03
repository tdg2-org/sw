
#include "xparameters.h"	/* SDK generated parameters */
#include "xsdps.h"		/* SD device driver */
#include "xil_printf.h"
#include "ff.h"
#include "xil_cache.h"
#include "xplatform_info.h"

#define RM0_LEN 135248
#define RM1_LEN 287912


extern u8 DestinationAddress[RM0_LEN];

int run_SD_read(void);
int prnt_vol_info(int SDx);//0 or 1. SD0 = eMMC, SD1 = SD card
void list_dir(const char *path);
int mountVol(int SDx);
int umountVol(int SDx);
int delFile(const TCHAR *filename);
int copy_partials_sd1_to_emmc0(void);
