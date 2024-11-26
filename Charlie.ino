// Amogh Dwivedi: Charlie
// https://amoghdwivedi.com
// https://github.com/4moghDwivedi/Charlie


const int trigPin = 9;
const int echoPin = 10;

struct Period {
  int rate = 1000;
  unsigned long time_now = 0;
};

#define NUM_READINGS 10
struct MyParameter {
  float readings[NUM_READINGS];
  float total = 0;
  int readIndex = 0;
};

//these are the structs which keep track of raw data in order to calculate rolling averages
//rolling averages
MyParameter ultrasonicRollingAverage;
MyParameter photoresistorRollingAverage;
MyParameter temperatureRollingAverage;
MyParameter humidityRollingAverage;

//these are the predefined 'polling' rates for the sensors' raw data measurement, and their printing rates.
//ultrasonic:
Period ultrasonicPeriod;
Period ultrasonicRollingPeriod;
// there are two polling periods. one for the measurement of raw data, 
// two, for the printing of rolling averages.

//photoresistor:
Period photoresistorPeriod;
Period photoresistorRollingPeriod;
// there are two polling periods. one for the measurement of raw data, 
// two, for the printing of rolling averages.


//temperature sensor:
Period temperaturePeriod;
Period temperatureRollingPeriod;

Period humidityPeriod;
Period humidityRollingPeriod;

Period heaterToggle;

int photocellPin = 14;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider



float duration, distance;

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

bool enableHeater = false;
uint8_t loopCnt = 0;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  for (int i = 0; i<NUM_READINGS; i++){
      ultrasonicRollingAverage.readings[i]=0;
  }

  //different rates for different sensors are declared here.

  //ultrasonic sensor:
  float ultrasonicPrintRate = 50;
  //period is 1/10th of rolling period!!!!!!
  ultrasonicPeriod.rate = ultrasonicPrintRate / 10;
  ultrasonicRollingPeriod.rate = ultrasonicPrintRate;

  //photoresistor sensor:
  float photoresistorPrintRate = 50;
  //period is 1/10th of rolling period!!!!!!
  photoresistorPeriod.rate = photoresistorPrintRate / 10;
  photoresistorRollingPeriod.rate = photoresistorPrintRate;

  //temperature/humidity sensor:
  //period is 1/10th of rolling period!!!!!!
  float temperaturePrintRate = 50;
  temperaturePeriod.rate = temperaturePrintRate / 10; //longer rates for temperature sensor
  temperatureRollingPeriod.rate = temperaturePrintRate;

  float humidityPrintRate = 50;
  humidityPeriod.rate = humidityPrintRate / 10; //longer rates for humidity sensor
  humidityRollingPeriod.rate = humidityPrintRate;

  heaterToggle.rate = 30000;//toggling heater every 30 seconds

    while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())
    Serial.println("ENABLED");
  else
    Serial.println("DISABLED");

}

void loop() {
  //put your main code here, to run repeatedly:

  ultrasonicSensor();
  if (ultrasonicRollingAverage.readIndex == NUM_READINGS - 1) {
    printRollingAverageUltrasonic();
  }

  photoresistorSensor();
    if (photoresistorRollingAverage.readIndex == NUM_READINGS - 1) {
    printRollingAveragePhotoresistor();
  }

  temperatureSensor();
    if (temperatureRollingAverage.readIndex == NUM_READINGS - 1) {
    printRollingAverageTemperature();
  }

  humiditySensor();
    if (humidityRollingAverage.readIndex == NUM_READINGS - 1) {
    printRollingAverageHumidity();
  }

  heater();

  }

  void ultrasonicSensor() {

      if(millis() >= ultrasonicPeriod.time_now + ultrasonicPeriod.rate){

            ultrasonicPeriod.time_now += ultrasonicPeriod.rate;
            
            digitalWrite(trigPin, LOW); //setting trig pin to low
            delayMicroseconds(2);
            digitalWrite(trigPin, HIGH); //setting trig pin to high
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);

            duration = pulseIn(echoPin, HIGH);
            distance = (duration*.0343)/2;
            updateMovingAverage(&ultrasonicRollingAverage, distance);
  }

  }

  void photoresistorSensor(){
      if(millis() >= photoresistorPeriod.time_now + photoresistorPeriod.rate){
            photoresistorPeriod.time_now += photoresistorPeriod.rate;
            photocellReading = analogRead(photocellPin);

            updateMovingAverage(&photoresistorRollingAverage, photocellReading);
      }
  }

  void temperatureSensor(){

      if(millis() >= temperaturePeriod.time_now + temperaturePeriod.rate){
      temperaturePeriod.time_now += temperaturePeriod.rate;
      float temp = sht31.readTemperature();
      
        if (! isnan(temp)) {  // check if 'is not a number'
          updateMovingAverage(&temperatureRollingAverage, temp);
      } else { 
          Serial.println("Failed to read temperature");
             }
      }
  }

  void humiditySensor(){

  if(millis() >= humidityPeriod.time_now + humidityPeriod.rate){
      humidityPeriod.time_now += humidityPeriod.rate;
        float hum = sht31.readHumidity();
      
         if (! isnan(hum)) {  // check if 'is not a number'
            updateMovingAverage(&humidityRollingAverage, hum);
       } else { 
            Serial.println("Failed to read humidity");
              }
    }

  }

  void updateMovingAverage(MyParameter* param, float value){

  param->total -= param->readings[param->readIndex];
  // Update stored value at register and total
  param->readings[param->readIndex] = value;
  param->total += value;
  param->readIndex++;

        if (param->readIndex >= NUM_READINGS) {
          param->readIndex = 0;
        }
}

  void printRollingAverageUltrasonic(){

          if(millis() >= ultrasonicRollingPeriod.time_now + ultrasonicRollingPeriod.rate){
          ultrasonicRollingPeriod.time_now += ultrasonicRollingPeriod.rate;
          Serial.print("rolling/ ");
          Serial.print("distance/ ");
          Serial.println(ultrasonicRollingAverage.total / NUM_READINGS);
     }
  }

  void printRollingAveragePhotoresistor(){

          if(millis() >= photoresistorRollingPeriod.time_now + photoresistorRollingPeriod.rate){
          photoresistorRollingPeriod.time_now += photoresistorRollingPeriod.rate;
          Serial.print("rolling/ ");
          Serial.print("light/ ");
          Serial.println(photoresistorRollingAverage.total / NUM_READINGS);
    }
  }

  void printRollingAverageTemperature(){

          if(millis() >= temperatureRollingPeriod.time_now + temperatureRollingPeriod.rate){
          temperatureRollingPeriod.time_now += temperatureRollingPeriod.rate;
          Serial.print("rolling/ ");
          Serial.print("temp/ ");
          Serial.println(temperatureRollingAverage.total / NUM_READINGS);
    }
  }

  void printRollingAverageHumidity(){

          if(millis() >= humidityRollingPeriod.time_now + humidityRollingPeriod.rate){
          humidityRollingPeriod.time_now += humidityRollingPeriod.rate;
          Serial.print("rolling/ ");
          Serial.print("hum/ ");
          Serial.println(humidityRollingAverage.total / NUM_READINGS);
    }
  }

  void heater(){

      if(millis() >= heaterToggle.time_now + heaterToggle.rate){
      heaterToggle.time_now += heaterToggle.rate;
      enableHeater = !enableHeater;
      sht31.heater(enableHeater);
      Serial.print("Heater Enabled State: ");
        if (sht31.isHeaterEnabled())
          Serial.println("ENABLED");
        else
          Serial.println("DISABLED");
      }
  }