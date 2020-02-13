#include <EEPROM.h>
#include "SPI.h"
#include "ILI9341_t3.h"
#include <SD.h>
#include <Wire.h>
#include <URTouch.h>
#define  BLACK      0x0000
#define  WHITE      0xFFFF
#define  RED        0xF800
#define GREEN      0x07E0
#define BLUE       0x001F
#define CYAN       0x07FF
#define MAGENTA    0xF81F
#define YELLOW     0xFFE0
#define ORANGE     0xFC00
#define TFT_DC  9
#define TFT_CS 10

#define TFT_RST    6  // 255 = unused, connect to 3.3V
#define TFT_MOSI     11
#define TFT_SCLK    13
#define TFT_MISO    12
#define SD_CS    17          // Chip select line for SD card
#define led    20  
URTouch  myTouch( 4, 3, 2, 22, 23);
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

int tempC;
int pinLM35 = 28;
int led_red = 24;
int led_yallow = 25;
int led_blue = 26;
int led_green = 27;
int hot = 30;
int motor = 33;

int led_back = 20;
int gsmon= 18;
int pw= 19;
int pwf= 5;

int x, y;
void setup() {

  pinMode(19,OUTPUT);pinMode(20,OUTPUT);
  digitalWrite(19,1);
     digitalWrite(20,1);   
 Serial.begin(9600);
tft.setTextWrap(false);

      tft.begin ();
      tft.setTextWrap(false);
      tft.fillScreen(BLACK);
      tft.setRotation(0);
      tft.setTextSize(6);
      tft.setTextColor (WHITE);
        myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
      tft.setCursor (20, 0);
      tft.print("21:30"); //test lcd text

    tft.setCursor (20, 60);tft.setTextSize(2); tft.print("Friday");
    tft.setCursor (20, 80);tft.setTextSize(2); tft.print("September 27");
    
      tft.fillRect(0, 100, 240, 45,  0x867F);
        tft.setCursor (20, 115);tft.setTextSize(2); tft.print("Home IOT");
        tft.fillRect(0, 155, 240, 45,  0x867F);
        tft.setCursor (20, 170);tft.setTextSize(2); tft.print("Phone");
        tft.fillRect(0, 210, 240, 45,  0x867F);
        tft.setCursor (20, 225);tft.setTextSize(2); tft.print("Message");
        tft.fillRect(0, 265, 240, 45,  0x867F);
        tft.setCursor (20, 280);tft.setTextSize(2); tft.print("Settings");

          tft.fillRect(186, 50, 40, 20,  WHITE);
tft.fillRect(183, 55, 3, 10,  WHITE);
    tft.setCursor (190, 80);tft.setTextSize(2); tft.print("73%");
   //tft.fillRect(189, 52, 10, 16,  RED); //1
//tft.fillRect(201, 52, 10, 16,  RED);//2
//tft.fillRect(213, 52, 10, 16,  RED);//3
    
}

void loop() {  
  while (true)
  {
    if (myTouch.dataAvailable())
    {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
      
      if ((y>=0) && (y<=240))  // Upper row
      {
        if ((x>=10) && (x<=50))  // Settings: 1
        {
tft.fillRect(189, 52, 10, 16,  RED);
        }
         if ((x>=70) && (x<=100))  // Message: 2
        {
tft.fillRect(201, 52, 10, 16,  RED);
        }
        if ((x>=125) && (x<=155))  // Phone: 3
        {
      tft.fillRect(213, 52, 10, 16,  RED);
        }
      if ((x>=180) && (x<=220))  // settings: 3
        {
          tft.fillRect(186, 50, 40, 20,  WHITE);
tft.fillRect(183, 55, 3, 10,  WHITE);
        }
      }

}}}
