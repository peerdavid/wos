#include "Watchy_BTTF.h"


// For more fonts look here:
// https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
//#define FONT_LARGE       Bohemian_Typewriter22pt7b
//#define FONT_MEDUM       Bohemian_Typewriter18pt7b
#define FONT_7_SEG_LARGE    Seven_Seg24pt7b
#define FONT_7_SEG_MEDIUM   Seven_Seg18pt7b

WatchyBTTF::WatchyBTTF(){

}


void WatchyBTTF::handleButtonPress(){
    WatchyBase::handleButtonPress();

    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if(IS_DOUBLE_TAP){
        // NOP
    }
}


void WatchyBTTF::drawWatchFace(){
    WatchyBase::drawWatchFace();
    if(watchFaceDisabled()){
        return;
    }

    display.drawBitmap(0, 0, bttf, 200, 200, FOREGROUND_COLOR);
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();
    //drawHelperGrid();
}


void WatchyBTTF::printCentered(uint16_t x, uint16_t y, String text){
    int16_t  x1, y1;
    uint16_t w, h;

    display.getTextBounds(text, 40, 100, &x1, &y1, &w, &h);
    display.setCursor(x-w/2, y+h/2);
    display.println(text);
}


void WatchyBTTF::drawTime(){
    display.setFont(&FONT_7_SEG_LARGE);
    display.setTextColor(BACKGROUND_COLOR);
    String hourStr = String(currentTime.Hour);
    hourStr = currentTime.Hour < 10 ? "0" + hourStr : hourStr;
    printCentered(101, 42, hourStr);

    String minStr = String(currentTime.Minute);
    minStr = currentTime.Minute < 10 ? "0" + minStr : minStr;
    printCentered(165, 42, minStr);
}


void WatchyBTTF::drawDate(){
    display.setFont(&FONT_7_SEG_MEDIUM);
    display.setTextColor(BACKGROUND_COLOR);

    String month = monthShortStr(currentTime.Month);
    printCentered(35, 103, month);

    String dayStr = String(currentTime.Day);
    dayStr = currentTime.Day < 10 ? "0" + dayStr : dayStr;
    printCentered(94, 103, dayStr);

    if(alarm_timer < 0){
        String yearStr = String(currentTime.Year + YEAR_OFFSET);
        printCentered(157, 103, yearStr);
    } else {
        String alarm_str = "T-" + String(alarm_timer);
        printCentered(157, 103, alarm_str);
    }
}


void WatchyBTTF::drawBattery(){
    display.setFont(&FONT_7_SEG_MEDIUM);
    display.setTextColor(BACKGROUND_COLOR);

    int8_t bat = getBattery();
    String batStr = String(bat);
    batStr = bat < 10 ? "0" + batStr : batStr;
    batStr = bat < 100 ? "0" + batStr : batStr;
    printCentered(151, 169, batStr);
}


void WatchyBTTF::drawSteps(){
    display.setFont(&FONT_7_SEG_MEDIUM);
    display.setTextColor(BACKGROUND_COLOR);

    uint32_t steps = sensor.getCounter();
    String stepStr = String(steps);
    for(int i=1; i<5; i++){
        stepStr = steps < pow(10, i) ? "0" + stepStr : stepStr;
    }

    printCentered(62, 169, stepStr);
}