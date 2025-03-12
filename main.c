#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"

// Wi-Fi en MQTT instellingen
#define WIFI_SSID "hugonet"
#define WIFI_PASS "6494YsPg1"
#define MQTT_BROKER "10.42.0.1" // Vervang dit door het IP van jouw broker
#define MQTT_PORT 1883

static void mqtt_pub_request_cb(void *arg, err_t err) {
    if (err == ERR_OK) {
        printf("Bericht succesvol gepubliceerd!\n");
    } else {
        printf("Publicatie mislukt, foutcode: %d\n", err);
    }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("Verbonden met MQTT broker!\n");

        const char *topic = "test";
        const char *message = "Hallo van Raspberry Pi Pico!";
        mqtt_publish(client, topic, message, strlen(message), 0, 0, mqtt_pub_request_cb, NULL);
    } else {
        printf("MQTT verbinding mislukt! Status: %d\n", status);
    }
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("Wi-Fi module initialisatie mislukt!\n");
        return -1;
    }

    printf("Verbinding maken met Wi-Fi...\n");
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Wi-Fi verbinding mislukt!\n");
        return -1;
    }
    printf("Verbonden met Wi-Fi!\n");

    // MQTT-client instellen
    mqtt_client_t *client = mqtt_client_new();
    ip_addr_t broker_ip;
    ipaddr_aton(MQTT_BROKER, &broker_ip);
    mqtt_connect(client, &broker_ip, MQTT_PORT, mqtt_connection_cb, NULL, NULL);

    while (true) {
        sleep_ms(5000);
    }

    return 0;
}
