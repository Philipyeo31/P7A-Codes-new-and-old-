    #include "DFRobot_GDL.h"

    #define TFT_DC  2
    #define TFT_CS  3
    #define TFT_RST 4

    DFRobot_ST7789_240x360_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

    void setup() {
         Serial.begin(115200);
         screen.begin();
    }

    void loop(){
        ///Set font size to 4, font range 1-4
        screen.setTextSize(2);
        //Set screen color 
        screen.fillScreen(COLOR_RGB565_BLACK);
        screen.setFont(&FreeMono12pt7b);//Set font to FreeMono12pt7b
        screen.setCursor(/*x=*/10,/*y=*/120);
        //Set text color 
        screen.setTextColor(COLOR_RGB565_LGRAY);
        //Set to text auto-wrap mode, true=Auto-wrap, false=No auto-wrap
        screen.setTextWrap(true);
        screen.print("HelloWorld!");
        delay(500);
    }
