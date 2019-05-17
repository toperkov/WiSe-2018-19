#include "Radio_nRF.h"
#include "TempHumLightState.h"

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

RADIO::RADIO(){}
RADIO::~RADIO(){}

const byte address[6] = {"3node"};

void RADIO::nRF_init(){
  radio.begin();
  radio.setDataRate( RF24_250KBPS );

  radio.setChannel(111);
  radio.setPALevel(RF24_PA_MAX);

  radio.setRetries(3,5); // delay, count
  radio.openWritingPipe(address);
}

bool RADIO::RF_send( struct SensorData sensorData){
  return false; //!!!your code goes here!!!
}

void RADIO::RF_receive(bool rslt) {
  //!!!your code goes here!!!
}

void RADIO::RF_powerUp() {
  radio.powerUp();
  delay(50);
}

void RADIO::RF_powerDown() {
  delay(50);
  radio.powerDown();
  delay(50);
}
