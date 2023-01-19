#include <time.h>
#include <Arduino.h>

#include "SleepModeClass.h"
#include "MessageOutput.h"
#include "SunsetClass.h"
#include "Configuration.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

SleepModeClass SleepMode;

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

    struct tm timeinfo;
    getLocalTime(&timeinfo, 5);

    int sunrise = SunsetClassInst.getSunriseMinutes() + Configuration.get().Sunset_Sunriseoffset;
    int sunset = SunsetClassInst.getSunsetMinutes() + Configuration.get().Sunset_Sunsetoffset;

    int minutesPastMidnight = timeinfo.tm_hour * 60 + timeinfo.tm_min;
    _isDayTime = (minutesPastMidnight >= sunrise) && (minutesPastMidnight < sunset);

    bool afterSunrise = minutesPastMidnight > sunrise;
    if (afterSunrise == true) {
        _minutesUntilSunrise = 1440 - minutesPastMidnight + sunrise;
    } else {
        _minutesUntilSunrise = sunrise - minutesPastMidnight;
    }

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
    MessageOutput.print((int)(_minutesUntilSunrise / 60));
    MessageOutput.print(":");
    MessageOutput.println(_minutesUntilSunrise % 60);

    if (!_isDayTime && time > (1000 * 180)) { // Wait at least 3 minutes before sleeping
        go_to_sleep(_minutesUntilSunrise);
    }
}