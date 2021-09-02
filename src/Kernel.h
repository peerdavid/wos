#ifndef WOS_KERNEL_H
#define WOS_KERNEL_H


#include "Watchy_Analog.h"
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



class Kernel {

    public:
        Kernel();
        void tick();

    private:
        WatchyAnalog _watchface;
};

#endif