#include "Kernel.h"


Kernel::Kernel(){

}


void Kernel::tick(){
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
    _watchface.init(wakeup_reason);

    // Whenever we have a new hour, we can restart our step counting.
    // But only if its an rtc alarm - ignore button press etc.
    // ToDo: Move into step counter module which maintains steps over (1) 48h and (2) 7 days or so.
    bool rtc_alarm = wakeup_reason == ESP_SLEEP_WAKEUP_EXT0;
    if(rtc_alarm && _watchface.currentTime.Minute == 0 && _watchface.currentTime.Hour == 0){
        sensor.resetStepCounter();
    }


    /*
    1.) Move step counter logic into step counter module.
    2.) Move MQTT logic into SmartHome module.
    3.) Check what else we have in the huge Watchy class.
     */
}