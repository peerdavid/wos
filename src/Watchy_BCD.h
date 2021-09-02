#ifndef WATCHY_BTTF_H
#define WATCHY_BTTF_H

#include "Watchy_Base.h"
#include "icons.h"
#include "fonts.h"
#include "Images/pcb.h"

class WatchyBCD : public WatchyBase {
    
    public:
        WatchyBCD();
        void drawWatchFace();
        void handleButtonPress();
    
    private:
        void printCentered(uint16_t x, uint16_t y, String text);
        void printBinary(uint16_t x, uint16_t y, uint8_t value, uint8_t n);
        void printBCD(String s, uint16_t y, uint16_t value);
};

#endif
