#include <Arduino.h>
#include "motor.h"
#include "log.h"

const int pinM1 = D2;
const int pinM2 = D3;

unsigned long m1Timeout = 0;
unsigned long m2Timeout = 0;

void motor_init() {
    LOG("Initializing motors");
    LOG_NEW_LINE

    pinMode(pinM1, OUTPUT);
    pinMode(pinM2, OUTPUT);

    digitalWrite(pinM1, HIGH);
    digitalWrite(pinM2, HIGH);
}

void motor_on(int id, int msecs) {
    bool changed = false;

    switch(id) {
        case 1:
            digitalWrite(pinM1, LOW);
            m1Timeout = millis() + msecs;
            break;
        case 2:
            digitalWrite(pinM2, LOW);
            m2Timeout = millis() + msecs;
            break;
    }

    if(changed) {
        LOG("motor on");
        LOG_NEW_LINE
    }
}

void motor_off(int id) {
    bool changed = false;

    switch(id) {
        case 1:
            digitalWrite(pinM1, HIGH);
            break;
        case 2:
            digitalWrite(pinM2, HIGH);
            break;
    }

    if(changed) {
        LOG("motor off");
        LOG_NEW_LINE
    }
}

void motor_loop() {
    if(m1Timeout < millis()) {
        motor_off(1);
    }

    if(m2Timeout < millis()) {
        motor_off(2);
    }
}