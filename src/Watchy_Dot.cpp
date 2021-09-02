#include "Watchy_Dot.h"


#define FONT                Himalaya_40020pt7b

WatchyDot::WatchyDot(){

}


void WatchyDot::handleButtonPress(){
    WatchyBase::handleButtonPress();

    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if(IS_DOUBLE_TAP){
        dark_mode = dark_mode ? false : true;
        RTC.read(currentTime);
        showWatchFace(false);
        return;
    }
}


void WatchyDot::drawWatchFace(){
    WatchyBase::drawWatchFace();
    if(watchFaceDisabled()){
        return;
    }

    drawTriangles();
    drawDate();
    drawAlarm();
    drawTime();
}


void WatchyDot::printCentered(uint16_t x, uint16_t y, String text, uint16_t size=1){
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 40, 100, &x1, &y1, &w, &h);

    display.setTextColor(BACKGROUND_COLOR);
    for(int i=-size-3;i<size+4;i++){
        for(int j=-size-3;j<size+4;j++){
            display.setCursor(x-w/2+i, y+h/2+j);
            display.println(text);
        }
    }

    display.setTextColor(FOREGROUND_COLOR);
    for(int i=-size;i<size+1;i++){
        for(int j=-size;j<size+1;j++){
            display.setCursor(x-w/2+i, y+h/2+j);
            display.println(text);
        }
    }
}


float M(float theta){
    return max(abs(cos(theta)), abs(sin(theta)));
}


void WatchyDot::drawTime(){
    // See also https://math.stackexchange.com/questions/2468060/find-x-y-coordinates-of-a-square-given-an-angle-alpha
    int theHour = currentTime.Hour;
    int theMinute = currentTime.Minute;

    // Hour hand
    int s = 80;
    float theta = ((theHour%12)*60 + theMinute) * 360 / 720;
    theta = (int)(theta-90) % 360;
    theta = theta * PI / 180.0;

    int x = 100 + (int)(cos(theta) * s / M(theta));
    int y = 100 + (int)(sin(theta) * s / M(theta));
    //printCentered(x, y, "h");
    display.fillCircle(x, y, 16, FOREGROUND_COLOR);
    display.fillCircle(x, y, 12, BACKGROUND_COLOR);

    // Minute hand
    theta = theMinute * 360 / 60;
    theta = (int)(theta-90) % 360;
    theta = theta * PI / 180.0;
    x = 100 + (int)(cos(theta) * s / M(theta));
    y = 100 + (int)(sin(theta) * s / M(theta));

    //printCentered(x, y+7, "m");
    display.fillCircle(x, y, 12, BACKGROUND_COLOR);
    display.fillCircle(x, y, 8, FOREGROUND_COLOR);
}


void WatchyDot::drawDate(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);

    // Day
    String dayStr = String(currentTime.Day);
    dayStr = currentTime.Day < 10 ? "0" + dayStr : dayStr;
    String date = dayShortStr(currentTime.Wday);
    date += "  " + String(dayStr);
    printCentered(100, 80, date, 2);
}


void WatchyDot::drawTriangles(){
    int steps = sensor.getCounter();
    steps = steps % 10000;
    steps = 200.0 * (steps / 10000.0);

    int bat = getBattery();
    bat = min(bat, 100);
    bat *= 2;

    // Draw upper and lower triangle
    for(int y=0; y < 200; y++){
        for(int x=0; x < 200; x++){
            uint16_t color;
            bool upper_triangle = x > y;

            if(upper_triangle){
                if(y >= 200-bat-1 && y < 200-bat+1){
                    color = GxEPD_WHITE;
                } else {
                    color = y >= 200-bat ? DARK_GREY : LIGHT_GREY;
                }
            } else {
                if(y >= 200-steps-1 && y < 200-steps+1){
                    color = GxEPD_WHITE;
                } else {
                    color = y >= 200-steps ? GxEPD_BLACK : GREY;
                }
            }

            // Split both triangles
            if(abs(x-y) < 2){
                color = GxEPD_WHITE;
            }

            drawPixel(x, y, color);
        }
    }
}


void WatchyDot::drawAlarm(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);
    if(alarm_timer < 0){
        return;
    }

    printCentered(100, 153, String(alarm_timer) + " min.");
}