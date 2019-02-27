#include <SPI.h>
#include <SD.h>

#define RT0 10000   //Resistance of resistor
#define B 3977      //Beta factor 
#define VCC 3.30    //Power supply U= 3.3V
#define R 10000     //Resistance of the resistor used: R=10 KΩ

float RT, VR, ln, TX, T0, VRT, teplota;
const int chipSelect = 53;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;                 
  //wait for serial port to connect
  }

  T0 = 30 + 273.15;
  Serial.print("Initialization SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("Loading failed!");
      // don't do anything more:
    return;
  }
  Serial.println("Card loaded.");
}

void loop()
{
  String dataString = "";
  for (int analogPin = 0; analogPin < 1; analogPin++) {
     VRT = analogRead(analogPin);

  VRT = (3.30 / 1023.00) * VRT;      //Voltage conversion
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance RT

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0)));  //Temperature of termistor (K)
    
    dataString += String(teplota);
    if (analogPin > 1) {
      dataString += ",";
    }
  }
  File dataFile = SD.open("Temperature.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
    delay(500);
  }
  else {
    Serial.println("Problem with opening Temperature.txt");
    delay(10000);
  }
}
