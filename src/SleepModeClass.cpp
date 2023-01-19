#include <time.h>
#include <Arduino.h>

#include "SleepModeClass.h"
#include "MessageOutput.h"
#include "SunsetClass.h"
#include "Configuration.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

void SleepModeClass::init()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
        MessageOutput.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        MessageOutput.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        MessageOutput.println("Wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        MessageOutput.println("Wakeup caused by touchpad");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        MessageOutput.println("Wakeup caused by ULP program");
        break;
    default:
        MessageOutput.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
}
void SleepModeClass::go_to_sleep(uint64_t minutes) {
    if(minutes < 5) {
        return;
    }
    esp_sleep_enable_timer_wakeup(minutes * 60 * uS_TO_S_FACTOR);

    MessageOutput.println("Going to sleep in 3 seconds.");
    delay(3000);
    esp_deep_sleep_start();
}


void SleepModeClass::loop() {
    unsigned long time = millis();
    if(time - _lastChecked < 10000) {
        return;
    }
    _lastChecked = time;

    MessageOutput.print(F("Is daytime: "));
    MessageOutput.println(SunsetClassInst.isDayTime());

    MessageOutput.print(F("Sunrise: "));
    MessageOutput.print((int)(SunsetClassInst.getSunriseMinutes() / 60));
    MessageOutput.print(":");
    MessageOutput.println(SunsetClassInst.getSunriseMinutes() % 60);

    MessageOutput.print(F("Sunset: "));
    MessageOutput.print((int)(SunsetClassInst.getSunsetMinutes() / 60));
    MessageOutput.print(":");
    MessageOutput.println(SunsetClassInst.getSunsetMinutes() % 60);

    MessageOutput.print(F("Time until sunrise: "));
    MessageOutput.print((int)(SunsetClassInst.getMinutesUntilSunrise() / 60));
    MessageOutput.print(":");
    MessageOutput.println(SunsetClassInst.getMinutesUntilSunrise() % 60);

    if (!SunsetClassInst.isDayTime() && (time > (1000 * 180))) { // Wait at least 3 minutes before sleeping after boot
        go_to_sleep(SunsetClassInst.getMinutesUntilSunrise());
    }
}

SleepModeClass SleepMode;
