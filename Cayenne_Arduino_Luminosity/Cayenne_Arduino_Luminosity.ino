#include <Adafruit_TSL2561_U.h>
#include <pgmspace.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

#include <CayenneSerial.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "tsxqyphrff";

// Virtual Pin of the TSL2561 widget.
#define VIRTUAL_PIN V3

// Address used to read from the TSL2561. This is determined by the ADDR pin on the TSL2561.
// If ADDR is unconnected it means the sensor will use TSL2561_ADDR_FLOAT (0x39) for the address.  See the TSL2561 datasheet for more info.
const int address = TSL2561_ADDR_FLOAT;

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(address, 12345);


void setup()
{
  Cayenne.begin(token);
  if (!tsl.begin())
  {
    CAYENNE_LOG("No TSL2561 detected");
  }

  tsl.enableAutoRange(true);
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}

void loop()
{
  Cayenne.run();
}

// This function is called when the Cayenne widget requests data for the Virtual Pin.
CAYENNE_OUT(VIRTUAL_PIN)
{
  // Send the command to get luminosity.
  sensors_event_t event;
  tsl.getEvent(&event);

  if (event.light)
  {
    // Send the value to Cayenne in lux.
    Cayenne.luxWrite(VIRTUAL_PIN, event.light);
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
    and no reliable data could be generated! */
    CAYENNE_LOG("No sensor data");
  }
}

