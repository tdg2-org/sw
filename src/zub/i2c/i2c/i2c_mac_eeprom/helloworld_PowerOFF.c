/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h" 
#include "xil_printf.h"
#include "xil_types.h"
#include <xil_io.h>
#include "xparameters.h"
#include "helpFunctions.h"
//#include <unistd.h> //incompatible with sleep.h

#include "sleep.h"  //incompatible with unistd.h
#include "xiicps.h"

#define BD_REG32_ADDR   XPAR_AXIL_REG32_0_BASEADDR

//#define I2C_BASEADDR XPAR_XIICPS_0_BASEADDR // EMIO test
//#define I2C_BASEADDR XPAR_XIICPS_1_BASEADDR // actual device
#define XIICPS_BASEADDRESS	XPAR_XIICPS_1_BASEADDR
#define IIC_SLAVE_ADDR		0x50
#define IIC_SLAVE_ADDR_EUI  0x58
#define IIC_SCLK_RATE		100000

#define TEST_BUFFER_SIZE	16

int IicPsMasterPolledExample(UINTPTR BaseAddress);
int IicPsMasterSend(UINTPTR BaseAddress);
int IicPsMasterRecv(UINTPTR BaseAddress);
int IicPsMasterReadEUI();
int IicPsMasterReadSN();
int IicPsMasterConfig(UINTPTR BaseAddress);
int IicPsMasterRead(u8 memAddr, s32 readBytes);
int IicPsMasterWrite(u8 memAddr, s32 readBytes);

XIicPs Iic;		/**< Instance of the IIC Device */

u8 SendBuffer[TEST_BUFFER_SIZE];    /**< Buffer for Transmitting Data */
u8 RecvBuffer[TEST_BUFFER_SIZE];    /**< Buffer for Receiving Data */


