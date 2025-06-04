#include "esp_wifi.h"
#include "esp_log.h"
#include "ui.h"
#include "about.h"
#include "logo.h"
#include "success_icon.h"
#include "fail_icon.h"
#include "wifi_0.h"
#include "wifi_1.h"
#include "wifi_2.h"
#include "wifi_3.h"

#define BACKGROUND_COLOR        0x121212ff
#define WHITE_COLOR             0xffffffff
#define BLUE_COLOR              0x0000ffff
#define ORANGE_COLOR            0xffa500ff

void ui_init(void)
{
    wifi_0_info.addr = 0;
    wifi_0_info.loaded_to_ram = false;

    wifi_1_info.addr = wifi_0_info.addr + ((wifi_0_info.size + 3) & ~3U);
    wifi_1_info.loaded_to_ram = false;

    wifi_2_info.addr = wifi_1_info.addr + ((wifi_1_info.size + 3) & ~3U);
    wifi_2_info.loaded_to_ram = false;

    wifi_3_info.addr = wifi_2_info.addr + ((wifi_2_info.size + 3) & ~3U);
    wifi_3_info.loaded_to_ram = false;

    logo_info.addr = wifi_3_info.addr + ((wifi_3_info.size + 3) & ~3U);
    logo_info.loaded_to_ram = false;
    
    success_icon_info.addr = logo_info.addr + ((logo_info.size + 3) & ~3U);
    success_icon_info.loaded_to_ram = false;

    fail_icon_info.addr = success_icon_info.addr + ((success_icon_info.size + 3) & ~3U);
    fail_icon_info.loaded_to_ram = false;

    about_info.addr = fail_icon_info.addr + ((fail_icon_info.size + 3) & ~3U);
    about_info.loaded_to_ram = false;
}

void ui_welcome_screen(void)
{
    bsp_display_ui_start(WHITE_COLOR);
    bsp_display_ui_draw_bitmap((320-280)/2, (240-85)/2, WHITE_COLOR, &logo_info);
    bsp_display_ui_end(1);
}
void ui_wifi_scan_screen(wifi_ap_record_t *ap_info, uint16_t ap_count)
{
    char text[128];

    bsp_display_ui_start(BACKGROUND_COLOR);
    bsp_display_ui_draw_rect(10, 10, 300, 220, 0xddddddff);
    if (ap_info)
    {
        snprintf(text, sizeof(text), "Total APs scanned = %d, List 10 scanned APs:", ap_count);
        bsp_display_ui_draw_text(15, 15, 0xff, 21, 0, text);
        for (int i = 0; i < 10; ++i)
        {
            snprintf(text, sizeof(text), "SSID: %s,", ap_info[i].ssid);
            bsp_display_ui_draw_text(15, 35 + i * 15, 0xff, 20, 0, text);
            snprintf(text, sizeof(text), "RSSI: %d,", ap_info[i].rssi);
            bsp_display_ui_draw_text(170, 35 + i * 15, 0xff, 20, 0, text);
            snprintf(text, sizeof(text), "Channel: %d,", ap_info[i].primary);
            bsp_display_ui_draw_text(240, 35 + i * 15, 0xff, 20, 0, text);
        }
    }
    else
    {
        bsp_display_ui_draw_text(15, 15, 0xff, 21, 0, "Scanning Wifi network...");
    }
    bsp_display_ui_end(1);
}
void ui_wifi_connecting_screen(const char *ssid)
{
    char text[128];
    snprintf(text, sizeof(text), "Connecting to AP %s...", ssid);
    bsp_display_ui_start(BACKGROUND_COLOR);
    bsp_display_ui_draw_rect(10, 10, 300, 220, 0xddddddff);
    bsp_display_ui_draw_text(15, 15, 0xff, 21, 0, text);
    bsp_display_ui_end(1);
}
void ui_wifi_connected_screen(esp_netif_ip_info_t *ip_info, uint8_t wifi_signal)
{
    char text[128];
    bsp_display_ui_start(BACKGROUND_COLOR);
    bsp_display_ui_draw_rect(10, 35, 300, 200, 0xddddddff);
    bsp_display_ui_draw_text(15, 40, 0xff, 21, 0, "Wifi connected.");

    bsp_display_ui_draw_text(15, 60, 0xff, 21, 0, "IPv4 Address");
    snprintf(text, sizeof(text), ": "IPSTR, IP2STR(&ip_info->ip));
    bsp_display_ui_draw_text(140, 60, 0xff, 21, 0, text);

    bsp_display_ui_draw_text(15, 80, 0xff, 21, 0, "Subnet Mask");
    snprintf(text, sizeof(text), ": "IPSTR, IP2STR(&ip_info->netmask));
    bsp_display_ui_draw_text(140, 80, 0xff, 21, 0, text);

    bsp_display_ui_draw_text(15, 100, 0xff, 21, 0, "Default Gateway");
    snprintf(text, sizeof(text), ": "IPSTR, IP2STR(&ip_info->gw));
    bsp_display_ui_draw_text(140, 100, 0xff, 21, 0, text);

    if (wifi_signal < 25)
    {
        bsp_display_ui_draw_bitmap(290, 10, WHITE_COLOR, &wifi_0_info);
    }
    else if (wifi_signal < 50)
    {
        bsp_display_ui_draw_bitmap(290, 10, WHITE_COLOR, &wifi_1_info);
    }
    else if (wifi_signal < 70)
    {
        bsp_display_ui_draw_bitmap(290, 10, WHITE_COLOR, &wifi_2_info);
    }
    else
    {
        bsp_display_ui_draw_bitmap(290, 10, WHITE_COLOR, &wifi_3_info);
    }
    bsp_display_ui_draw_bitmap(5, 5, WHITE_COLOR, &about_info);
    bsp_display_ui_draw_text(30, 12, WHITE_COLOR, 21, 0, "About");
    bsp_display_ui_draw_bitmap(140, 160, WHITE_COLOR, &success_icon_info);
    bsp_display_ui_end(1);
}

void ui_wifi_disconnected_screen(void)
{
    bsp_display_ui_start(BACKGROUND_COLOR);
    bsp_display_ui_draw_rect(10, 35, 300, 200, 0xddddddff);
    bsp_display_ui_draw_text(15, 40, 0xff, 21, 0, "Wifi disconnected.");
    bsp_display_ui_draw_bitmap(5, 5, WHITE_COLOR, &about_info);
    bsp_display_ui_draw_text(30, 12, WHITE_COLOR, 21, 0, "About");
    bsp_display_ui_draw_bitmap(290, 10, WHITE_COLOR, &wifi_0_info);
    bsp_display_ui_draw_bitmap(140, 160, WHITE_COLOR, &fail_icon_info);
    bsp_display_ui_end(1);
}