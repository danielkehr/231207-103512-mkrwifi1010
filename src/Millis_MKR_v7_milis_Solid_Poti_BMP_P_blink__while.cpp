#include <Arduino.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>              // Arduino SPI library
#include <Adafruit_BMP280.h>  // Tempsensor
// Anschluss Tempsensor via I2C -- an SCL und SDA

// ST7789 TFT module connections -- ansonsten SPI:   MOSID8=DIN;  SCK9=CLK;
#define TFT_CS 3   // define chip select pin
#define TFT_DC 4   // define data/command pin
#define TFT_RST 5  // define reset pin, or set to -1 and connect to Arduino RESET pin
//
// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

Adafruit_BMP280 bmp;

bool DEBUG = false; //lalala

// Constants !!! ///
//const byte LED_PIN_1 = 5;
const byte BUTTON_PIN_1 = 6;
const byte POTI_PIN_1 = A5;
const byte LED_PIN_1 = 16;
const byte LED_PIN_2 = 17;
const byte LED_PIN_3 = 18;
const byte LED_PIN_4 = 19;

//const byte TEMP_READ_1 = 11;
//const int TEMP_READ = 21;
const unsigned long zeit_1 = 996;
const unsigned long zeit_2 = 400;
const unsigned int interval = 100;

unsigned int potiValue = 0;
unsigned int previousPotiPosition = 0;
unsigned int potiPosition = 0;

unsigned int xAchsePoti_1 = 0;
unsigned int xAchseTaste_1 = 20;
unsigned int yAchseTaste_1 = 20;
unsigned int xAchseTaste_2 = 20;
unsigned int yAchseTaste_2 = 80;


byte xTempWert = 95;
byte yTempWert = 158;
byte xPressWert = 95;
byte yPressWert = 188;

float currentTemp;
float tempValue;
float currentPress;
float pressValue;


// variable
bool mediaButton = true;
//bool buttonState = true;
bool previousButtonState = true;
unsigned long previousTime_1 = 0;
unsigned long previousTime_2 = 0;
float temp = 0;
//float mVolt = 0;

void block(unsigned int xAchsePoti_1 = potiPosition) {
  // tft.fillRoundRect(xAchsePoti_1 + 55, 60, 205, 16, 0, ST77XX_BLACK);
  // tft.fillRoundRect(xAchsePoti_1 - 205, 60, 205, 16, 0, ST77XX_BLACK);
  tft.fillRoundRect(0, 60, 260, 20, 0, ST77XX_BLACK);  // es flimmert, wenn diese Funktion genutzt wird!
  tft.fillRoundRect(xAchsePoti_1, 60, 55, 15, 20, ST77XX_GREEN);
}

void mediabuttons_reverse(unsigned int xPos_1 = xAchseTaste_1, unsigned int yPos_1 = yAchseTaste_1, unsigned int xPos_2 = xAchseTaste_2, unsigned int yPos_2 = yAchseTaste_2) {
  // pause color
  tft.fillRoundRect(xPos_2, yPos_2, 54, 35, 5, ST77XX_WHITE);  // Hintergrund
  tft.fillRoundRect(xPos_1, yPos_1, 54, 35, 5, ST77XX_WHITE);
  tft.fillRoundRect(xPos_2 + 10, yPos_2 + 6, 12, 25, 1, ST77XX_RED);
  tft.fillRoundRect(xPos_2 + 30, yPos_2 + 6, 12, 25, 1, ST77XX_RED);
  // play reverse color
  tft.fillTriangle(xPos_1 + 45, yPos_1 + 6, xPos_1 + 45, yPos_1 + 30, xPos_1 + 5, yPos_1 + 16, ST77XX_BLUE);
  // 1: position x1 ; 2: position Y1 3: position x2; 4 position y2; 5: x3; 6: y3 ;
}
void mediabuttons_on(unsigned int xPos_1 = xAchseTaste_1, unsigned int yPos_1 = yAchseTaste_1, unsigned int xPos_2 = xAchseTaste_2, unsigned int yPos_2 = yAchseTaste_2) {
  // play
  tft.fillRoundRect(xPos_2, yPos_2, 54, 35, 5, ST77XX_WHITE);  // Hintergrund
  tft.fillRoundRect(xPos_1, yPos_1, 54, 35, 5, ST77XX_WHITE);
  tft.fillTriangle(xPos_1 + 10, yPos_1 + 6, xPos_1 + 10, yPos_1 + 30, xPos_1 + 50, yPos_1 + 14, ST77XX_RED);
  tft.fillRoundRect(xPos_2 + 10, yPos_2 + 6, 12, 25, 1, ST77XX_GREEN);
  tft.fillRoundRect(xPos_2 + 28, yPos_2 + 6, 12, 25, 1, ST77XX_GREEN);
}

