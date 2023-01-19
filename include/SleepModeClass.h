#pragma once

class SleepModeClass {
public:
    void loop();
    void init();

private:
    bool _isDayTime;
    unsigned long _lastChecked;
    int _minutesUntilSunrise;
    void go_to_sleep(uint64_t);
};

extern SleepModeClass SleepMode;
