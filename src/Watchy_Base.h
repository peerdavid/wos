#ifndef WATCHY_BASE_H
#define WATCHY_BASE_H

#include "Watchy.h"
#include "fonts.h"
#include "icons.h"
#include <WiFiManager.h>
#include "Images/smart_home.h"
#include "Images/sleep.h"
#include <PubSubClient.h>


// ---- USER SETTINGS ----------------------------
// Note: The user settings can also be specified
//       in config.h - its loaded if it exists.
#define WIFI_SSID      "ENTER_HERE"
#define WIFI_PASS      "ENTER_HERE"

#define MQTT_BROKER    "ENTER_HERE"
#define MQTT_TOPIC     "ENTER_HERE"
#define MQTT_PAYLOAD   "ENTER_HERE"

#define SLEEP_HOUR          1
#define SLEEP_MINUTE        0

#define BATTERY_OFFSET      0.25    // This offset is different for each watchy [V]

#if __has_include("usr_config.h") && __has_include(<stdint.h>)
# include "usr_config.h"
#endif
// -----------------------------------------------


extern RTC_DATA_ATTR bool show_mqqt_data;
extern RTC_DATA_ATTR int16_t alarm_timer;

// Btn definitions
#define IS_DOUBLE_TAP       (wakeupBit & ACC_INT_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_UP     (wakeupBit & UP_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_LEFT_UP      (wakeupBit & BACK_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_DOWN   (wakeupBit & DOWN_BTN_MASK && guiState == WATCHFACE_STATE)

// Color settings
extern RTC_DATA_ATTR bool dark_mode;
#define FOREGROUND_COLOR    (dark_mode ? GxEPD_WHITE : GxEPD_BLACK)
#define BACKGROUND_COLOR    (dark_mode ? GxEPD_BLACK : GxEPD_WHITE)

// Other settings
//#define EXT_INT_MASK        MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK|ACC_INT_MASK
#define EXT_INT_MASK        MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK

#define GREY 0x7BEF
#define DARK_GREY 0x7BEE
#define LIGHT_GREY 0x7BED

class WatchyBase : public Watchy {
    public:
        WatchyBase();
        virtual void drawWatchFace();
        virtual void init(esp_sleep_wakeup_cause_t wakeup_reason);
        virtual void handleButtonPress();
        virtual void deepSleep();
        void vibrate(uint8_t times=1, uint32_t delay_time=50);
        uint8_t getBattery();
        bool connectWiFi();
        void disconnectWiFi();
        uint8_t openDoor();
        uint8_t loadMqqtData();
        void drawHelperGrid();
        void drawMqqtData();
        bool watchFaceDisabled();
        void drawPixel(int16_t x, int16_t y,uint16_t col);
		void drawBitmapCol(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color1);
		int getPixel(int16_t x, int16_t y, const uint8_t *bitmap);
		void drawBitmapRotate(int xx, int yy, const uint8_t *bitmap, unsigned int fAngle, uint16_t color);
    private:
        void _minutesToHM(int16_t minutes, uint8_t &h, uint8_t &m);
        void _rtcConfig();
        void _bmaConfig();
        static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
        static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
};

#endif
