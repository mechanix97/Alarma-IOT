#ifndef _ALARM_H_
#define _ALARM_H_

#define FRONT_DOOR_SENSOR_PIN       13  
#define BACK_DOOR_SENSOR_PIN        12

#define MOVEMENT_SENSOR_1_PIN       14
#define MOVEMENT_SENSOR_2_PIN       27
#define MOVEMENT_SENSOR_3_PIN       26 

#define ALARM_SIREN_PIN             32  

#define LED_PIN  2

void alarm_init();
void activate_alarm();
void desactivate_alarm();

void turn_on_alarm_siren();
void turn_off_alarm_siren();

#endif
