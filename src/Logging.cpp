#include "Logging.h"


Logging* Logging::_instance= nullptr;


Logging::Logging(){
#ifndef DISABLE_LOGGING
    Serial.begin(9600);
    Serial.flush();
#endif
}


Logging* Logging::instance(){
    if(_instance == nullptr){
        _instance = new Logging();
    }
    return _instance;
}


void Logging::log(String module_name, String message){
#ifndef DISABLE_LOGGING
    Serial.println("[" + module_name + "] " + message);
    Serial.flush();
#endif
}