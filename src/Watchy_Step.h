#ifndef WATCHY_STEP_H
#define WATCHY_STEP_H

#include "Watchy_Base.h"
#include "icons.h"
#include "fonts.h"

class WatchyStep : public WatchyBase {
    
    public:
        WatchyStep();
        void drawWatchFace();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawBattery();
        void handleButtonPress();

    private:       
        float getMaxSteps();
        void startNewDay();
        int32_t getStepsOfDay();
};

#endif
