#include <pico/stdlib.h>
#include "pico/cyw43_arch.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "config.h"
#include "data_transmission.h"
#include <string.h>
#include "lwip/pbuf.h"



static struct udp_pcb *udp_client;
static ip_addr_t ip_address;


void data_transmission_initialization(void){
    int flag = cyw43_arch_init(); //initialization of the radio system
    if(flag != 0){
        return;
    }
    cyw43_arch_enable_sta_mode(); //client mode
    cyw43_arch_wifi_connect_async(WIFI_NAME, PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
    cyw43_arch_lwip_begin();
    udp_client = udp_new();
    cyw43_arch_lwip_end();
    ipaddr_aton(IP_MASTER, &ip_address); //address to network
}

bool data_transmission(outside_station *data){
    if(udp_client == NULL || data == NULL){ //error flag
        return false;
    } 
    int link_status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    if(link_status != CYW43_LINK_UP){
        cyw43_arch_wifi_connect_async(WIFI_NAME, PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
        return false;
    }
    cyw43_arch_lwip_begin();
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, sizeof(outside_station), PBUF_RAM);
    if(p == NULL){
        cyw43_arch_lwip_end();
        return false;
    }
    memcpy(p->payload, data, sizeof(outside_station));
    err_t error = udp_sendto(udp_client, p, &ip_address, MASTER_PORT);
    pbuf_free(p);
    cyw43_arch_lwip_end();
    if(error == ERR_OK){
        return true;
    }
    return false;
}


