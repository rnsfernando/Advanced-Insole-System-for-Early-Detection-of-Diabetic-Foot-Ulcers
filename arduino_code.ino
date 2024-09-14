#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ThingerESP8266.h>

#define USERNAME "in123"
#define DEVICE_ID "Esp8266"
#define DEVICE_CREDENTIAL "BMproject"
#define SSID "In" // Replace with your WiFi SSID
#define SSID_PASSWORD "19481234" // Replace with your WiFi password

Adafruit_ADS1115 ads; // Create an ADS1115 instance

#define ONE_WIRE_BUS D4 // Digital pin for DS18B20 sensors
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float pressure1, pressure2, pressure3; // Variables to store pressure readings
float tempC1, tempC2; // Variables to store temperature readings

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  sensors.begin();

  if (!ads.begin()) {
    Serial.println("ADS1115 not found!");
    while (1);
  }

  thing.add_wifi(SSID, SSID_PASSWORD);

  // Define resources for Thinger.io
  thing["pressure1"] >> [](pson &out) {
    out = pressure1; // Resource for pressure from ADS1115 channel 0
  };

  thing["pressure2"] >> [](pson &out) {
    out = pressure2; // Resource for pressure from ADS1115 channel 1
  };

  thing["pressure3"] >> [](pson &out) {
    out = pressure3; // Resource for pressure from ADS1115 channel 2
  };

  thing["temperature1"] >> [](pson &out) {
    out = tempC1; // Resource for temperature from DS18B20 sensor 1
  };

  thing["temperature2"] >> [](pson &out) {
    out = tempC2; // Resource for temperature from DS18B20 sensor 2
  };
}

void loop() {
  // Read analog values from ADS1115 channels
  int16_t adc0 = ads.readADC_SingleEnded(0); // Read value from channel 0
  int16_t adc1 = ads.readADC_SingleEnded(1); // Read value from channel 1
  int16_t adc2 = ads.readADC_SingleEnded(2); // Read value from channel 2

  // Map analog values to pressure values (adjust these mappings as needed)
  pressure1 = map(adc0, 4000, 5000, 0,100); // Map ADC value to pressure (0-100)
  pressure2 = map(adc1, 7000,  17000, 50, 100); // Map ADC value to pressure (0-100)
  pressure3 = map(adc2, 0,  8800, 0, 100); // Map ADC value to pressure (0-100)

  // Read temperatures from DS18B20 sensors
  sensors.requestTemperatures();
  tempC1 = sensors.getTempCByIndex(0); // Read temperature from sensor 1
  tempC2 = sensors.getTempCByIndex(1); // Read temperature from sensor 2

  Serial.print("Pressure 1: ");
  Serial.println(pressure1);

  Serial.print("Pressure 2: ");
  Serial.println(pressure2);

  Serial.print("Pressure 3: ");
  Serial.println(pressure3);

  Serial.print("Temperature 1: ");
  Serial.println(tempC1);

  Serial.print("Temperature 2: ");
  Serial.println(tempC2);

  thing.handle(); // Handle Thinger.io interactions
  delay(500);    // Adjust delay as needed
}
