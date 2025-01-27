#include "telegram.h"
#include "secrets.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String telegram_send_msg_url = String("https://api.telegram.org/bot") + TELEGRAM_BOT_TOKEN + "/sendMessage";
String telegram_get_upds_url = String("https://api.telegram.org/bot") + TELEGRAM_BOT_TOKEN + "/getUpdates";

void sendTelegramMessage(String message) {
    if (WiFi.status() == WL_CONNECTED) { // Verifica si está conectado a Wi-Fi
        WiFiClientSecure client; // Crea el cliente seguro WiFi para HTTPS
        client.setInsecure();    // No verifica el certificado SSL

        HTTPClient http; // Crea el objeto HTTP

        // Prepara el URL con los parámetros
        String url = telegram_send_msg_url + "?chat_id=" + TELEGRAM_CHAT_ID + "&text=" + message;
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


String getTelegramUpdate(int offset){
    String url = telegram_get_upds_url + "?offset=" + String(offset) + "&limit=1";
    HTTPClient http;          // Crear el cliente HTTP
    http.begin(url);          // Iniciar la conexión a la URL

    int httpResponseCode = http.GET(); // Enviar solicitud GET

    if (httpResponseCode > 0) {
        // Si la respuesta es válida, obtener el cuerpo
        String payload = http.getString();
        http.end(); // Cerrar la conexión
        return payload;
    } else {
        // Si hubo un error en la conexión
        Serial.print("Error en HTTP, código: ");
        Serial.println(httpResponseCode);
        http.end(); // Cerrar la conexión
        return "";
    }
}


bool parseTelegramUpdate(const String& response, String& messageText, int& updateOffset) {
    JsonDocument doc;

    // Analizar el JSON
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        Serial.print("Error al analizar JSON: ");
        Serial.println(error.c_str());
        return false;
    }

    // Verificar que la respuesta sea válida
    if (!doc["ok"]) {
        Serial.println("Respuesta de Telegram no válida.");
        return false;
    }

    // Acceder al array "result"
    JsonArray results = doc["result"].as<JsonArray>();
    if (results.size() == 0) {
        Serial.println("No hay resultados en el JSON.");
        return false;
    }

    // Extraer el primer "update_id" y "text"
    updateOffset = results[0]["update_id"];
    const char* text = results[0]["message"]["text"];

    // Verificar que el texto no sea nulo
    if (text) {
        messageText = String(text); // Asignar el texto al parámetro de salida
        return true;
    } else {
        Serial.println("El mensaje no contiene texto.");
        return false;
    }
}
