#include "bell.h"
#include "pins.h"
#include "telegram.h"

#include <arduino.h>

volatile bool bellActivated = false;

void IRAM_ATTR bell_ring_interrupt() {
   bellActivated = true;
}


void bell_init(){
    pinMode(BELL_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BELL_PIN), bell_ring_interrupt, FALLING);
}

void bell_ring(){    
    if ( bellActivated ){
        sendTelegramMessage("Timbreeee 👏👏");
        bellActivated = false;
    }
}

