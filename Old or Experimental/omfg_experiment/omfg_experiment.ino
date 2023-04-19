#include "DFRobot_GDL.h"
#include "DFRobot_UI.h"
#include "Arduino.h"
#include "DFRobot_Touch.h"
/*M0*/

/* AVR series mainboard */

#define TFT_DC  8
#define TFT_CS  10
#define TFT_RST 9
#define TFT_BL  4
#define TOUCH_CS 11


/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst reset pin of the screen
 */

DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


//touch chip set
DFRobot_Touch_GT911 touch(/*cs=*/TOUCH_CS);


DFRobot_UI ui (& screen, & touch);

uint8_t value1 = 0;

// Callback function of progress bar1


/*void DFRobot_GDL::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  
}*/

void setup ()
{
  
  Serial.begin (115200);
  // Initialize UI 
  ui.begin ();
  ui.setTheme (DFRobot_UI :: MODERN);

  ui.drawString (/* x = */ 33, /* y = */ screen.height () / 5 * 4, "Page of loading", COLOR_RGB565_WHITE, ui.bgColor, /* fontsize = */ 2, /* Invert = */ 0);
  

  /////////////////////////////////////////////////////////////////////////////
  DFRobot_UI :: sButton_t & button1 = ui.creatButton ();
  /** User-defined progress bar parameters */
  //button1.setStyle (DFRobot_UI :: CIRCULAR);
  button1.fgColor = COLOR_RGB565_GREEN;
  button1.setCallback ();
  ui.draw (& button1, /* x = */ 100, /* y = */ 100);
  ////////////////////////////////////////////////////////////////////////////

  
}


void loop ()
{
  // Refresh all controls
  ui.refresh ();
}
