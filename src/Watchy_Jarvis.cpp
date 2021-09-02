#include "Watchy_Jarvis.h"


// For more fonts look here:
// https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
#define FONT_LARGE       Bitwise_m19x18pt7b
#define FONT_MEDUM       Bitwise_m19x12pt7b
#define FONT_STEPS       Bitwise_m19x10pt7b
#define FONT_SMALL       Bitwise_m19x8pt7b


WatchyJarvis::WatchyJarvis(){

}


void WatchyJarvis::handleButtonPress(){
    WatchyBase::handleButtonPress();

    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if(IS_DOUBLE_TAP){
        dark_mode = dark_mode == true ? false : true;
        RTC.read(currentTime);
        showWatchFace(false);
        return;
    }
}


void WatchyJarvis::drawWatchFace(){
    WatchyBase::drawWatchFace();
    if(watchFaceDisabled()){
        return;
    }

    display.drawBitmap(60, 60, iron_man, 80, 80, FOREGROUND_COLOR);
    drawDate();
    drawTime();
    drawSteps();
    drawBattery();
}


void WatchyJarvis::drawTime(){
    display.setFont(&FONT_LARGE);
    display.setCursor(80, 30);
    if(currentTime.Hour < 10){
        display.print("0");
    }
    display.print(currentTime.Hour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }
    display.println(currentTime.Minute);

    display.drawLine(53,1, 195, 1, FOREGROUND_COLOR);
    display.drawLine(53,2, 195, 2, FOREGROUND_COLOR);
    display.drawLine(55,35, 195,35, FOREGROUND_COLOR);
    display.drawLine(55,36, 195,36, FOREGROUND_COLOR);
    display.drawLine(195,2, 195,135, FOREGROUND_COLOR);
    display.drawLine(196,2, 196,135, FOREGROUND_COLOR);
    display.drawLine(180,36, 180,135, FOREGROUND_COLOR);

    display.setFont(&FONT_SMALL);
    display.setCursor(120, 50);
    display.println("time");
    display.drawLine(53,42, 115,42, FOREGROUND_COLOR);
    display.drawLine(150,50, 195,50, FOREGROUND_COLOR);
}


void WatchyJarvis::drawDate(){
    int16_t  x1, y1;
    uint16_t w, h;

    drawCircle(10,20,48,FOREGROUND_COLOR,4);
    drawCircle(10,20,38,FOREGROUND_COLOR,2);

    display.setFont(&FONT_SMALL);
    String dayOfWeek = dayShortStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 55, 195, &x1, &y1, &w, &h);
    display.setCursor(5, 20);
    display.println(dayOfWeek);

    display.setFont(&FONT_MEDUM);
    display.getTextBounds("00", 55, 195, &x1, &y1, &w, &h);
    display.setCursor(5, 40);
    if(currentTime.Day < 10){
        display.print("0");
    }
    display.println(currentTime.Day);
}


void WatchyJarvis::drawBattery(){
    int16_t  x1, y1;
    uint16_t w, h;

    int8_t percentage = getBattery();
    int8_t height = 60 - float(percentage)/100 * 60;
    drawCircle(20, 170, 30, FOREGROUND_COLOR, 6);
    display.fillRect(0, 140, 60, height, BACKGROUND_COLOR);
    display.fillRect(0, 140, 20, 30, BACKGROUND_COLOR);
    drawCircle(20, 170, 40, FOREGROUND_COLOR, 3);

    percentage = min((int8_t) 99, percentage);
    display.setFont(&FONT_MEDUM);
    display.setCursor(5, 180);
    if(percentage < 10) {
        display.print("0");
    }
    display.println(percentage);

    display.setFont(&FONT_SMALL);
    display.setCursor(10, 110);
    display.println("power");
    display.getTextBounds("power", 10, 195, &x1, &y1, &w, &h);
    display.drawLine(10-1, 112, 10+w+1, 112, FOREGROUND_COLOR);
    display.drawLine(10, 112, 5, 130, FOREGROUND_COLOR);
}


void WatchyJarvis::drawSteps(){
    int16_t  x1, y1;
    uint16_t w, h;

    display.setFont(&FONT_STEPS);
    uint32_t steps = sensor.getCounter();
    if(steps < 1000){
        display.getTextBounds(String(steps), 100, 180, &x1, &y1, &w, &h);
        display.setCursor(175 - w/2, 185);
        display.println(steps);
    }else{
        steps = round(float(steps / 1000));
        display.getTextBounds(String(steps), 100, 180, &x1, &y1, &w, &h);
        display.setCursor(175 - w/2, 180);
        display.println(steps);

        display.setFont(&FONT_SMALL);
        display.setCursor(155, 195);
        display.println("thous.");
    }

    drawCircle(185, 185, 50, FOREGROUND_COLOR, 2);
    drawCircle(185, 185, 40, FOREGROUND_COLOR, 4);

    display.setFont(&FONT_SMALL);
    display.setCursor(80, 170);
    display.println("steps");
    display.getTextBounds("steps", 80, 170, &x1, &y1, &w, &h);
    display.drawLine(80-1, 172, 80+w+1, 172, FOREGROUND_COLOR);
    display.drawLine(80+w, 172, 135, 182, FOREGROUND_COLOR);

    display.drawLine(60, 195, 135, 195, FOREGROUND_COLOR);
}


void WatchyJarvis::drawCircle(int16_t x0, int16_t y0, int16_t r,
    uint16_t color, uint8_t width){
    for(int i=0; i < width; i++){
        display.drawCircle(x0, y0, r-i, color);
    }
}