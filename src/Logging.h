#include <Arduino.h>


// #define DISABLE_LOGGING

class Logging {
    public:
        static Logging* instance();
        void log(String module_name, String message);

    private:
        static Logging* _instance;
        Logging();
};