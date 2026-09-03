#include "config.h"
#include "pico/stdlib.h"
#include "datareception.h"
#include "pico/cyw43_arch.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include <string.h>



struct udp_pcb *udp_server;

static kitchen_data current_kitchen_data;
static outside_data current_outside_data;

// Network callback function invoked automatically in the background by lwIP
static void receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port) {
    if (p == NULL) {
        return; 
    }
    uint8_t incoming_id = *((uint8_t *)p->payload);
    if (incoming_id == STATION_KITCHEN && p->tot_len == sizeof(kitchen_data)) {
        memcpy(&current_kitchen_data, p->payload, sizeof(kitchen_data));
    } 
    else if (incoming_id == STATION_OUTSIDE && p->tot_len == sizeof(outside_data)) {
        memcpy(&current_outside_data, p->payload, sizeof(outside_data));
    }
    pbuf_free(p);
}

void data_reception_initialization(void){
    int flag = cyw43_arch_init();
    if(flag != 0){
        return;
    }
    cyw43_arch_enable_ap_mode(WIFI_NAME, PASSWORD, CYW43_AUTH_WPA2_AES_PSK); //hotspot mode
    udp_server = udp_new();
    udp_bind(udp_server, IP_ADDR_ANY, MASTER_PORT);
    udp_recv(udp_server, receive_callback, NULL);
}

bool get_kitchen_data(kitchen_data *data) {
    if (data == NULL) {
        return false;
    }
    memcpy(data, &current_kitchen_data, sizeof(kitchen_data));
    return true;
}

bool get_outside_data(outside_data *data) {
    if (data == NULL) {
        return false;
    }
    memcpy(data, &current_outside_data, sizeof(outside_data));
    return true;
}