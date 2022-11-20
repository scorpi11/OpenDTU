#include <stdint.h>

class BlinkerClass {
public:
    void loop();

private:
    void LedToggle(uint32_t time);
    void LedOn(uint32_t time);
    void LedOff(uint32_t time);
    void LedSetState(uint8_t state, uint32_t time);
    void LedSwitchGpio(uint8_t state);

    uint32_t _lastLedStateCheck = 0;
    uint32_t _ledStateSince = 0;
    uint8_t _ledState = 0;

};

extern BlinkerClass Blinker;