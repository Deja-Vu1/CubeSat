/***************************************************************************

  _____       __        ____       __
  / ___/__ __ / /  ___  / __/___ _ / /_
  / /__ / // // _ \/ -_)_\ \ / _ `// __/
  \___/ \_,_//_.__/\__//___/ \_,_/ \__/

  Turkey / Stars of North Team
  github: https://github.com/Deja-Vu1/CubeSat
 ***************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"
#include <SoftwareSerial.h>


#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define BMP_SCK  (10)
#define BMP_MISO (9)
#define BMP_MOSI (8)
#define BMP_CS   (7)
unsigned long myTime;

File myFile;

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

void setup() {
  Serial.begin(9600);
  if (!SD.begin(6)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  dht.begin();

  //if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  myFile = SD.open("data.txt", FILE_WRITE);
  if (myFile) {
    myFile.print("------[Turkey / Stars of North]------\n");
    myFile.close();
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  Serial.print("start");
  myFile = SD.open("data.txt", FILE_WRITE);
  Serial.print("Opened");
  if (myFile) {
    Serial.print("\ngo to write");
    myTime = millis();
    myFile.print(float(myTime) / float(1000));
    bmp280();
    dht11();
    myFile.print("\n");
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.print("\nerror opening test.txt");
  }

  delay(750);
}
void bmp280() {
  myFile.print(F("BMP(Temperature = "));
  myFile.print(bmp.readTemperature());
  myFile.print(" *C");

  myFile.print(F("  Pressure = "));
  myFile.print(bmp.readPressure());
  myFile.print(" Pa");

  myFile.print(F("  Approx altitude = "));
  myFile.print(bmp.readAltitude(1019.66)); /* Adjusted to local forecast! */
  myFile.print(" m)");
}
void dht11() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    myFile.print(F("DHT11(Failed to read from DHT sensor!)"));
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  myFile.print(F("DHT11(Humidity: "));
  myFile.print(h);
  myFile.print(F("%  Temperature: "));
  myFile.print(t);
  myFile.print(F("°C "));
  myFile.print(f);
  myFile.print(F("°F  Heat index: "));
  myFile.print(hic);
  myFile.print(F("°C "));
  myFile.print(hif);
  myFile.print(F("°F)"));
}
