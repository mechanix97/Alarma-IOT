#include "alarm.h"
#include <arduino.h>


volatile bool frontDoorSensorActivated = false;
volatile bool backDoorSensorActivated = false;
volatile bool movementrSensor1Activated = false;
volatile bool movementrSensor2Activated = false;
volatile bool movementrSensor3Activated = false;


volatile bool alarmActivated = false;

void IRAM_ATTR handleInterruptfrontDoorSensorActivated() {
    frontDoorSensorActivated = true;
}

void alarm_init(){
    pinMode(FRONT_DOOR_SENSOR_PIN, INPUT);
    pinMode(BACK_DOOR_SENSOR_PIN, INPUT);
    pinMode(MOVEMENT_SENSOR_1_PIN, INPUT);
    pinMode(MOVEMENT_SENSOR_2_PIN, INPUT);
    pinMode(MOVEMENT_SENSOR_3_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(FRONT_DOOR_SENSOR_PIN), handleInterruptfrontDoorSensorActivated, ONLOW);

    pinMode(ALARM_SIREN_PIN, OUTPUT);

    pinMode(LED_PIN, OUTPUT);
}


void activate_alarm(){
    digitalWrite(LED_PIN, HIGH);
    alarmActivated = true;
}

void desactivate_alarm(){
    digitalWrite(LED_PIN, LOW);
    alarmActivated = false;
}

void turn_on_alarm_siren(){
    digitalWrite(ALARM_SIREN_PIN, HIGH);
} 

void turn_off_alarm_siren(){
    digitalWrite(ALARM_SIREN_PIN, LOW);
} 