int main()
{
    init_platform();

    xil_printf("\n\rtesting adg0000\n\r");
    unsigned int val, Status;
    check0();    
    versionCtrl();
    Xil_Out32(BD_REG32_ADDR + 0x18, 0xF);

    //val = Xil_In32(XPAR_XIICPS_0_BASEADDR + 0x4); xil_printf("val=%x\r\n",val); 
    //val = Xil_In32(XPAR_XIICPS_1_BASEADDR + 0x4); xil_printf("val=%x\r\n",val); 
    
    /*********************************************************************************************/
    Status = IicPsMasterConfig(XIICPS_BASEADDRESS);if (Status != XST_SUCCESS) {xil_printf("INIT FAIL\r\n");}
    Status = IicPsMasterReadEUI();if (Status != XST_SUCCESS) {xil_printf("EUI FAIL\r\n");}
    Status = IicPsMasterReadSN();if (Status != XST_SUCCESS) {xil_printf("SN FAIL\r\n");}
    
    /*********************************************************************************************/
    xil_printf("Running...\r\n");
    s8 Ch;
    while (1) {
      Ch = inbyte();
      if (Ch == '\r') {
          outbyte('\n');
      }
      outbyte(Ch);
      xil_printf("\r\n");

      if (Ch == 'p') {
        xil_printf("\r\n POWER OFF\r\n");
        usleep(10000);
        powerOff();
      } else if (Ch == 'b') {break;
      } else if (Ch == 'a') {   xil_printf("NULL\r\n");
      } else if (Ch == 'c') {   //val = Xil_In32(BD_REG32_2_ADDR + 0x0); xil_printf("gh0=%d\r\n",val);
      } else if (Ch == 'd') {   Status = IicPsMasterPolledExample(XIICPS_BASEADDRESS);
      } else if (Ch == 'e') {   
      } else if (Ch == 'f') {   
      } else if (Ch == 'g') {   
      } else if (Ch == 'h') {   Status = IicPsMasterReadEUI();
      } else if (Ch == 'i') {   Status = IicPsMasterReadSN();
      } else if (Ch == 'j') {   Status = IicPsMasterRead(0, 4);
      } else if (Ch == 'k') {   Status = IicPsMasterWrite(0, 4);
      } else if (Ch == 'l') {   
      } else if (Ch == 'm') {   
      } else if (Ch == 'n') {   
      } else if (Ch == 'o') {   
      } else if (Ch == 'q') {   Xil_Out32(BD_REG32_ADDR + 0x18, 0x7);
      } else if (Ch == 'r') {   Xil_Out32(BD_REG32_ADDR + 0x18, 0x9);
      } else if (Ch == 's') {   Xil_Out32(BD_REG32_ADDR + 0x18, 0xB);
      } else if (Ch == 't') {   Xil_Out32(BD_REG32_ADDR + 0x18, 0xD);
        //val = Xil_In32(BD_REG32_2_ADDR + 0x0); xil_printf("gh0=%d\r\n",val);
        //val = Xil_In32(BD_REG32_2_ADDR + 0xC); xil_printf("const=%x\r\n",val);
        //Xil_Out32(BD_REG32_2_ADDR + 0x1C, 0x5);
      }
      //} else if (Ch == '0') {Xil_Out32(BD_REG32_ADDR + 0x2C, 0x0);
      //}
    }
    xil_printf("\n\r----------------------------------------\n\r");
    xil_printf("** END **\n\r");
    xil_printf("----------------------------------------\n\r\n\r");

    cleanup_platform();
    
    return 0;
}
/*****************************************************************************/
/*****************************************************************************/
// mem addressing 0x00-0xFF, 256x8bits = 2048bits
int IicPsMasterWrite(u8 memAddr, s32 readBytes)
{
	int Status;
    s32 numBytes;
    u16 devAddr;

    // TX
    SendBuffer[0] = memAddr;   // mem addr
    SendBuffer[1] = 0x34;   // data
    SendBuffer[2] = 0x5a;   // data
    SendBuffer[3] = 0x6b;   // data
    SendBuffer[4] = 0x7c;   // data

    numBytes = readBytes + 1;   //
    devAddr = IIC_SLAVE_ADDR;

	Status = XIicPs_MasterSendPolled(&Iic, SendBuffer, numBytes, devAddr); 
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

    while (XIicPs_BusIsBusy(&Iic)) {/* NOP */}

	return XST_SUCCESS;
}

// mem addressing 0x00-0xFF, 256x8bits = 2048bits
int IicPsMasterRead(u8 memAddr, s32 readBytes)
{
	int Status;
	int Index;
    s32 numBytes;
    u16 devAddr;

    // TX
    SendBuffer[0] = memAddr;   // mem addr

    numBytes = 1;   //first send 1byte mem address
    devAddr = IIC_SLAVE_ADDR;

	Status = XIicPs_MasterSendPolled(&Iic, SendBuffer, numBytes, devAddr); 
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

    while (XIicPs_BusIsBusy(&Iic)) {/* NOP */}

    // RX
	for (Index = 0; Index < readBytes; Index++) {
		RecvBuffer[Index] = 0;
	}

    numBytes = readBytes;

	Status = XIicPs_MasterRecvPolled(&Iic, RecvBuffer, numBytes, devAddr);
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

    // print data
    for (Index = 0; Index < numBytes; Index++) {
        Status = RecvBuffer[Index]; xil_printf("read%x=%x\r\n",Index,Status);
    }

	while (XIicPs_BusIsBusy(&Iic)) {/* NOP */}

	return XST_SUCCESS;
}

