#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

#define DHTPIN 3
#define DHTPIN_VCC 4
#define DHTTYPE DHT22

class SENSORS {
public:
        SENSORS();
        ~SENSORS();
        void DHT_init();
        void BH1750_init();
        float readTemp();
        float readHum();
        uint16_t readLight();
        void printTemp(float );
        void printHum(float );
};

#endif
