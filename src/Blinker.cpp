#include <Arduino.h>

#include "Hoymiles.h"
#include "Blinker.h"

BlinkerClass Blinker;

void BlinkerClass::loop() {
    if ((millis() - _lastLedStateCheck) > 200) {
        // Blink if at least one update was received in the last 60 seconds
        uint32_t time = millis();
        bool received = false;
        for (int i = 0; i < Hoymiles.getNumInverters(); i++) {
            std::shared_ptr<InverterAbstract> iv = Hoymiles.getInverterByPos(i);
            if (((time - iv->Statistics()->getLastUpdate()) / 1000) < 60) {
                received = true;
            }
        }
        if ((received == true) && (Hoymiles.getRadio()->isConnected())) {
            // blink
            LedToggle(time);
        } else {
            LedOff(time);
        }
        _lastLedStateCheck = time;
    }
}

void BlinkerClass::LedToggle(uint32_t time)
{
    if(_ledState == 0) {
        if((time - _ledStateSince) > 1700) {

            LedOn(time);
        }
    } else {
        if((time - _ledStateSince) > 100) {
            LedOff(time);
        }
    }
}

void BlinkerClass::LedSetState(uint8_t state, uint32_t time) {
    if (_ledState != state) {
        LedSwitchGpio(state);
        _ledState = state;
        _ledStateSince = time;
    }

}
void BlinkerClass::LedOn(uint32_t time)
{
    LedSetState(1, time);
}

void BlinkerClass::LedOff(uint32_t time)
{
    LedSetState(0, time);
}

#define LED_PIN_GPIO (gpio_num_t) 2
void BlinkerClass::LedSwitchGpio(uint8_t state)
{
    gpio_pad_select_gpio(LED_PIN_GPIO);
    gpio_set_direction(LED_PIN_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN_GPIO, state);
}

