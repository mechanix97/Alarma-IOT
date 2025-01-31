#include <Arduino.h>
#include <WiFi.h>

#include "secrets.h"
#include "telegram.h"
#include "alarm.h"
#include "pins.h"
#include "bell.h"

volatile bool telegramScheduler = false;

hw_timer_t* telegramTimer = NULL;

void connectToWiFi();


int offset = 0;

String text;

void IRAM_ATTR telegram_interrupt() {
    telegramScheduler = true;
}

void setup() {
    // Serial.begin(115200);
    alarm_init();
    bell_init();

    telegramTimer = timerBegin(0, 80, true); // Timer 0, divisor 80, contador en modo ascendente
    timerAttachInterrupt(telegramTimer, &telegram_interrupt, true); // Enlazar la función a la interrupción
    timerAlarmWrite(telegramTimer, 10000000, true); // Configurar para 30 segundos (30000000 microsegundos)
    timerAlarmEnable(telegramTimer);
    
    connectToWiFi();    
}

void loop() {
    // bell_ring();
    // execute_intruder_alert();
    
    if( telegramScheduler ){
        telegramScheduler = false;
        String reponse = getTelegramUpdate(offset);
        if (parseTelegramUpdate(reponse, text, offset)){
            offset++;
            parse_text(text);
        }
    }
    
   
}



void connectToWiFi() {
    // Inicia la conexión Wi-Fi
    WiFi.mode(WIFI_STA); // Modo cliente Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Esperar hasta que se conecte al Wi-Fi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}