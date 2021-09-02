#ifndef WATCHY_JARVIS_H
#define WATCHY_JARVIS_H

#include "Watchy_Base.h"
#include "icons.h"
#include "fonts.h"
#include "Images/iron_man.h"

class WatchyJarvis : public WatchyBase {
    
    public:
        WatchyJarvis();
        void drawWatchFace();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawBattery();
        void handleButtonPress();
    
    private:
        void drawCircle(int16_t x0, int16_t y0, int16_t r, 
                        uint16_t color, uint8_t width);

};

#endif
