#include <Arduino.h>
#include <WiFi.h>

#include "secrets.h"
#include "telegram.h"

#define SENSOR_PIN  13  // Pin GPIO al que conectaste el sensor
#define LED_PIN  2


void connectToWiFi();


void setup() {
    pinMode(SENSOR_PIN, INPUT); // Configurar como entrada
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    // connectToWiFi();
    // sendTelegramMessage("Hola!!");
}
bool algo = true;
void loop() {
  int sensorValue = digitalRead(SENSOR_PIN); // Leer estado del sensor
  
  if (sensorValue == HIGH) {
    Serial.println("Puerta cerrada");
  } else {
    Serial.println("Puerta abierta");
  }
//   delay(500); // Esperar un poco antes de leer nuevamente
//   digitalWrite(LED_PIN, algo);
//   algo = !algo;
    // Serial.println(getTelegramUpdate(592280713));
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