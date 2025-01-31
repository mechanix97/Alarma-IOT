#ifndef _TELEGRAM_H_
#define _TELEGRAM_H_

#include <Arduino.h>

void sendTelegramMessage(String message);
String getTelegramUpdate(int offset);
bool parseTelegramUpdate(const String& response, String& messageText, int& updateOffset);
void sendTelegramKeyboard(String keyboardJson);
void removeTelegramKeyboard();
#endif