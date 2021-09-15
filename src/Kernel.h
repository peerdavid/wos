#ifndef WOS_KERNEL_H
#define WOS_KERNEL_H


#include "Watchy_BTTF.h"
#include <Arduino.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <DS3232RTC.h>
#include <GxEPD2_BW.h>
#include <Wire.h>
#include "BLE.h"
#include "bma.h"
#include "config.h"
#include "Logging.h"


enum KernelState{
    KERNEL_STATE_RTC_ALARM,
    KERNEL_STATE_BTN_PRESSED,
    KERNEL_STATE_RESET
};


class Kernel {

    public:
        Kernel();
        void state_machine();

    private:
        KernelState get_state();
        void tick();
        void btn_press();
        void alarm();
        void watch_face_tick();
        void step_module_tick();
        void deep_sleep();

        // State parameters
        esp_sleep_wakeup_cause_t _wakeup_reason;

        // Modules
        WatchyBTTF _watchface;
        Logging* _logging;
};


extern RTC_DATA_ATTR BMA423 sensor;

#endif