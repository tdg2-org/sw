
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
