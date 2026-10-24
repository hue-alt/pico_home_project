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
    cyw43_wifi_pm(&cyw43_state, CYW43_NONE_PM);
    cyw43_arch_wifi_connect_async(WIFI_NAME, PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
    cyw43_arch_lwip_begin();
    udp_client = udp_new();
    cyw43_arch_lwip_end();
    ipaddr_aton(IP_MASTER, &ip_address); //address to network
}

bool data_transmission2(kitchen_data *data){
    if(data == NULL){
        return false;
    } 
    int link_status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    if(link_status != CYW43_LINK_UP){
        static uint32_t last_reconnect = 0;
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if((link_status < 0 || link_status == CYW43_LINK_DOWN) && (now - last_reconnect >= 4000)){
            cyw43_arch_wifi_connect_async(WIFI_NAME, PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
            last_reconnect = now;
        }
        return false;
    }
    cyw43_arch_lwip_begin();
    if(udp_client == NULL){
        udp_client = udp_new();
        if(udp_client == NULL){
            cyw43_arch_lwip_end();
            return false;
        }
    }
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, sizeof(kitchen_data), PBUF_RAM);
    if(p == NULL){
        cyw43_arch_lwip_end();
        return false;
    }
    memcpy(p->payload, data, sizeof(kitchen_data));
    err_t error = udp_sendto(udp_client, p, &ip_address, MASTER_PORT);
    pbuf_free(p);
    cyw43_arch_lwip_end();
    if(error == ERR_OK){
        return true;
    }
    return false;
}


