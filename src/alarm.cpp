#include "alarm.h"
#include <arduino.h>


volatile bool frontDoorSensorActivated = false;
volatile bool backDoorSensorActivated = false;
volatile bool movementSensor1Activated = false;
volatile bool movementSensor2Activated = false;
volatile bool movementSensor3Activated = false;

volatile bool intruderAlert = false;

volatile bool alarmActivated = false;

void IRAM_ATTR handleInterruptfrontDoorSensorActivated() {
    frontDoorSensorActivated = true;
    if(alarmActivated){
        intruderAlert = true;
    }
    detachInterrupt(digitalPinToInterrupt(FRONT_DOOR_SENSOR_PIN));
}

void IRAM_ATTR handleInterruptBackDoorSensorActivated() {
    backDoorSensorActivated = true;
    if(alarmActivated){
        intruderAlert = true;
    }
    detachInterrupt(digitalPinToInterrupt(BACK_DOOR_SENSOR_PIN));
}

void IRAM_ATTR handleInterruptMovementSensor1Activated() {
    movementSensor1Activated = true;
    if(alarmActivated){
        intruderAlert = true;
    }
    detachInterrupt(digitalPinToInterrupt(MOVEMENT_SENSOR_1_PIN));
}

void IRAM_ATTR handleInterruptMovementSensor2Activated() {
    movementSensor2Activated = true;
    if(alarmActivated){
        intruderAlert = true;
    }
    detachInterrupt(digitalPinToInterrupt(MOVEMENT_SENSOR_2_PIN));
}

void IRAM_ATTR handleInterruptMovementSensor3Activated() {
    movementSensor3Activated = true;
    if(alarmActivated){
        intruderAlert = true;
    }
    detachInterrupt(digitalPinToInterrupt(MOVEMENT_SENSOR_3_PIN));
}

void attachInterrupts(){
    attachInterrupt(digitalPinToInterrupt(FRONT_DOOR_SENSOR_PIN), handleInterruptfrontDoorSensorActivated, ONLOW);
    attachInterrupt(digitalPinToInterrupt(BACK_DOOR_SENSOR_PIN), handleInterruptBackDoorSensorActivated, ONLOW);
    attachInterrupt(digitalPinToInterrupt(MOVEMENT_SENSOR_1_PIN), handleInterruptMovementSensor1Activated, ONLOW);
    attachInterrupt(digitalPinToInterrupt(MOVEMENT_SENSOR_2_PIN), handleInterruptMovementSensor2Activated, ONLOW);
    attachInterrupt(digitalPinToInterrupt(MOVEMENT_SENSOR_3_PIN), handleInterruptMovementSensor3Activated, ONLOW);

}

void alarm_init(){
    pinMode(FRONT_DOOR_SENSOR_PIN, INPUT);
    pinMode(BACK_DOOR_SENSOR_PIN, INPUT);
    pinMode(MOVEMENT_SENSOR_1_PIN, INPUT);
    pinMode(MOVEMENT_SENSOR_2_PIN, INPUT);
    pinMode(MOVEMENT_SENSOR_3_PIN, INPUT);
    pinMode(ALARM_SIREN_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    attachInterrupts();
}


void activate_alarm(){
    digitalWrite(LED_PIN, HIGH);
    alarmActivated = true;
    if( frontDoorSensorActivated ||
        backDoorSensorActivated  ||
        movementSensor1Activated ||
        movementSensor2Activated ||
        movementSensor3Activated)
    {
        intruderAlert = true;
    }
}

void desactivate_alarm(){
    digitalWrite(LED_PIN, LOW);
    alarmActivated = false;
    intruderAlert = false;
}

void turn_on_alarm_siren(){
    digitalWrite(ALARM_SIREN_PIN, HIGH);
} 

void turn_off_alarm_siren(){
    digitalWrite(ALARM_SIREN_PIN, LOW);
} 