// SN = 0x0A908504841004613C1DA000A0000000
int IicPsMasterReadSN()
{
	int Status;
	int Index;
    s32 numBytes;
    u16 devAddr;

    // TX
    SendBuffer[0] = 0x80;   // mem addr

    numBytes = 1;
    devAddr = IIC_SLAVE_ADDR_EUI;

	Status = XIicPs_MasterSendPolled(&Iic, SendBuffer, numBytes, devAddr);
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

    while (XIicPs_BusIsBusy(&Iic)) {/* NOP */}

    // RX
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		RecvBuffer[Index] = 0;
	}

    numBytes = 16;

	Status = XIicPs_MasterRecvPolled(&Iic, RecvBuffer, numBytes, devAddr);
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

    // print data
    //for (Index = 0; Index < numBytes; Index++) {
    //    Status = RecvBuffer[Index]; xil_printf("read%x=%x\r\n",Index,Status);
    //}

    const u32 *val32 = (const u32 *)RecvBuffer;

    xil_printf("SN = 0x%08x%08x%08x%08x\r\n",
        __builtin_bswap32(val32[0]),
        __builtin_bswap32(val32[1]),
        __builtin_bswap32(val32[2]),
        __builtin_bswap32(val32[3]));

	while (XIicPs_BusIsBusy(&Iic)) {/* NOP */}

	return XST_SUCCESS;
}

// EUI = 0xFCC23Dxxxxxx     ->  0xFCC23D2771BF
int IicPsMasterReadEUI()
{
	int Status;
	int Index;
    s32 numBytes;
    u16 devAddr;

    // TX
    SendBuffer[0] = 0x9a;   // mem addr

    numBytes = 1;
    devAddr = IIC_SLAVE_ADDR_EUI;

	Status = XIicPs_MasterSendPolled(&Iic, SendBuffer, numBytes, devAddr);
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

    while (XIicPs_BusIsBusy(&Iic)) {/* NOP */}

    // RX
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		RecvBuffer[Index] = 0;
	}

    numBytes = 6;

	Status = XIicPs_MasterRecvPolled(&Iic, RecvBuffer, numBytes, devAddr);
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

    // print data
    //for (Index = 0; Index < numBytes; Index++) {
    //    Status = RecvBuffer[Index]; xil_printf("read%x=%x\r\n",Index,Status);
    //}

    //const u8 *val8 = (const u8 *)RecvBuffer;
    //xil_printf("EUI = 0x%02x%02x%02x%02x%02x%02x\r\n",val8[5],val8[4],val8[3],val8[2],val8[1],val8[0]);

    const u16 *val16 = (const u16 *)RecvBuffer;
    xil_printf("EUI = 0x%04x%04x%04x\r\n",
        __builtin_bswap16(val16[0]),
        __builtin_bswap16(val16[1]),
        __builtin_bswap16(val16[2]));

	while (XIicPs_BusIsBusy(&Iic)) {/* NOP */}

	return XST_SUCCESS;
}

int IicPsMasterConfig(UINTPTR BaseAddress)
{
	int Status;
	XIicPs_Config *Config;

    // init driver
	Config = XIicPs_LookupConfig(BaseAddress);
	if (NULL == Config) {return XST_FAILURE;}

	Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

    // reset
    XIicPs_Reset(&Iic);

    // self test
	Status = XIicPs_SelfTest(&Iic);
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

	XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);

	return XST_SUCCESS;
}

