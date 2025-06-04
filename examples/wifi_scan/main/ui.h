#ifndef UI_HDR_H_
#define UI_HDR_H_

#include "bsp_display.h"

void ui_init(void);
void ui_welcome_screen(void);
void ui_wifi_scan_screen(wifi_ap_record_t *ap_info, uint16_t ap_count);
void ui_wifi_connecting_screen(const char *ssid);
void ui_wifi_connected_screen(esp_netif_ip_info_t *ip_info, uint8_t wifi_signal);
void ui_wifi_disconnected_screen(void);

#endif /* UI_HDR_H_ */
