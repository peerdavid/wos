#ifndef WATCHY_DOT_H
#define WATCHY_DOT_H

#include "Watchy_Base.h"
#include "icons.h"
#include "fonts.h"
#include "Images/analog.h"

class WatchyDot : public WatchyBase {

    public:
        WatchyDot();
        void drawWatchFace();
        void drawTime();
        void drawDate();
        void drawAlarm();
        void drawTriangles();
        void handleButtonPress();

    private:
        void printCentered(uint16_t x, uint16_t y, String text, uint16_t size);
        void drawCircle(int16_t x0, int16_t y0, int16_t r,
                        uint16_t color, uint8_t width);

};

#endif
