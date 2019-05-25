#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino_FreeRTOS.h>

// Sensor data wire in pin 2
#define ONE_WIRE_BUS    2
#define BAUD_RATE       9600

#define LED_TANK_ERROR  12

// OneWire initialization
OneWire oneWire(ONE_WIRE_BUS);

// Initialize Dallas with reference to OneWire
DallasTemperature sensors(&oneWire);

void setupLibraries()
{
  // Start serial port
  Serial.begin(BAUD_RATE);

  // Start dallas library
  sensors.begin();
}

void setup() 
{
  setupLibraries();

  pinMode(LED_TANK_ERROR, OUTPUT);
  digitalWrite(LED_TANK_ERROR, LOW);
}

void loop() 
{
  sensors.requestTemperatures();

  Serial.print("Temperature: ");
  Serial.println(sensors.getTempFByIndex(0));

  delay(1000);
}

