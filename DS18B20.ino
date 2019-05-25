#include <OneWire.h>
#include <DallasTemperature.h>

#define BAUD_RATE               9600

#define ONE_WIRE_BUS            2   // Sensor data wire in pin 2
#define TEMPERATURE_PRECISION   12  // 12 bits precision
#define LED_TANK_ERROR          12  

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
    Serial.println("SENSOR DISCONNECTED");
    delay(1000);
    
    setupSensor();
    return;
  }
  
  sensors.requestTemperatures();
  processTankSensor();

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
  pinMode(LED_TANK_ERROR, OUTPUT);
  
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
    ERROR_STATE = HIGH;
  }
  
  setErrorLED();
}

void setErrorLED()
{
  digitalWrite(LED_TANK_ERROR, ERROR_STATE);
}

void processTankSensor()
{
  float tempF = sensors.getTempF(tankSensor);

  if (tempF == DEVICE_DISCONNECTED_F)
  {
    ERROR_STATE = HIGH;
    setErrorLED();
  }
  else 
  {
    outputTemperature('f', tempF);
    Serial.print(", ");
    outputTemperature('c', DallasTemperature::toCelsius(tempF));
    Serial.println();
  }
}

void outputTemperature(char measurement, float temp)
{
  Serial.print(measurement);
  Serial.print(": ");
  Serial.print(temp);
}