void testSystem() {
  for (int i = 0; i < 3; i = i + 1) {
    tft.drawCircle(120, 120, 80, ST77XX_COLMOD);
    delay(20);
    tft.drawCircle(120, 120, 60, ST77XX_BLUE);
    delay(20);
    tft.drawCircle(120, 120, 40, ST77XX_CYAN);
    delay(20);
    tft.drawCircle(120, 120, 20, ST77XX_DISPON);
    delay(20);
    tft.drawCircle(120, 120, 5, ST77XX_MAGENTA);
    delay(30);
    tft.fillScreen(ST77XX_BLACK);
  }
  Serial.println("Graphic Test ok");

  byte blink = 5;
  for (int i = 0; i < blink; i = i + 1) {
    switch (i) {
      case (1):
        for (int b = 1; b < blink; b++) {
          tft.fillCircle(120, 120, 20, ST77XX_CYAN);
          digitalWrite(LED_PIN_1, HIGH);
          if (DEBUG) Serial.println("blink !");
          delay(200);
          tft.fillScreen(ST77XX_BLACK);
          digitalWrite(LED_PIN_1, LOW);
          delay(100);
        }
        break;
      case (2):
        for (int b = 1; b < blink; b++) {
          tft.fillCircle(120, 120, 40, ST77XX_GREEN);
          digitalWrite(LED_PIN_2, HIGH);
          if (DEBUG) Serial.println("blink2 !");
          delay(100);
          tft.fillScreen(ST77XX_BLACK);
          digitalWrite(LED_PIN_2, LOW);
          delay(10);
        }
        break;
      case (3):
        for (int b = 2; b < blink; b++) {
          tft.fillCircle(120, 120, 60, ST77XX_CASET);
          digitalWrite(LED_PIN_3, HIGH);
          delay(100);
          tft.fillScreen(ST77XX_BLACK);
          digitalWrite(LED_PIN_3, LOW);
          delay(10);
        }
        break;
      case (4):
        for (int b = 4; b < blink; b++) {
          tft.fillCircle(120, 120, 80, ST77XX_ORANGE);
          digitalWrite(LED_PIN_4, HIGH);
          delay(100);
          tft.fillScreen(ST77XX_COLMOD);
          digitalWrite(LED_PIN_4, LOW);
          delay(10);
        }
        break;
    }
  }

  Serial.println("Logiktest OK");
}

