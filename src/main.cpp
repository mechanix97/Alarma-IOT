#include <Arduino.h>
#include <WiFi.h>

#include "secrets.h"
#include "telegram.h"
#include "alarm.h"


extern bool frontDoorSensorActivated;
extern bool backDoorSensorActivated;
extern bool movementSensor1Activated;
extern bool movementSensor2Activated;
extern bool movementSensor3Activated;

extern bool alarmActivated;

extern bool intruderAlert;


volatile bool telegramScheduler = false;

hw_timer_t* telegramTimer = NULL;

void connectToWiFi();


int offset = 0;

String text;

void IRAM_ATTR telegram_interrupt() {
//   Serial.println("Interrupción ejecutada");
  telegramScheduler = true;
}

void setup() {
    alarm_init();
    // Serial.begin(115200);
    

    telegramTimer = timerBegin(0, 80, true); // Timer 0, divisor 80, contador en modo ascendente
    timerAttachInterrupt(telegramTimer, &telegram_interrupt, true); // Enlazar la función a la interrupción
    timerAlarmWrite(telegramTimer, 10000000, true); // Configurar para 30 segundos (30000000 microsegundos)
    timerAlarmEnable(telegramTimer);

    connectToWiFi();    
}

void loop() {
    if(intruderAlert){
        turn_on_alarm_siren();
        sendTelegramMessage("🚨 ALERTA INTRUSO 🚨");
        if (frontDoorSensorActivated) {
            sendTelegramMessage("PUERTA DELANTERA ABIERTA 🚪");
        } 
        if (backDoorSensorActivated) {
            sendTelegramMessage("PUERTA TRASERA ABIERTA 🚪");
        } 
        if (movementSensor1Activated) {
            sendTelegramMessage("MOMIMIENTO DETECTADO 🏃🏿‍♂️ SENSOR 1");
        } 
        if (movementSensor2Activated) {
            sendTelegramMessage("MOMIMIENTO DETECTADO 🏃🏿‍♂️ SENSOR 2");
        } 
        if (movementSensor3Activated) {
            sendTelegramMessage("MOMIMIENTO DETECTADO 🏃🏿‍♂️ SENSOR 3");
        }

        frontDoorSensorActivated  = false;
        backDoorSensorActivated   = false;
        movementSensor1Activated = false;
        movementSensor2Activated = false;
        movementSensor3Activated = false;
        attachInterrupts();
    }else{
        turn_off_alarm_siren();
    }
    if( telegramScheduler ){
        telegramScheduler = false;
        String reponse = getTelegramUpdate(offset);
        if (parseTelegramUpdate(reponse, text, offset)){
            offset++;
            if (text == "/alarma_on"){
                activate_alarm();
                sendTelegramMessage("Alarma encendida 🔒");
            } else if (text == "/alarma_off"){
                desactivate_alarm();
                turn_off_alarm_siren();
                sendTelegramMessage("Alarma apagada 🔓");
            }
        }
    }
    
   
}



void connectToWiFi() {
    // Inicia la conexión Wi-Fi
    WiFi.mode(WIFI_STA); // Modo cliente Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Serial.print("Conectando a Wi-Fi...");

    // Esperar hasta que se conecte al Wi-Fi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        // Serial.print(".");
    }

    // Serial.println("\nConectado a la red Wi-Fi");
}