/*****************************************************************************/
/*****************************************************************************/
int IicPsMasterPolledExample(UINTPTR BaseAddress)
{
	int Status,val;
	XIicPs_Config *Config;
	int Index;

	/*
	 * Initialize the IIC driver so that it's ready to use
	 * Look up the configuration in the config table,
	 * then initialize it.
	 */
	Config = XIicPs_LookupConfig(BaseAddress);

	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

    XIicPs_Reset(&Iic);
    val = Xil_In32(XPAR_XIICPS_0_BASEADDR + 0x4); xil_printf("val=%x\r\n",val); 


	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XIicPs_SelfTest(&Iic);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);

	/*
	 * Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero to allow the receive data to be
	 * verified.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		//SendBuffer[Index] = (Index % TEST_BUFFER_SIZE);
		RecvBuffer[Index] = 0;
	}
        SendBuffer[0] = 0x7a; // mem addr
        SendBuffer[1] = 0xA5; // data
        SendBuffer[2] = 0xB6; // data
        //SendBuffer[3] = 0xC7; // data

	/*
	 * Send the buffer using the IIC and ignore the number of bytes sent
	 * as the return value since we are using it in interrupt mode.
	 */
	Status = XIicPs_MasterSendPolled(&Iic, SendBuffer,
					 TEST_BUFFER_SIZE, IIC_SLAVE_ADDR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait until bus is idle to start another transfer.
	 */
	while (XIicPs_BusIsBusy(&Iic)) {
		/* NOP */
	}

	Status = XIicPs_MasterRecvPolled(&Iic, RecvBuffer,
					 TEST_BUFFER_SIZE, IIC_SLAVE_ADDR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Verify received data is correct.
	 */
    for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
        Status = RecvBuffer[Index]; xil_printf("read%x=%x\r\n",Index,Status);
    }

    //for (Index = 0; Index < TEST_BUFFER_SIZE; Index ++) {
    //    
	//	/* Aardvark as slave can only set 64 bytes for output */
	//	if (RecvBuffer[Index] != Index % 64) {
	//		return XST_FAILURE;
	//	}
	//}

	while (XIicPs_BusIsBusy(&Iic)) {
		/* NOP */
	}

	return XST_SUCCESS;
}

int IicPsMasterSend(UINTPTR BaseAddress)
{
	int Status,val;
	XIicPs_Config *Config;
	int Index;

	/*
	 * Initialize the IIC driver so that it's ready to use
	 * Look up the configuration in the config table,
	 * then initialize it.
	 */
	Config = XIicPs_LookupConfig(BaseAddress);

	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

    XIicPs_Reset(&Iic);
    val = Xil_In32(XPAR_XIICPS_0_BASEADDR + 0x4); xil_printf("val=%x\r\n",val); 


	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XIicPs_SelfTest(&Iic);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);

	/*
	 * Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero to allow the receive data to be
	 * verified.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		//SendBuffer[Index] = (Index % TEST_BUFFER_SIZE);
		RecvBuffer[Index] = 0;
	}
        SendBuffer[0] = 0x7a; // mem addr
        SendBuffer[1] = 0xA5; // data
        SendBuffer[2] = 0xB6; // data
        //SendBuffer[3] = 0xC7; // data

	/*
	 * Send the buffer using the IIC and ignore the number of bytes sent
	 * as the return value since we are using it in interrupt mode.
	 */
	Status = XIicPs_MasterSendPolled(&Iic, SendBuffer,
					 TEST_BUFFER_SIZE, IIC_SLAVE_ADDR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait until bus is idle to start another transfer.
	 */
	while (XIicPs_BusIsBusy(&Iic)) {
		/* NOP */
	}

	return XST_SUCCESS;
}

int IicPsMasterRecv(UINTPTR BaseAddress)
{
	int Status,val;
	XIicPs_Config *Config;
	int Index;

	/*
	 * Initialize the IIC driver so that it's ready to use
	 * Look up the configuration in the config table,
	 * then initialize it.
	 */
	Config = XIicPs_LookupConfig(BaseAddress);

	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

    XIicPs_Reset(&Iic);
    val = Xil_In32(XPAR_XIICPS_0_BASEADDR + 0x4); xil_printf("val=%x\r\n",val); 


	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XIicPs_SelfTest(&Iic);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);

	/*
	 * Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero to allow the receive data to be
	 * verified.
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		RecvBuffer[Index] = 0;
	}

	Status = XIicPs_MasterRecvPolled(&Iic, RecvBuffer,
					 TEST_BUFFER_SIZE, IIC_SLAVE_ADDR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Verify received data is correct.
	 */
    for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
        Status = RecvBuffer[Index]; xil_printf("read%x=%x\r\n",Index,Status);
    }

	while (XIicPs_BusIsBusy(&Iic)) {
		/* NOP */
	}

	return XST_SUCCESS;
}
