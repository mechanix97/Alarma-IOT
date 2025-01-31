#ifndef _ALARM_H_
#define _ALARM_H_

#include <Arduino.h>

void alarm_init();
void activate_alarm();
void desactivate_alarm();

void attachInterrupts();

void turn_on_alarm_siren();
void turn_off_alarm_siren();

void execute_intruder_alert();

void sensor_status();
void sensor_config();

void parse_text(String text);

bool get_ds1();
bool get_ds2();
bool get_ms1();
bool get_ms2();
bool get_ms3();

#endif
