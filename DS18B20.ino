#include <OneWire.h>
#include <DallasTemperature.h>

#define BAUD_RATE               9600
#define TEMPERATURE_PRECISION   12  // 12 bits precision
#define TEMPERATRE_ERROR        185

// Pin locations
#define ONE_WIRE_BUS            2   // Sensor data wire in pin 2
#define LED_SENSOR_ERROR        12  // Pin 12 for error LED

// OneWire initialization
OneWire oneWire(ONE_WIRE_BUS);

// Initialize Dallas with reference to OneWire
DallasTemperature sensors(&oneWire);

// Address for tank sensor
DeviceAddress tankSensor;

bool ERROR_STATE;

void setup() 
{
  setupErrorLED();
  setupLibraries();
  setupSensor();
}

void loop() 
{
  if (ERROR_STATE)
  {
    setupSensor();
  }
  else 
  {
    processSensor(); 
  }

  delay(1000);
}

void setupLibraries()
{
  // Start serial port
  Serial.begin(BAUD_RATE);

  // Start dallas library
  sensors.begin();
}

void setupErrorLED()
{
  pinMode(LED_SENSOR_ERROR, OUTPUT);
  
  ERROR_STATE = LOW;
  setErrorLED();
}

void setupSensor()
{
  if (sensors.getAddress(tankSensor, 0))
  {
    sensors.setResolution(tankSensor, TEMPERATURE_PRECISION);
    ERROR_STATE = LOW;
  }
  else
  {
    Serial.println("SENSOR NOT FOUND");
    ERROR_STATE = HIGH;
  }
}

void setErrorLED()
{
  digitalWrite(LED_SENSOR_ERROR, ERROR_STATE);
}

void processSensor()
{
  sensors.requestTemperatures();
  
  float tempF = sensors.getTempF(tankSensor);

  if (tempF == DEVICE_DISCONNECTED_F || tempF == TEMPERATRE_ERROR)
  {
    ERROR_STATE = HIGH;

    Serial.println("SENSOR READ ERROR");
  }
  else 
  {
    ERROR_STATE = LOW;
    
    outputTemperature(tempF);
  }

  setErrorLED();
}

void outputTemperature(float tempF)
{
  // Print Fahrenheit
  Serial.print("f: ");
  Serial.print(tempF);
  Serial.print(", ");

  // Print Celsius
  Serial.print("c: ");
  Serial.print(DallasTemperature::toCelsius(tempF));
  Serial.println();
}
