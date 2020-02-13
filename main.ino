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
 pinMode(SD_CS, INPUT_PULLUP);
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
      Serial.print(F("Initializing SD card..."));
  tft.println(F("Init SD card..."));
  while (!SD.begin(SD_CS)) {
    Serial.println(F("failed to access SD card!"));
    tft.println(F("failed to access SD card!"));
    delay(2000);
  }
  
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


#define BUFFPIXEL 80


//===========================================================
// Try Draw using writeRect
void bmpDraw(const char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint16_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  uint16_t awColors[320];  // hold colors for one row at a time...

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if (!(bmpFile = SD.open(filename))) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            awColors[col] = tft.color565(r,g,b);
          } // end pixel
          tft.writeRect(0, row, w, 1, awColors);
        } // end scanline
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}



// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
