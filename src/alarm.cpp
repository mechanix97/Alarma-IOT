#include "alarm.h"
#include "pins.h"
#include "telegram.h"

#include <arduino.h>


volatile bool frontDoorSensorActivated = false;
volatile bool backDoorSensorActivated = false;
volatile bool movementSensor1Activated = false;
volatile bool movementSensor2Activated = false;
volatile bool movementSensor3Activated = false;

volatile bool intruderAlert = false;

volatile bool alarmActivated = false;

extern int offset;

bool ds1Enabled = true;
bool ds2Enabled = true;
bool ms1Enabled = true;
bool ms2Enabled = true;
bool ms3Enabled = true;

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
    if (ds1Enabled){
        attachInterrupt(digitalPinToInterrupt(FRONT_DOOR_SENSOR_PIN), handleInterruptfrontDoorSensorActivated, ONLOW);
    }
    if (ds2Enabled){
        attachInterrupt(digitalPinToInterrupt(BACK_DOOR_SENSOR_PIN), handleInterruptBackDoorSensorActivated, ONLOW);
    }
    if (ms1Enabled){
        attachInterrupt(digitalPinToInterrupt(MOVEMENT_SENSOR_1_PIN), handleInterruptMovementSensor1Activated, ONLOW);
    }
    if (ms2Enabled){
        attachInterrupt(digitalPinToInterrupt(MOVEMENT_SENSOR_2_PIN), handleInterruptMovementSensor2Activated, ONLOW);
    }
    if (ms3Enabled){
        attachInterrupt(digitalPinToInterrupt(MOVEMENT_SENSOR_3_PIN), handleInterruptMovementSensor3Activated, ONLOW);
    }
}

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
    attachInterrupts();
    if( frontDoorSensorActivated ||
        backDoorSensorActivated  ||
        movementSensor1Activated ||
        movementSensor2Activated ||
        movementSensor3Activated)
    {
        sendTelegramMessage("❗️No se pudo activar la alarma%0AHay sensores activados");
    } else {
        digitalWrite(LED_PIN, HIGH);
        sendTelegramMessage("Alarma encendida 🔒");
        alarmActivated = true;
    }
}

void desactivate_alarm(){
    turn_off_alarm_siren();
    sendTelegramMessage("Alarma apagada 🔓");
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

void execute_intruder_alert(){
    if(! intruderAlert){
        turn_off_alarm_siren();
        return;
    }
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
}



void parse_text(String text){
    if (text == "/alarma_on"){
        activate_alarm();
    } else if (text == "/alarma_off"){
        desactivate_alarm();
    } else if (text == "/status"){
        sensor_status();
    } else if (text == "/config"){
        sensor_config();
    }
}

void sensor_status(){
    String mensaje = "**Estado de los sensores:**%0A";
    mensaje += "Sensor Puerta Delantera: " + String(get_ds1() ? "habilitado ✅" : "deshabilitado ❌") + "%0A";
    mensaje += "Sensor Puerta Trasera: " + String(get_ds2() ? "habilitado ✅" : "deshabilitado ❌") + "%0A";
    mensaje += "Sensor Movimiento 1: " + String(get_ms1() ? "habilitado ✅" : "deshabilitado ❌") + "%0A";
    mensaje += "Sensor Movimiento 2: " + String(get_ms2() ? "habilitado ✅" : "deshabilitado ❌") + "%0A";
    mensaje += "Sensor Movimiento 3: " + String(get_ms3() ? "habilitado ✅" : "deshabilitado ❌");

    sendTelegramMessage(mensaje);
}


void sensor_config(){
    
    // String mensaje = "**Estado de los sensores:**%0A";
    // mensaje += "Sensor Puerta Delantera: " + String(get_ds1() ? "habilitado ✅" : "deshabilitado ❌") + "%0A";
    // mensaje += "Sensor Puerta Trasera: " + String(get_ds2() ? "habilitado ✅" : "deshabilitado ❌") + "%0A";
    // mensaje += "Sensor Movimiento 1: " + String(get_ms1() ? "habilitado ✅" : "deshabilitado ❌") + "%0A";
    // mensaje += "Sensor Movimiento 2: " + String(get_ms2() ? "habilitado ✅" : "deshabilitado ❌") + "%0A";
    // mensaje += "Sensor Movimiento 3: " + String(get_ms3() ? "habilitado ✅" : "deshabilitado ❌");

    // sendTelegramMessage(mensaje);

    
        String keyboardJson = R"({
            "inline_keyboard": [
                [{"text": "Sensor puerta delantera"}],
                [{"text": "Sensor puerta trasera"}],
                [{"text": "Sensor movimiento 1"}]
            ],
            "one_time_keyboard": true
        })";

    sendTelegramKeyboard(keyboardJson);
    for( int i = 0; i < 500; i++){
        String reponse = getTelegramUpdate(offset);
        if (reponse != ""){
            break;
        }
    }
    removeTelegramKeyboard();
}

bool get_ds1(){
    return ds1Enabled;
}


bool get_ds2(){
    return ds2Enabled;
}


bool get_ms1(){
    return ms1Enabled;
}


bool get_ms2(){
    return ms2Enabled;
}


bool get_ms3(){
    return ms3Enabled;
}