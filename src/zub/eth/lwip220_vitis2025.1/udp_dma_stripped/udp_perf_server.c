/*
 * Copyright (C) 2018 - 2022 Xilinx, Inc. All rights reserved.
 * Copyright (C) 2022 - 2024 Advanced Micro Devices, Inc.  All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

/** Connection handle for a UDP Server session */

#include "udp_perf_server.h"
#include <xil_printf.h>
#include "xil_cache.h"
#include "xaxidma.h"
#include "xdebug.h"
#include "xparameters.h"

extern struct netif server_netif;
static struct udp_pcb *pcb;
static struct perf_stats server;
/* Report interval in ms */
#define REPORT_INTERVAL_TIME (INTERIM_REPORT_INTERVAL * 20)


#define NUM_DMA_WORDS 200
#define DMA_BUF_SIZE_BYTES (NUM_DMA_WORDS * sizeof(u64))
static u64 dma_buffer[NUM_DMA_WORDS];  // 8-byte aligned by default

XAxiDma AxiDma;

void print_app_header(void)
{
	xil_printf("UDP server listening on port %d\r\n", UDP_CONN_PORT);
}

int init_dma_adg() {
  	XAxiDma_Config *CfgPtr;
  	int Status;

	/* Initialize the XAxiDma device.
	 */
	CfgPtr = XAxiDma_LookupConfig(XPAR_XAXIDMA_0_BASEADDR);
	if (!CfgPtr) {
		xil_printf("No config found for %d\r\n", XPAR_XAXIDMA_0_BASEADDR);
	}

  	Status = XAxiDma_CfgInitialize(&AxiDma, CfgPtr);
  	if (Status != XST_SUCCESS) {
  	  xil_printf("DMA init failed: %d\r\n", Status);
  	  return Status;
  	}

  	// Disable interrupts if polling
  	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
  	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

  	return XST_SUCCESS;
}


void start_my_dma(u32 len) 
{
	int Status;
	
	Xil_DCacheFlushRange((UINTPTR)dma_buffer, len);
	Status = XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR)dma_buffer, len, XAXIDMA_DMA_TO_DEVICE);
	return;
}


/** Receive data on a udp session */
static void udp_recv_perf_traffic(void *arg, struct udp_pcb *tpcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	static u8_t first = 1;
	static u64_t now;
	u32_t drop_datagrams = 0;
	s32_t recv_id;

	memcpy(dma_buffer, p->payload, p->len);
	
	start_my_dma((u32)p->len);	
	
	pbuf_free(p);
	return;
}


void start_application(void)
{
	err_t err;

	/* Create Server PCB */
	pcb = udp_new();
	if (!pcb) {
		xil_printf("UDP server: Error creating PCB. Out of Memory\r\n");
		return;
	}

	err = udp_bind(pcb, IP_ADDR_ANY, UDP_CONN_PORT);
	if (err != ERR_OK) {
		xil_printf("UDP server: Unable to bind to port");
		xil_printf(" %d: err = %d\r\n", UDP_CONN_PORT, err);
		udp_remove(pcb);
		return;
	}

	/* specify callback to use for incoming connections */
	udp_recv(pcb, udp_recv_perf_traffic, NULL);

	return;
}
