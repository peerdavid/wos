#include "Watchy_Analog.h"


#define FONT                WallingtonRegular12pt7b
#define WHITE_TEXT_SIZE     2

WatchyAnalog::WatchyAnalog(){

}


void WatchyAnalog::handleButtonPress(){
    WatchyBase::handleButtonPress();

    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if(IS_DOUBLE_TAP){
        dark_mode = dark_mode ? false : true;
        RTC.read(currentTime);
        showWatchFace(false);
        return;
    }
}


void WatchyAnalog::drawWatchFace(){
    WatchyBase::drawWatchFace();
    if(watchFaceDisabled()){
        return;
    }

    // drawBitmapRotate(100, 200, analog_bg, 0, LIGHT_GREY);
    // drawBitmapRotate(100, 200, analog, 0, GxEPD_BLACK);
    for(int y=100; y < 200; y++){
        for(int x=0; x < 200; x++){
            drawPixel(x, y, LIGHT_GREY);
        }
    }

    drawDate();
    drawSteps();
    drawBattery();
    drawAlarm();
    drawTime();

    //drawHelperGrid();
}


void WatchyAnalog::printCentered(uint16_t x, uint16_t y, String text){
    int16_t  x1, y1;
    uint16_t w, h;

    display.getTextBounds(text, 100, 100, &x1, &y1, &w, &h);
    display.setCursor(x-w/2, y+h/2);
    display.println(text);
}


void WatchyAnalog::drawTime(){
    int theHour = currentTime.Hour;
    int theMinute = currentTime.Minute;

    // Hour hand
    int hourAngle = ((theHour%12)*60 + theMinute) * 360 / 720;
    drawBitmapRotate(100,100, hour_hand_inv, hourAngle, GxEPD_WHITE);
    drawBitmapRotate(100,100, hour_hand_inv, hourAngle, GREY);
    drawBitmapRotate(100,100, hour_hand, hourAngle, GxEPD_BLACK);

    // Minute hand
    drawBitmapRotate(100,100, minute_hand_inv, theMinute * 6, GxEPD_WHITE);
    drawBitmapRotate(100,100, minute_hand_inv, theMinute * 6, GREY);
    drawBitmapRotate(100,100, minute_hand, theMinute * 6, GxEPD_BLACK);

    // Middle circle
    display.fillCircle(100,100, 13, BACKGROUND_COLOR);
    display.fillCircle(100,100, 9, FOREGROUND_COLOR);
    display.fillCircle(100,100, 5, BACKGROUND_COLOR);
}


void WatchyAnalog::drawDate(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);

    // Day
    String dayStr = String(currentTime.Day);
    dayStr = currentTime.Day < 10 ? "0" + dayStr : dayStr;

    display.setTextColor(BACKGROUND_COLOR);
    for(int i=-WHITE_TEXT_SIZE;i<WHITE_TEXT_SIZE+1;i++){
        for(int j=-WHITE_TEXT_SIZE;j<WHITE_TEXT_SIZE+1;j++){
            printCentered(155+i, 112+j, String(dayStr));
        }
    }
    display.setTextColor(FOREGROUND_COLOR);
    printCentered(155, 112, dayStr);

    // Week day
    String dayOfWeek = dayShortStr(currentTime.Wday);
    printCentered(155, 84, dayOfWeek);
}

void WatchyAnalog::drawAlarm(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);

    if (alarm_timer >= 0){
        printCentered(100, 40, "T-" + String(alarm_timer) + " min.");
    } else{
        printCentered(100, 35, "Watchy");
    }
}


void WatchyAnalog::drawBattery(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);

    int8_t bat = getBattery();
    bat = bat >= 100 ? 99 : bat;
    String batStr = String(bat);
    batStr = bat < 10 ? "0" + batStr : batStr;

    display.setTextColor(BACKGROUND_COLOR);
    for(int i=-WHITE_TEXT_SIZE;i<WHITE_TEXT_SIZE+1;i++){
        for(int j=-WHITE_TEXT_SIZE;j<WHITE_TEXT_SIZE+1;j++){
            printCentered(45+i, 100+j, String(batStr) + "%");
        }
    }
    display.setTextColor(FOREGROUND_COLOR);

    printCentered(45, 100, batStr + "%");
}


void WatchyAnalog::drawSteps(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);

    uint32_t steps = sensor.getCounter();

    display.setTextColor(BACKGROUND_COLOR);
    for(int i=-WHITE_TEXT_SIZE;i<WHITE_TEXT_SIZE+1;i++){
        for(int j=-WHITE_TEXT_SIZE;j<WHITE_TEXT_SIZE+1;j++){
            printCentered(100+i, 155+j, String(steps));
        }
    }
    display.setTextColor(FOREGROUND_COLOR);

    printCentered(100, 155, String(steps));
}