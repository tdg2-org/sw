
this is stripped from udp perf server example from xilinx, with my main
jumbo frames, DMA to PL

Make sure to check UserConfig.cmake
udp_perf_server.c must be listed in 'set(USER_COMPILE_SOURCES'
