#include <SPI.h>
#include <SD.h>

#define RT0 10000   // Odpor termistora
#define B 3977      // Beta faktor 
#define VCC 3.30    // Napájanie U= 3.3V
#define R 10000    // Odpor použitého rezistora R=10KΩ

float RT, VR, ln, TX, T0, VRT, teplota;
const int chipSelect = 53;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  T0 = 30 + 273.15;
  Serial.print("Inicializacia SD karty..");

  if (!SD.begin(chipSelect)) {
    Serial.println("Nacitavanie zlyhalo.");
    // don't do anything more:
    return;
  }
  Serial.println("Katra nacitana.");
}

void loop()
{
  String dataString = "";
  for (int analogPin = 0; analogPin < 1; analogPin++) {
     VRT = analogRead(analogPin);

  VRT = (3.30 / 1023.00) * VRT;      // Prepočet na volty
  VR = VCC - VRT;
  RT = VRT / (VR / R);               // Odpor RT

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0)));  // Teplota z termistora v Kelvinoch

  teplota = (TX - 273.15);                  // Prepočet z Kelvinov na °C
    
    dataString += String(teplota);
    if (analogPin > 1) {
      dataString += ",";
    }
  }
  File dataFile = SD.open("telota.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
    delay(500);
  }
  else {
    Serial.println("problem s otvenim telota.txt");
    delay(10000);
  }
}
