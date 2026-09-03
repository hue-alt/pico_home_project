#include <pico/stdlib.h>
#include "pico/cyw43_arch.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "config.h"
#include "data_transmission2.h"
#include <string.h>
#include "lwip/pbuf.h"



static struct udp_pcb *udp_client;
static ip_addr_t ip_address;


void data_transmission2_initialization(void){
    int flag = cyw43_arch_init(); //initialization of the radio system
    if(flag != 0){
        return;
    }
    cyw43_arch_enable_sta_mode(); //client mode
    cyw43_arch_wifi_connect_async(WIFI_NAME, PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
    udp_client = udp_new();
    ipaddr_aton(IP_MASTER, &ip_address); //address to network
}

bool data_transmission2(kitchen_data *data){
    if(udp_client == NULL || data == NULL){ //error flag
        return false;
    } 
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, sizeof(kitchen_data), PBUF_RAM);
    if(p == NULL){
        return false;
    }
    memcpy(p->payload, data, sizeof(kitchen_data));
    err_t error = udp_sendto(udp_client, p, &ip_address, MASTER_PORT);
    pbuf_free(p);
    if(error == ERR_OK){
        return true;
    }
    return false;
}


