#include <DFRobot_GDL.h>
#include <DFRobot_Gesture.h>
#include <DFRobot_Picdecoder_SD.h>
#include <DFRobot_Touch.h>
#include <DFRobot_Type.h>
#include <DFRobot_UI.h>
#include <graphic.h>

//#include "Bitmap.h"
//#include "TestImage.h"

/* AVR series mainboard */
#define TFT_DC  49
#define TFT_CS  53
#define TFT_RST 48


//Declaration and Flags
int functionSelect = 0;
int page = 0;
int functionstep1 = 0;

bool drawnTextFlag = false;
bool drawnSetupFlag = false;






DFRobot_Touch_GT911 touch;
DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);






/**
   @brief Constructor
   @param gdl Screen object
   @param touch Touch object
*/
DFRobot_UI ui(&screen, &touch);

//void DFRobot_GDL::setRotation(uint8_t r);
void DFRobot_GDL::fillScreen(uint16_t color); //prototype to use the fillScreen unction later.



void setup() {
  Serial.begin(9600);
  screen.setRotation(0);
  ui.begin();
  ui.setTheme(DFRobot_UI::MODERN);
  ui.setBgColor(COLOR_RGB565_BLACK);
}


void loop() {
  ui.refresh();
  switch(page){
    case 0:
      displayHome();
      break;

    case 1:
      displayFunction1();
      break;    
  }
  
  
}

void runFunction(DFRobot_UI::sButton_t &btn){
  if (functionSelect == 1){
    page = 1; //sets the page to displayFunction1();
    drawnTextFlag = false; //need to reset flags to false to generate new screen.
    drawnSetupFlag = false;
    ui.refresh(); //refreshed ui to displayFunction1(); then runs the function below
    
    //function1(); //runs the actual function called function1

    }

  else if (functionSelect == 2){
    //run only loading of sub and developer

    ui.drawString(0, 0, "goodbye world", COLOR_RGB565_PINK , COLOR_RGB565_BLACK, 5, 0);
  }

  else if (functionSelect == 3){
    //run blah blah blah

    ui.drawString(0, 0, "Owie", COLOR_RGB565_PINK , COLOR_RGB565_BLACK, 5, 0);
  }
}

void returnToHomePage(DFRobot_UI::sButton_t &btn){
  page = 0;
  functionSelect = 0;
  ui.refresh();
}


