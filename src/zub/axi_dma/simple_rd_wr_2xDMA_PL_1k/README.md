### Two simple DMAs

1. PL <-> DDR
  - e: dma2PL(64*8);  // send 64*8 bytes to PL
  - h: dma2PL(256*8); // send 256*8 bytes to PL
  - d: testDMA_READ(); // receive 256*8 bytes from PL and print (PL axis stimulus is configured for 256 64-bit words)

2. PL <-> PL BRAM
  - j: reg_write_field(BD_REG32_ADDR, 0x78, 24, 8, 0x1F);                   // config PL axis stimulus for 32 words (1024bits each)
  - k: val = Xil_In32(BD_REG32_ADDR + 0x78); xil_printf("val=%x\r\n",val);  // verify 
  - l: PL_2_BRAM_S2MM(32*128);                                              // DMA write stimulus to BRAM 32*128 bytes (32*1024-bit words)
  - m: dmaFrom_BRAM(32*128);                                                // DMA read from BRAM into PL