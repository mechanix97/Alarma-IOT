#include <Arduino.h>
#include <WiFi.h>

#include "secrets.h"
#include "telegram.h"
#include "alarm.h"


void connectToWiFi();


void setup() {
    alarm_init();
    Serial.begin(115200);
    // connectToWiFi();
    // sendTelegramMessage("Hola!!");
}

void loop() {
    turn_on_alarm_siren();
    delay(1000);
    turn_off_alarm_siren();
    delay(1000);
}



void connectToWiFi() {
    // Inicia la conexión Wi-Fi
    WiFi.mode(WIFI_STA); // Modo cliente Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Conectando a Wi-Fi...");

    // Esperar hasta que se conecte al Wi-Fi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConectado a la red Wi-Fi");
}