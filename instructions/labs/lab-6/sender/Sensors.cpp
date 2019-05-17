#include "Sensors.h"

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter(0x23);

SENSORS::SENSORS(){}
SENSORS::~SENSORS(){}

void SENSORS::DHT_init(){
  pinMode(DHTPIN_VCC,OUTPUT);
  digitalWrite(DHTPIN_VCC,LOW);
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void SENSORS::BH1750_init(){
  lightMeter.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
  Serial.println(F("BH1750 Test"));
}

void SENSORS::printHum(float h) {
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F(" %\t"));
}

void SENSORS::printTemp(float t) {
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F(" *C "));
}

float SENSORS::readTemp() {
  digitalWrite(DHTPIN_VCC, HIGH);

  // Wait at least 2 seconds seconds between measurements.
  unsigned long previousMillis = millis();
  unsigned long currentMillis = millis();
  while (currentMillis - previousMillis <= 2300) {
    // save the last time you read the sensor
    currentMillis = millis();
  }

  return dht.readTemperature();


  digitalWrite(DHTPIN_VCC,LOW);
}

float SENSORS::readHum() {

  float h = dht.readHumidity();
  digitalWrite(DHTPIN_VCC,LOW);
  return h;

}

uint16_t SENSORS::readLight() {
  return lightMeter.readLightLevel();
}
