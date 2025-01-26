#include "alarm.h"
#include <arduino.h>

void alarm_init(){
    pinMode(FRONT_DOOR_SENSOR_PIN, INPUT);
    pinMode(BACK_DOOR_SENSOR_PIN, INPUT);
    pinMode(MOVEMENT_SENSOR_1_PIN, INPUT);
    pinMode(MOVEMENT_SENSOR_2_PIN, INPUT);
    pinMode(MOVEMENT_SENSOR_3_PIN, INPUT);

    pinMode(ALARM_SIREN_PIN, OUTPUT);

    pinMode(LED_PIN, OUTPUT);
}


void activate_alarm(){
    digitalWrite(LED_PIN, HIGH);
}

void desactivate_alarm(){
    digitalWrite(LED_PIN, LOW);
}

void turn_on_alarm_siren(){
    digitalWrite(ALARM_SIREN_PIN, HIGH);
} 

void turn_off_alarm_siren(){
    digitalWrite(ALARM_SIREN_PIN, LOW);
} 