// from xfpga_partialbitsream_load_example.c

#include "xilfpgaFunc_adg.h"

int loadPartialBit(UINTPTR bitAddr, u32 bitSize) {
	XFpga XFpgaInstance = {0U};
	UINTPTR KeyAddr = (UINTPTR)NULL;
	s32 Status;

	Status = XFpga_Initialize(&XFpgaInstance);
	if (Status != XST_SUCCESS) {
		goto done;
	}

	//Status = XFpga_BitStream_Load(&XFpgaInstance, addr, KeyAddr, BITSTREAM_SIZE, XFPGA_PARTIAL_EN);
	//Status = XFpga_BitStream_Load(&XFpgaInstance, (UINTPTR)RM1_RM_led2_2_partial_bin, KeyAddr, RM1_RM_led2_2_partial_bin_len, XFPGA_PARTIAL_EN);
	Status = XFpga_BitStream_Load(&XFpgaInstance, bitAddr, KeyAddr, bitSize, XFPGA_PARTIAL_EN);

 done:
	if (Status == XFPGA_SUCCESS)
		xil_printf("Partial Reconfiguration Bitstream loaded into the PL successfully");
	else
		xil_printf("Partial Reconfiguration Bitstream loading into the PL failed\n\r");

	return 0;

}