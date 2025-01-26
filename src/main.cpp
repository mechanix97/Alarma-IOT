#include <Arduino.h>
#include <WiFi.h>

#include "secrets.h"
#include "telegram.h"
#include "alarm.h"


extern bool frontDoorSensorActivated;
extern bool backDoorSensorActivated;
extern bool movementrSensor1Activated;
extern bool movementrSensor2Activated;
extern bool movementrSensor3Activated;

extern bool alarmActivated;

void connectToWiFi();


int offset = 0;

String text;

void setup() {
    alarm_init();
    Serial.begin(115200);
    connectToWiFi();
    // sendTelegramMessage("Hola!!");
}

void loop() {
    // if(alarmActivated && (
    //     frontDoorSensorActivated  ||
    //     backDoorSensorActivated   ||
    //     movementrSensor1Activated ||
    //     movementrSensor2Activated ||
    //     movementrSensor3Activated)
    // ){
    //     turn_on_alarm_siren();
    // }
    String reponse = getTelegramUpdate(offset);
    if (parseTelegramUpdate(reponse, text, offset)){
        Serial.println(text);
        offset++;
        if (text == "\\ALARMAON"){
            activate_alarm();
        } else if (text == "\\ALARMAOFF"){
            desactivate_alarm();
        }
    }

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