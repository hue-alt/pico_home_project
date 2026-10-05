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
    (void)arg;
    (void)pcb;
    (void)addr;
    (void)port;
    if (p == NULL) {
        return; 
    }
    uint8_t incoming_id = 0;
    pbuf_copy_partial(p, &incoming_id, sizeof(incoming_id), 0);
    if (incoming_id == STATION_KITCHEN && p->tot_len >= sizeof(kitchen_data)) {
        pbuf_copy_partial(p, &current_kitchen_data, sizeof(kitchen_data), 0);
    } 
    else if (incoming_id == STATION_OUTSIDE && p->tot_len >= sizeof(outside_data)) {
        pbuf_copy_partial(p, &current_outside_data, sizeof(outside_data), 0);
    }
    pbuf_free(p);
}

void data_reception_initialization(void){
    int flag = cyw43_arch_init();
    if(flag != 0){
        return;
    }
    cyw43_arch_enable_ap_mode(WIFI_NAME, PASSWORD, CYW43_AUTH_WPA2_AES_PSK); //hotspot mode
    cyw43_arch_lwip_begin();
    udp_server = udp_new();
    if (udp_server != NULL) {
        udp_bind(udp_server, IP_ADDR_ANY, MASTER_PORT);
        udp_recv(udp_server, receive_callback, NULL);
    }
    cyw43_arch_lwip_end();
}

bool get_kitchen_data(kitchen_data *data) {
    if (data == NULL) {
        return false;
    }
    cyw43_arch_lwip_begin();
    memcpy(data, &current_kitchen_data, sizeof(kitchen_data));
    cyw43_arch_lwip_end();
    return true;
}

bool get_outside_data(outside_data *data) {
    if (data == NULL) {
        return false;
    }
    cyw43_arch_lwip_begin();
    memcpy(data, &current_outside_data, sizeof(outside_data));
    cyw43_arch_lwip_end();
    return true;
}