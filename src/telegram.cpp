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
    String url = telegram_get_upds_url + "?offset=" + String(offset);

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
    // Crear un objeto JSON dinámico
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(3) + 3 * JSON_OBJECT_SIZE(4) + 200;
    DynamicJsonDocument doc(capacity);

    // Parsear la respuesta
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        Serial.print("Error al parsear JSON: ");
        Serial.println(error.c_str());
        return false;
    }

    // Comprobar si hay resultados en el JSON
    if (!doc["result"].is<JsonArray>() || doc["result"].size() == 0) {
        Serial.println("No hay mensajes en la respuesta.");
        return false;
    }

    // Obtener el primer objeto de la matriz de resultados
    JsonObject firstResult = doc["result"][0];

    // Extraer el update_id y el texto del mensaje
    updateOffset = firstResult["update_id"]; // Offset
    if (firstResult.containsKey("message") && firstResult["message"].containsKey("text")) {
        messageText = firstResult["message"]["text"].as<String>(); // Texto del mensaje
        return true;
    } else {
        Serial.println("No se encontró texto en el mensaje.");
        return false;
    }
}