void setup() {
  while (!Serial) delay(100);  // wait for native usb
  Serial.begin(115200);
  Serial.println("initialized");
  delay(1000);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(POTI_PIN_1, INPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  //pinMode(TEMP_READ_1, INPUT);

  testSystem();

  Serial.println(F("BMP280 test"));
  // muss das Programm warten, bis Serial bereit ist ??? -- while (!serial) delay(100);
  bool status = false;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin();
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    Serial.print("SensorID was: 0x");
    Serial.println(bmp.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (true) delay(10);
  }
  
  Serial.println("initialized");
  // Default settings from datasheet. //

  //    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
  //                 Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  //                 Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
  //                 Adafruit_BMP280::FILTER_X16,      /* Filtering. */
  //                 Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


  // if  display has CS pin - work with SPI_MODE0
  tft.init(240, 240, SPI_MODE0);  // Init ST7789 display 240x240 pixel
  tft.setRotation(1);

  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(120, 20);
  tft.print("sleeping....");
/*
int buttonCount;
  while (buttonCount <= 2){
    if (digitalRead (BUTTON_PIN_1) == LOW) {
      buttonCount = buttonCount +1;
      delay (100);
    }
  }
  */
  
  mediabuttons_on();

  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.setCursor(10, 124);
  tft.println("Laufzeit: ");
  tft.setCursor(10, 154);
  tft.setTextColor(ST77XX_RED);
  tft.println("Temp: ");
  tft.setCursor(10, 184);

  tft.setTextColor(ST77XX_CASET);
  tft.println("QNH: ");


  // feststellen der Zeit
  //uint16_t time = millis();
  //mediabuttons_on();
  //time = millis() - time;
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
}



void loop() {
  

  //tft.setCursor(180, 120);
  //tft.setTextColor(ST77XX_BLACK);
  //tft.print("  ");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(175, 128);
  tft.fillRoundRect(171, 115, 150, 30, 5, ST77XX_BLACK);
  tft.print(millis() / 1000);
  unsigned long currentTime = millis();
  //if (currentTime - previousTime_1 >= zeit_1) {
  //  tft.fillRoundRect(xPressWert, yPressWert, 160, 30, 0, ST77XX_BLACK);
  //}
  //tft.setCursor(180, 120);
  //tft.print(millis() / 1000);
  /*
    if (currentTime >= zeit_1 + interval) {
      //if (DEBUG) Serial.println(currentTime);
      previousTime_1 = currentTime;
    }
  //}
  */

  // Taster Solid State und Flimmerfrei durch Integration der mediabuttons in die if Abfrage //

  if (mediaButton == true && digitalRead(BUTTON_PIN_1) == LOW) {
    //if (buttonState != previousButtonState) {
    Serial.println("Play...");
    if (DEBUG) Serial.print("Mediabutton: ");
    Serial.println(mediaButton);
    mediabuttons_on(xAchseTaste_1);
    delay(200);
    mediaButton = !mediaButton;
    if (DEBUG) Serial.print("Mediabutton: ");
    Serial.println(mediaButton);
    //buttonState = !buttonState;
  }
  if (mediaButton == false && digitalRead(BUTTON_PIN_1) == LOW) {
    Serial.println("Reverse...");
    if (DEBUG) Serial.print("Mediabutton: ");
    if (DEBUG) Serial.println(mediaButton);
    if (DEBUG) delay(1000);
    mediabuttons_reverse();
    mediaButton = !mediaButton;
    if (DEBUG) Serial.print("Mediabutton: ");
    if (DEBUG) Serial.println(mediaButton);
    delay(200);
    //buttonState = !buttonState;
  }

  // Poti zum Bewegen des Blocks //
  potiValue = analogRead(POTI_PIN_1);
  potiPosition = map(potiValue, 0, 1023, -60, 240);
  // if Funktion zum entflimmern des Blocks, wenn Block mit schwarze Streifen überschrieben wird, flimmert trotzdem etwas //
  if (previousPotiPosition != potiPosition) {
    block();
    //if (DEBUG) Serial.println(potiPosition);
  }
  // Wenn Block mit parallel laufenden Streifen läuft,
  //block();
  previousPotiPosition = potiPosition;

  byte potiInterval = map(potiPosition, -60, 238, 1, 7);
  //byte lastpotiInterval = 0;
  //if (potiInterval != lastpotiInterval) {
  /*  
  switch (potiInterval) {
    case 1:
      tft.fillRoundRect(1, 230, 240, 60, 1, ST77XX_WHITE);
      break;
    case 2:
      tft.fillRoundRect(1, 230, 240, 60, 1, ST77XX_BLACK);
      tft.fillRoundRect(xAchsePoti_1 = potiPosition - 30, 230, 60, 20, 30, ST77XX_ORANGE);
      break;
    case 3:
      tft.fillRoundRect(1, 230, 280, 60, 1, ST77XX_BLACK);
      tft.fillRoundRect(xAchsePoti_1 = potiPosition - 15, 230, 60, 20, 30, ST77XX_MAGENTA);
      break;
    case 4:
      tft.fillRoundRect(1, 230, 280, 60, 1, ST77XX_BLACK);
      tft.fillRoundRect(xAchsePoti_1 = potiPosition, 230, 60, 20, 30, ST77XX_CYAN);
      break;
    case 5:
      tft.fillRoundRect(1, 230, 280, 60, 1, ST77XX_BLACK);
      tft.fillRoundRect(xAchsePoti_1 = potiPosition - 70, 230, 60, 20, 30, ST77XX_NORON);
      break;
    case 6:
      tft.fillRoundRect(1, 230, 280, 60, 1, ST77XX_BLACK);
      tft.fillRoundRect(xAchsePoti_1 = potiPosition - 80, 230, 60, 20, 30, ST77XX_YELLOW);
      break;
    case 7:
      tft.fillRoundRect(1, 230, 280, 60, 2, ST77XX_GREEN);
      break;
    default:
      break;
  }
  */
  //}
  /*
  int potiSumme;
  //int potiZyklus = 0;
  if (previousPotiPosition != potiPosition) {
    int potiSumme = previousPotiPosition + potiPosition;
  }
  while (potiSumme <= 1023) {
    tft.fillCircle(20, 200, 20, ST77XX_GREEN);
    if (mediaButton == true) {
      tft.fillCircle(20, 200, 20, ST77XX_RED);
    }
  }
  */

  /*
  if (DEBUG) {
    Serial.println(tempValue);
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); // Adjusted to local forecast! 
    Serial.println(" m");

    Serial.println();
  } */
  currentTemp = bmp.readTemperature();

  if (currentTemp != tempValue) {
    tft.fillRoundRect(xTempWert, yTempWert, 80, 30, 0, ST77XX_BLACK);
    tft.setCursor(xTempWert, yTempWert);
    tft.print(currentTemp);
    tft.print("    C");
    Serial.println(currentTemp);
  }
  currentPress = bmp.readPressure();
  //Serial.print("vor -if- Pressure: ");
  //Serial.println(currentPress);

  currentPress = currentPress * 10000;
  int roundPress = round(currentPress);
  //roundPress/10000
  currentPress = roundPress / 10000 / 100;


  //if (currentTime - previousTime_2 >= zeit_2 && currentPress != pressValue) {
  if (currentPress != pressValue) {
    //if (currentTime - previousTime_1 >= zeit_2) {
    tft.fillRoundRect(xPressWert, yPressWert, 160, 30, 0, ST77XX_BLACK);
    //if (currentTime >= zeit_2 + interval) {
    //tft.fillRoundRect(xPressWert, yPressWert, 160, 30, 0, ST77XX_BLACK);
    tft.setCursor(xPressWert, yPressWert);
    tft.print(currentPress);
    tft.print(" hPa");
    //Serial.println(bmp.readPressure());
    previousTime_1 = currentTime;
    //}
  }
  //}
  //if (currentTime - previousTime_1 >= zeit_2) {
  //Serial.println(currentTime);
  //Serial.println(currentPress);
  ///Serial.println(bmp.readPressure());

  //}

  //}
  currentTime = previousTime_1;
  currentTime = previousTime_2;
  //previousTime_1 = currentTime;
  previousTime_2 = currentTime;
  tempValue = bmp.readTemperature();
  pressValue = bmp.readPressure();
  //currentPress = pressValue;


  /// Serielle Steuerung des Arduino ///
  //byte incommingInfo;

  byte bytesinSerialBuffer = Serial.available();
  if (DEBUG) Serial.println(bytesinSerialBuffer);
  if (Serial.available() > 0) {
    byte incommingInfo = Serial.read();
    switch (incommingInfo) {
      case 'a':
        //xAchseTaste_1 = 120;
        tft.drawRoundRect(10, 10, 120, 200, 0, ST77XX_BLACK);
        delay(500);
        mediabuttons_on(xAchseTaste_1 + 60, yAchseTaste_1);
        Serial.println("Taste 1");
        break;
      case 'b':
        xAchseTaste_1 = 20;
        Serial.println("Taste 2");
        tft.drawRoundRect(20, 20, 240, 200, 5, ST77XX_BLACK);
        delay(1000);
        break;
      case 'c':
        tft.drawRoundRect(20, 20, 240, 200, 5, ST77XX_BLACK);
        break;
      default:
        break;
    }
  }

  //tft.setCursor(20, 220);
  //tft.print(incommingInfo);
}
