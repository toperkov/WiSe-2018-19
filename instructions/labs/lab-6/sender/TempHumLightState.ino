#include "TempHumLightState.h"
#include "Sensors.h"
#include "Radio_nRF.h"

SENSORS sensor;

RADIO radioNRF;

/*----------- Radio variables -----------*/
// Payload that the child sends to a parent
SensorData sensorData;

bool rslt; //returns true if receives ACK over radio; otherwise, it receives false

void setup() {
  Serial.begin(115200);
  sensor.DHT_init();
  sensor.BH1750_init();
  radioNRF.nRF_init();
  delay(100); // give some time to send data over Serial before going to sleep
}

void loop() {
  State state = GOTO_SLEEP;
  for(;;) {
    switch(state) {
      case READ_SERIAL:
        state = GOTO_SLEEP;
        break;
      case GOTO_SLEEP:
        Serial.println(F("Going to sleep"));
        radioNRF.RF_powerDown();
        delay(100);
        // Enter power down state for 8 s with ADC and BOD module disabled
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
        delay(100); // give him some time to wake up from sleep :D
        radioNRF.RF_powerUp();
        state = READ_SENSORS;
        break;
      case READ_SENSORS:
        sensorData.temp = sensor.readTemp();
        sensorData.hum = sensor.readHum();
        sensorData.lightLevel = sensor.readLight();
        delay(100); // give some time to send data over Serial
        state = SENDING_RADIO;
        break;
      case SENDING_RADIO:
        rslt = radioNRF.RF_send(sensorData);
        state = RECEIVING_RADIO;
        break;
      case RECEIVING_RADIO:
        radioNRF.RF_receive(rslt);
        state = READ_SERIAL;
        break;
    }
  }
}
