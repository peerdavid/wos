#include "Kernel.h"


#define MODULE_NAME "Kernel"

Kernel::Kernel(){
    _logging = Logging::instance();
}


void Kernel::state_machine(){
    KernelState state = get_state();

    switch(state){
        case KERNEL_STATE_RTC_ALARM:
            _logging->log(MODULE_NAME, "State: RTC Alarm.");
            tick();
            break;

        case KERNEL_STATE_BTN_PRESSED:
            _logging->log(MODULE_NAME, "State: Btn pressed.");
            tick();
            break;

        case KERNEL_STATE_RESET:
            _logging->log(MODULE_NAME, "State: Reset.");
            tick();
            break;
    }

    // Pow, lots of stuff to do - but finally we can sleep again :)
    deep_sleep();
}


KernelState Kernel::get_state(){
    KernelState state;
    _wakeup_reason = esp_sleep_get_wakeup_cause();
    if(_wakeup_reason == ESP_SLEEP_WAKEUP_EXT0){
        state = KERNEL_STATE_RTC_ALARM;
    } else if(_wakeup_reason == ESP_SLEEP_WAKEUP_EXT1){
        state = KERNEL_STATE_BTN_PRESSED;
    } else {
        state = KERNEL_STATE_RESET;
    }

    return state;
}


void Kernel::tick(){
    watch_face_tick();
    step_module_tick();
}


void Kernel::watch_face_tick(){
    _watchface.init(_wakeup_reason);
}


void Kernel::step_module_tick(){
    tmElements_t currentTime;
    _watchface.RTC.read(currentTime);

    if(currentTime.Hour == 0 && currentTime.Minute == 0){
        _logging->log(MODULE_NAME, "Reset step counter.");
        sensor.resetStepCounter();
    }
}


void Kernel::btn_press(){

}


void Kernel::alarm(){

}


void Kernel::deep_sleep(){
  esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
  esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
  esp_deep_sleep_start();
}