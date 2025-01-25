#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>


#include "secrets.h"

#define SENSOR_PIN  4  // Pin GPIO al que conectaste el sensor

String telegram_api_url = "https://api.telegram.org/bot" + telegram_bot_token + "/sendMessage";

void connectToWiFi();
void sendTelegramMessage(String message);

void setup() {
    pinMode(SENSOR_PIN, INPUT); // Configurar como entrada
    Serial.begin(115200);
    connectToWiFi();
    sendTelegramMessage("Hola!!");
}

void loop() {
  int sensorValue = digitalRead(SENSOR_PIN); // Leer estado del sensor
  if (sensorValue == HIGH) {
    Serial.println("Puerta cerrada");
  } else {
    Serial.println("Puerta abierta");
  }
  delay(500); // Esperar un poco antes de leer nuevamente
}


void sendTelegramMessage(String message) {
    if (WiFi.status() == WL_CONNECTED) { // Verifica si está conectado a Wi-Fi
        WiFiClientSecure client; // Crea el cliente seguro WiFi para HTTPS
        client.setInsecure();    // No verifica el certificado SSL

        HTTPClient http; // Crea el objeto HTTP

        // Prepara el URL con los parámetros
        String url = telegram_api_url + "?chat_id=" + telegram_chat_id + "&text=" + message;
        http.begin(client, url); // Inicia la conexión con la URL usando WiFiClientSecure

        // Envía la solicitud GET
        int httpCode = http.GET();

        // Verifica el código de respuesta de la solicitud
        if (httpCode > 0) {
            String payload = http.getString(); // Respuesta del servidor
        Serial.println("Mensaje enviado correctamente a Telegram");
        Serial.println(payload); // Muestra la respuesta
        } 
        else {
          Serial.println("Error al enviar mensaje a Telegram, Código HTTP: " + String(httpCode));
        }

        http.end(); // Cierra la conexión
    } else {
        Serial.println("Error de conexión Wi-Fi");
    }
}


void connectToWiFi() {
    // Inicia la conexión Wi-Fi
    WiFi.mode(WIFI_STA); // Modo cliente Wi-Fi
    WiFi.begin(wifi_ssid, wifi_password);

    Serial.print("Conectando a Wi-Fi...");

    // Esperar hasta que se conecte al Wi-Fi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConectado a la red Wi-Fi");
}