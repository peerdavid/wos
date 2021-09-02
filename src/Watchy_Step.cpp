#include "Watchy_Step.h"


// For more fonts look here:
// https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
#define FONT_LARGE_BOLD  FreeSansBold24pt7b
#define FONT_MEDUM_BOLD  FreeSansBold12pt7b
#define FONT_MEDUM       FreeSans12pt7b
#define FONT_SMALL_BOLD  FreeSansBold9pt7b
#define FONT_SMALL       FreeSans9pt7b

#define GRAPH_COLOR         FOREGROUND_COLOR

// Store in RTC RAM, otherwise we loose information between different interrupts
RTC_DATA_ATTR uint32_t steps_hours[24] = {0}; //{20,20,15,13,12,11,10,9,8,7,0};
RTC_DATA_ATTR uint32_t steps_hours_yesterday[24] = {0}; //{5,5,5,5,5,5,10,15,20,30,35,30,30,25,14,13,12,5,13,0,0,0,0,0};
RTC_DATA_ATTR bool print_date = true;


WatchyStep::WatchyStep(){

}


void WatchyStep::handleButtonPress(){
    WatchyBase::handleButtonPress();

    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if(IS_DOUBLE_TAP){
        print_date = print_date == true ? false : true;
        RTC.read(currentTime);
        showWatchFace(true);
        return;
    }
}


void WatchyStep::drawWatchFace(){
    WatchyBase::drawWatchFace();
    if(watchFaceDisabled()){
        return;
    }

    for(uint8_t i=0; i<4; i++){
        display.drawRoundRect(0+i, 0+i, 196, 196, 4, FOREGROUND_COLOR);
    }

    drawTime();
    drawSteps();
    if(print_date){
        drawDate();
    } else {
        drawBattery();
    }
}


void WatchyStep::drawTime(){
    display.setFont(&FONT_LARGE_BOLD);
    display.setCursor(12, 42);
    if(currentTime.Hour < 10){
        display.print("0");
    }
    display.print(currentTime.Hour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }
    display.println(currentTime.Minute);
}


void WatchyStep::drawDate(){
    int16_t  x1, y1;
    uint16_t w, h;

    display.setFont(&FONT_MEDUM);
    String dayOfWeek = dayShortStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 55, 195, &x1, &y1, &w, &h);
    display.setCursor(200-20-w, 25);
    display.println(dayOfWeek);

    display.getTextBounds("00", 55, 195, &x1, &y1, &w, &h);
    display.setCursor(200-20-w, 45);
    if(currentTime.Day < 10){
        display.print("0");
    }
    display.println(currentTime.Day);
}


void WatchyStep::drawBattery(){
    int8_t percentage = getBattery();
    percentage = min((int8_t) 99, percentage);
    display.drawBitmap(145, 15, battery, 37, 21, FOREGROUND_COLOR);
    display.setFont(&FONT_SMALL_BOLD);
    display.setCursor(152, 31);
    if(percentage < 10) {
        display.print("0");
    }
    display.println(percentage);
}


float WatchyStep::getMaxSteps(){
    float max_steps = -1;
    for(int i=0; i < 24; i++){
        max_steps = max(float(steps_hours[i]), max_steps);
        max_steps = max(float(steps_hours_yesterday[i]), max_steps);
    }
    return max_steps;
}


void WatchyStep::startNewDay(){
    for(int i=0; i < 24; i++){
        steps_hours_yesterday[i] = steps_hours[i];
        steps_hours[i] = 0;
    }
}


int32_t WatchyStep::getStepsOfDay(){
    int32_t steps = 0;
    for(int i=0; i < 24; i++){
        steps += steps_hours[i];
    }
    return steps;
}


void WatchyStep::drawSteps(){
    int8_t max_height = 70;

    // Bottom line
    display.drawLine(5, 140, 195, 140, FOREGROUND_COLOR);

    // Text of bottom line
    display.setFont(&FONT_SMALL);
    int16_t  x1, y1;
    uint16_t width, height;
    display.getTextBounds("12", 55, 195, &x1, &y1, &width, &height);
    display.setCursor(4+12*8 + 2-int(width/2), 160);
    display.println("12");
    display.setCursor(4+6*8 + 2-int(width/2), 160);
    display.println("06");
    display.setCursor(4+18*8 + 2-int(width/2), 160);
    display.println("18");

    // Whenever we have a new hour, we can restart our step counting.
    // But only if its an rtc alarm - ignore button press etc.

    // ToDo: Fix -> changed due to refactoring
    //if(rtc_alarm && currentTime.Minute == 0){
    if(currentTime.Minute == 0){
        int8_t last_hour = currentTime.Hour < 1 ? 23 : currentTime.Hour-1;
        steps_hours[last_hour] = sensor.getCounter();
        steps_hours[currentTime.Hour] = 0;
        sensor.resetStepCounter();
    } else {
        steps_hours[currentTime.Hour] = sensor.getCounter();
    }

    // Print max steps for y axis
    uint32_t max_steps = getMaxSteps();
    display.setCursor(20, 140-max_height-5);
    display.print(max_steps);
    display.println(" steps");

    for(int h=0; h < 24; h++){
        // Clean lines for current hour
        uint32_t relative_steps = max_height * float(steps_hours[h]) / float(max_steps+1); // Numerical stability
        display.drawLine(4+h*8 + 2, 140, 4+h*8 + 2, 140-relative_steps, GRAPH_COLOR);
        display.drawLine(4+h*8 + 3, 140, 4+h*8 + 3, 140-relative_steps, GRAPH_COLOR);
        display.drawLine(4+h*8 + 4, 140, 4+h*8 + 4, 140-relative_steps, GRAPH_COLOR);
        display.drawLine(4+h*8 + 5, 140, 4+h*8 + 5, 140-relative_steps, GRAPH_COLOR);

        // Last day - only horizontal line
        if(steps_hours_yesterday[h] > 0){
            relative_steps = max_height * float(steps_hours_yesterday[h]) / float(max_steps+1);
            uint16_t color = steps_hours[h] < steps_hours_yesterday[h] ? FOREGROUND_COLOR : BACKGROUND_COLOR;
            display.drawLine(4+h*8 + 2, 140-relative_steps, 4+h*8 + 5, 140-relative_steps, color);
            display.drawLine(4+h*8 + 2, 140-relative_steps-1, 4+h*8 + 5, 140-relative_steps-1, color);
        }
    }

    // Small indicator for current position
    display.drawLine(4+currentTime.Hour*8 + 3, 140, 4+currentTime.Hour*8 + 3, 146, FOREGROUND_COLOR);
    display.drawLine(4+currentTime.Hour*8 + 4, 140, 4+currentTime.Hour*8 + 4, 146, FOREGROUND_COLOR);

    // Show number of steps as text and bitmap
    int32_t step_count_day = getStepsOfDay();
    display.fillRect(0, 170, 196, 26, FOREGROUND_COLOR);

    display.setTextColor(BACKGROUND_COLOR);
    display.setFont(&FONT_MEDUM_BOLD);
    display.getTextBounds(String(step_count_day), 55, 195, &x1, &y1, &width, &height);
    int8_t bitmap_pos = int(200-width)/2;
    display.drawBitmap(bitmap_pos-15, 174, steps, 19, 23, BACKGROUND_COLOR);
    display.setCursor(bitmap_pos+15, 192);
    display.println(step_count_day);
    display.setTextColor(FOREGROUND_COLOR);
}