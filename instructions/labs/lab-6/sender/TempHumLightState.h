#ifndef TempHumLightState_h
#define TempHumLightState_h

#include <avr/pgmspace.h>
#include <LowPower.h>

// Payload that child sends to the parent
struct SensorData {
    float temp;
    float hum;
    uint16_t lightLevel;
};

typedef enum {
  READ_SERIAL,
  GOTO_SLEEP,
  READ_SENSORS,
  RECEIVING_RADIO,
  SENDING_RADIO
} State;

#endif