//Home page display
void displayHome(){
  if (drawnSetupFlag == false){
    ui.setGestureArea(50, 0, 270, 355);
    screen.setRotation(0); //makes the screen upright, to generate the run button, because if diff. rotation, the location it is drawn and location it is detected will be diff.
    screen.fillScreen(COLOR_RGB565_BLACK);
    
    DFRobot_UI::sButton_t & btn1 = ui.creatButton();
        btn1.setText("run");
        btn1.bgColor = COLOR_RGB565_GREEN;
        btn1.setCallback(runFunction);
        ui.draw(&btn1,/**x=*/50,/**y=*/360,/*width*/270,/*height*/115);

    screen.drawRect(50, 0, 270, 355, COLOR_RGB565_WHITE);
    screen.setRotation(3);
    ui.drawString(135,305,"Press Green Button to run",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
    screen.setRotation(0);
    ui.refresh();
    drawnSetupFlag = true;
  }

  screen.setRotation(0);
  // Check for gestures 
  switch(ui.getGestures()){ 
    case ui.DDOWNGLIDE : //effecitvely left glide because orientation is set to 0.
      screen.setRotation(0);
      functionSelect += 1;
      drawnTextFlag = false;
      break;

    case ui.DUPGLIDE : 
      screen.setRotation(0);
      functionSelect -= 1;
      drawnTextFlag = false;
      break;
  }

  // Depending on function number, the following TEXT will be displayed
  if (functionSelect == 0){
    if (drawnTextFlag == false){ //Checks if text was drawn alr to prevent screen redrawing in loop.
      screen.setRotation(3);
      //Clear Previous Text (don't need to touch)
      screen.drawRect(0,0,480,48,COLOR_RGB565_BLACK);
      screen.fillRect(0,0,480,48,COLOR_RGB565_BLACK);
      //Draw / Generate Strings
      ui.drawString(5, 0, "Swipe LEFT in the box", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(5, 24, "below to begin.", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(400,305,"function: 0",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
      screen.setRotation(0);
      drawnTextFlag = true;
    }
  }

  if (functionSelect == 1){
    if (drawnTextFlag == false){ //Checks if text was drawn alr to prevent screen redrawing in loop.
      screen.setRotation(3);
      //Clear Previous Text (don't need to touch)
      screen.drawRect(0,0,480,48,COLOR_RGB565_BLACK);
      screen.fillRect(0,0,480,48,COLOR_RGB565_BLACK);
      //Draw / Generate Strings
      ui.drawString(5, 0, "Run full protocol", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(400,305,"function: 1",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
      screen.setRotation(0);
      drawnTextFlag = true;
    }
  }

  if (functionSelect == 2){
    if (drawnTextFlag == false){ //Checks if text was drawn alr to prevent screen redrawing in loop.
      screen.setRotation(3);
      //Clear Previous Text (don't need to touch)
      screen.drawRect(0,0,480,48,COLOR_RGB565_BLACK);
      screen.fillRect(0,0,480,48,COLOR_RGB565_BLACK);
      //Draw / Generate Strings
      ui.drawString(5, 0, "Load Substrate", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(400,305,"function: 2",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
      screen.setRotation(0);
      drawnTextFlag = true;
    }
  }

  if (functionSelect == 3){
    if (drawnTextFlag == false){ //Checks if text was drawn alr to prevent screen redrawing in loop.
      screen.setRotation(3);
      //Clear Previous Text (don't need to touch)
      screen.drawRect(0,0,480,48,COLOR_RGB565_BLACK);
      screen.fillRect(0,0,480,48,COLOR_RGB565_BLACK);
      //Draw / Generate Strings
      ui.drawString(5, 0, "Load Developer", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(400,305,"function: 3",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
      screen.setRotation(0);
      drawnTextFlag = true;
    }
  }

  if (functionSelect == 4){
    if (drawnTextFlag == false){ //Checks if text was drawn alr to prevent screen redrawing in loop.
      screen.setRotation(3);
      //Clear Previous Text (don't need to touch)
      screen.drawRect(0,0,480,48,COLOR_RGB565_BLACK);
      screen.fillRect(0,0,480,48,COLOR_RGB565_BLACK);
      //Draw / Generate Strings
      ui.drawString(5, 0, "Wash Sequence", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(400,305,"function: 4",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
      screen.setRotation(0);
      drawnTextFlag = true;
    }
  }
}

void displayFunction1(){
  if (drawnSetupFlag == false){

    //setup for displayFunction1()
    screen.fillScreen(COLOR_RGB565_BLACK);
    screen.setRotation(0);
    DFRobot_UI::sButton_t & btn1 = ui.creatButton();
        btn1.setText("STOP");
        btn1.bgColor = COLOR_RGB565_RED;
        btn1.setCallback(returnToHomePage);
        ui.draw(&btn1,/**x=*/50,/**y=*/360,/*width*/270,/*height*/115);

    screen.drawRect(50, 0, 270, 355, COLOR_RGB565_WHITE);
    screen.setRotation(3);
    ui.drawString(135,305,"Press Red Button to STOP",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
    screen.setRotation(0);
    
    drawnSetupFlag = true;
  }

  if (drawnTextFlag == false){ //Checks if text was drawn alr to prevent screen redrawing in loop.
      screen.setRotation(3);
      //Clear Previous Text (don't need to touch)
      screen.drawRect(0,0,480,48,COLOR_RGB565_BLACK);
      screen.fillRect(0,0,480,48,COLOR_RGB565_BLACK);
      //Draw / Generate Strings
      ui.drawString(5, 0, "Run full protocol", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(400,305,"function: 1",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
      screen.setRotation(0);
      drawnTextFlag = true;
  }

  // if (funtionstep1 == 0){
  //     ui.drawString(5, 0, "Run full protocol", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
  //}
}

void function1(){
  //blah blah blah
}
