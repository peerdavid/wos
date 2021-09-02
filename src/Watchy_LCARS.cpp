#include "Watchy_LCARS.h"


// For more fonts look here:
// https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
//#define FONT_LARGE       Bohemian_Typewriter22pt7b
//#define FONT_MEDUM       Bohemian_Typewriter18pt7b
#define FONT_LARGE    Okuda_A5PL25pt7b
#define FONT_MEDIUM   Okuda_A5PL16pt7b
#define FONT_SMALL    Okuda_A5PL14pt7b

WatchyLCARS::WatchyLCARS(){

}


void WatchyLCARS::handleButtonPress(){
    WatchyBase::handleButtonPress();

    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if(IS_DOUBLE_TAP){
        // NOP
    }
}


void WatchyLCARS::drawWatchFace(){
    WatchyBase::drawWatchFace();
    if(watchFaceDisabled()){
        return;
    }

    // Draw background
    display.drawBitmap(0, 0, lcars_img, 200, 200, FOREGROUND_COLOR);
    display.setFont(&FONT_SMALL);
    display.setTextColor(BACKGROUND_COLOR);
    display.setCursor(110, 185);
    display.println("STEP");
    display.setCursor(60, 93);
    display.println("lcars-watchy");

    // Draw data
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();
    drawAlarm();
}


void WatchyLCARS::printCentered(uint16_t x, uint16_t y, String text){
    int16_t  x1, y1;
    uint16_t w, h;

    display.getTextBounds(text, 40, 100, &x1, &y1, &w, &h);
    display.setCursor(x-w/2, y+h/2);
    display.println(text);
}


void WatchyLCARS::drawTime(){
    display.setFont(&FONT_LARGE);
    display.setTextColor(BACKGROUND_COLOR);
    String hourStr = String(currentTime.Hour);
    hourStr = currentTime.Hour < 10 ? "0" + hourStr : hourStr;

    String minStr = String(currentTime.Minute);
    minStr = currentTime.Minute < 10 ? "0" + minStr : minStr;
    printCentered(55, 25, hourStr + ":" + minStr);
}


void WatchyLCARS::drawDate(){
    display.setFont(&FONT_SMALL);
    display.setTextColor(BACKGROUND_COLOR);

    String dayOfWeek = dayShortStr(currentTime.Wday);
    String dayStr = String(currentTime.Day);
    dayStr = currentTime.Day < 10 ? "0" + dayStr : dayStr;
    display.setCursor(97, 25);
    display.println(dayStr);
    display.setCursor(97, 47);
    display.println(dayOfWeek);
}


void WatchyLCARS::drawBattery(){
    display.setFont(&FONT_MEDIUM);
    display.setTextColor(BACKGROUND_COLOR);

    int8_t bat = getBattery();
    bat = bat >= 100 ? 99 : bat;
    String batStr = String(bat);
    batStr = bat < 10 ? "0" + batStr : batStr;
    display.setCursor(35, 125);
    display.println("battery: ");
    display.setCursor(113, 125);
    display.println(batStr + "%");
}


void WatchyLCARS::drawAlarm(){
    display.setFont(&FONT_MEDIUM);
    display.setTextColor(BACKGROUND_COLOR);

    display.setCursor(35, 151);
    display.println("alarm:");
    display.setCursor(113, 151);
    if(alarm_timer < 0){
        display.println("off");
    } else {
        display.println("T-" + String(alarm_timer));
    }
}


// void WatchyLCARS::drawTemperature(){
//     display.setFont(&FONT_SMALL);
//     display.setTextColor(BACKGROUND_COLOR);

//     uint8_t temperature = RTC.temperature() / 4;
//     display.setCursor(65, 143);
//     display.println("Temp. " + String(temperature));
// }


void WatchyLCARS::drawSteps(){
    display.setFont(&FONT_MEDIUM);
    display.setTextColor(BACKGROUND_COLOR);

    uint32_t steps = sensor.getCounter();
    display.setCursor(35, 186);
    display.println(steps);
}