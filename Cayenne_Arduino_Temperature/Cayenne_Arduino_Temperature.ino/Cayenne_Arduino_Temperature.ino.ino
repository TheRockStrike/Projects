#include <CayenneSerial.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

// Virtual Pins of the BMP180 widget.
#define BAROMETER_PIN V0
#define TEMPERATURE_PIN V1

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "vgv59ait2q";

bool bmpSensorDetected = true;

void setup()
{
  //Baud rate can be specified by calling Cayenne.begin(token, 9600);
  Cayenne.begin(token);

  if (!bmp.begin())
  {
    CAYENNE_LOG("No BMP sensor detected");
    bmpSensorDetected = false;
  }
}

void loop()
{
  Cayenne.run();
}

// This function is called when the Cayenne widget requests data for the barometer's Virtual Pin.
CAYENNE_OUT(BAROMETER_PIN)
{
  if (bmpSensorDetected)
  {
    // Send the value to Cayenne in hectopascals.
    Cayenne.hectoPascalWrite(BAROMETER_PIN, bmp.readPressure());
  }

  else
  {
    CAYENNE_LOG("No BMP sensor detected");
  }
}

// This function is called when the Cayenne widget requests data for the temperature's Virtual Pin.
CAYENNE_OUT(TEMPERATURE_PIN)
{
  if (bmpSensorDetected)
  {
    // Send the value to Cayenne in Celsius.
    Cayenne.celsiusWrite(TEMPERATURE_PIN, bmp.readTemperature());
  }
  else
  {
    CAYENNE_LOG("No BMP sensor detected");
  }
}
