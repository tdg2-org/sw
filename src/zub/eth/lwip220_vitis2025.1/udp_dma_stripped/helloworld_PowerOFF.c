
#include <stdio.h>
#include "xil_printf.h"
#include "xil_types.h"
#include <xil_io.h>
#include "xparameters.h"
#include "helpFunctions.h"
#include <unistd.h>
#include "bspconfig.h"
#include "xuartps.h"

#include "netif/xadapter.h"
#include "platform.h" 
#include "platform_config.h"
#include "lwipopts.h"
//#include "sleep.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/init.h"
#include "lwip/inet.h"
#include "xil_cache.h"
#include "xaxidma.h"
#include "xdebug.h"


#define BD_REG32_ADDR   PL_REG32_ADDR

extern volatile int TcpFastTmrFlag;
extern volatile int TcpSlowTmrFlag;

#define DEFAULT_IP_ADDRESS	"192.168.1.10"
#define DEFAULT_IP_MASK		"255.255.255.0"
#define DEFAULT_GW_ADDRESS	"192.168.1.1"

static void assign_default_ip(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw);
static void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw);
void platform_enable_interrupts(void);
void start_application(void);
void print_app_header(void);
int init_dma_adg();
static void config_udp_dma();

struct netif server_netif;


int main()
{
    init_platform();

    xil_printf("\n\rtesting adg0000\n\r");
    
    struct netif *netif;
    netif = &server_netif;

    check0();    
    versionCtrl();
    ledCfg();
    config_udp_dma();
    start_application();

    unsigned int val, led=0;
    //xil_printf("  Scripts Git Hash: %0x%0x\n\r",gitMBD,gitLBD);
    //val = Xil_In32(BD_REG32_2_ADDR + 0x4); xil_printf("gh1=%d\r\n",val);
    //Xil_Out32(BD_REG32_2_ADDR + 0x18, 0x7);

    xil_printf("Running...\r\n");
    s8 Ch;
    while (1) {
      //Ch = inbyte();
      //if (Ch == '\r') {
      //    outbyte('\n');
      //}
      if (XUartPs_IsReceiveData(STDIN_BASEADDRESS)) {
        Ch = XUartPs_ReadReg(STDIN_BASEADDRESS, XUARTPS_FIFO_OFFSET);
        outbyte(Ch);
        xil_printf("\r\n");

        if (Ch == 'p') {
          xil_printf("\r\n POWER OFF\r\n");
          usleep(10000);
          powerOff();
        } else if (Ch == 'b') {break;
        } else if (Ch == 'a') {   xil_printf("NULL\r\n");
        } else if (Ch == 'c') {   //val = Xil_In32(BD_REG32_2_ADDR + 0x0); xil_printf("gh0=%d\r\n",val);
        } else if (Ch == 'd') {   
        } else if (Ch == 'e') {   
        } else if (Ch == 'f') {   
        } else if (Ch == 'g') {   
        } else if (Ch == 'h') {   
        } else if (Ch == 'i') {   
        } else if (Ch == 'j') {   
        } else if (Ch == 'k') {   
        } else if (Ch == 'l') {   
        } else if (Ch == 'm') {   
        } else if (Ch == 'n') {   
        } else if (Ch == 'o') {   
        } else if (Ch == 'q') {   
        } else if (Ch == 'r') {   
        } else if (Ch == 's') {   
        } else if (Ch == 't') {

        } else if (Ch == '0') {   ledCfg();
        } else if (Ch == '1') {   ledON();
        } else if (Ch == '2') {   ledOff();
        } else if (Ch == '3') {
        } else if (Ch == '4') {
        } else if (Ch == '5') {
        } else if (Ch == '6') {
        } else if (Ch == '7') {
        } else if (Ch == '8') {
        } else if (Ch == '9') {
        }
      } // if (XUartPs_IsReceiveData(STDIN_BASEADDRESS))
      
      if (TcpFastTmrFlag) {
		  	tcp_fasttmr();
		  	TcpFastTmrFlag = 0;
		  }
		  if (TcpSlowTmrFlag) {
		  	tcp_slowtmr();
		  	TcpSlowTmrFlag = 0;
		  }
		  xemacif_input(netif);

    } // while
    xil_printf("\n\r----------------------------------------\n\r");
    xil_printf("** END **\n\r");
    xil_printf("----------------------------------------\n\r\n\r");

    cleanup_platform();
    
    return 0;
}

/*************************************************************************************************/
//
/*************************************************************************************************/

static void print_ip(char *msg, ip_addr_t *ip)
{
	print(msg);
	xil_printf("%d.%d.%d.%d\r\n", ip4_addr1(ip), ip4_addr2(ip),
			ip4_addr3(ip), ip4_addr4(ip));
}

static void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{
	print_ip("Board IP:       ", ip);
	print_ip("Netmask :       ", mask);
	print_ip("Gateway :       ", gw);
}

static void assign_default_ip(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{
	int err;

	xil_printf("Configuring default IP %s \r\n", DEFAULT_IP_ADDRESS);

	err = inet_aton(DEFAULT_IP_ADDRESS, ip);
	if (!err)		xil_printf("Invalid default IP address: %d\r\n", err);

	err = inet_aton(DEFAULT_IP_MASK, mask);
	if (!err)		xil_printf("Invalid default IP MASK: %d\r\n", err);

	err = inet_aton(DEFAULT_GW_ADDRESS, gw);
	if (!err)		xil_printf("Invalid default gateway address: %d\r\n", err);
}

static void config_udp_dma() 
{
  struct netif *netif;

  /* the mac address of the board. this should be unique per board */
  unsigned char mac_ethernet_address[] = {
  	0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

  netif = &server_netif;

  xil_printf("\r\n\r\n");
  xil_printf("-----lwIP RAW Mode UDP Server Application-----\r\n");

  /* initialize lwIP */
  lwip_init();

  init_dma_adg();

  /* Add network interface to the netif_list, and set it as default */
  if (!xemac_add(netif, NULL, NULL, NULL, mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
  	xil_printf("Error adding N/W interface\r\n");
  	//return -1;
  }
  netif_set_default(netif);

  /* specify that the network if is up */
  netif_set_up(netif);

  assign_default_ip(&(netif->ip_addr), &(netif->netmask), &(netif->gw));

  print_ip_settings(&(netif->ip_addr), &(netif->netmask), &(netif->gw));

  xil_printf("\r\n");

  /* print app header */
  print_app_header();

  return;

}
