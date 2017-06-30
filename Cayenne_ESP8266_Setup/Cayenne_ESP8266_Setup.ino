#include "CayenneDefines.h"
#include "BlynkSimpleEsp8266.h"
#include "CayenneWiFiClient.h"
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial

char token[] = "mmlognh31j";
char ssid[] = "BlueTardis_EXT";
char password[] = "Dog188*Uz6";

void setup()
{
  Serial.begin(115200);
  Cayenne.begin(token, ssid, password);
}
void loop()
{
  Cayenne.run();
  delay(100);
}

