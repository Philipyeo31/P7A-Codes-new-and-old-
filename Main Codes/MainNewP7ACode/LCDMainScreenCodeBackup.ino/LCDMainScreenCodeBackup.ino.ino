#include <DFRobot_GDL.h>
#include <DFRobot_Gesture.h>
#include <DFRobot_Picdecoder_SD.h>
#include <DFRobot_Touch.h>
#include <DFRobot_Type.h>
#include <DFRobot_UI.h>
#include <graphic.h>
#include <LongerPump.h>


/* AVR series mainboard */
#define TFT_DC  49
#define TFT_CS  53
#define TFT_RST 48

//Declaration and Flags
unsigned short int functionSelect = 0;
unsigned short int page = 0;
unsigned short int functionstep1 = 0;



bool v1Toggle = false;
bool v2Toggle = false;
bool v3Toggle = false;
bool v4Toggle = false;
bool v5Toggle = false;
bool v6Toggle = false;
bool v7Toggle = false;
bool v8Toggle = false;
bool v9Toggle = false;
bool v10Toggle = false;
bool v11Toggle = false;


uint8_t valve1 = 41;
uint8_t valve2 = 40;
uint8_t valve3 = 39;
uint8_t valve4 = 38;
uint8_t valve5 = 37;
uint8_t valve6 = 36;
uint8_t valve7 = 35;
uint8_t valve8 = 34;
uint8_t valve9 = 33;
uint8_t valve10 = 32;
uint8_t valve11 = 31;

bool pump1Dir = false;
bool pump2Dir = false;
bool p1Toggle = false;
bool p2Toggle = false;


bool drawnTextFlag = false;
bool drawnSetupFlag = false;
bool drawnButtonFlag = false;

DFRobot_Touch_GT911 touch;
DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
DFRobot_UI ui(&screen, &touch);
void DFRobot_GDL::fillScreen(uint16_t color); //prototype to use the fillScreen function later.



void setup() {
  Serial.begin(9600, SERIAL_8E1);
  screen.setRotation(0);
  ui.begin();
  ui.setTheme(DFRobot_UI::MODERN);
  ui.setBgColor(COLOR_RGB565_BLACK);
  
  initialiseValvePins();
}


void loop() {
  ui.refresh();
  switch(page){
  
    case 0:
      //displayHome();
      displayFunction2();
      break;

    case 1:
      displayFunction1();
      break;    

    case 2:
      displayFunction2();
      break;

    
  }
}


/*
  ____        _   _                  ______                _   _                 
 |  _ \      | | | |                |  ____|              | | (_)                
 | |_) |_   _| |_| |_ ___  _ __     | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
 |  _ <| | | | __| __/ _ \| '_ \    |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 | |_) | |_| | |_| || (_) | | | |   | |  | |_| | | | | (__| |_| | (_) | | | \__ \
 |____/ \__,_|\__|\__\___/|_| |_|   |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                                                                 
Button Functions  
Functions for the buttons to call. specifically run and return to home. 

*/


void runFunction(DFRobot_UI::sButton_t &btn){
  
  if (functionSelect == 1){
    page = 1; //sets the page to displayFunction1();
    drawnTextFlag = false; //need to reset flags to false to generate new screen.
    drawnSetupFlag = false;
    ui.refresh(); //refreshed ui to displayFunction1(); then runs the function below
    //function1(); //runs the actual function called function1
    }

  else if (functionSelect == 2){
    page = 2; //sets the page to displayFunction1();
    drawnTextFlag = false; //need to reset flags to false to generate new screen.
    drawnSetupFlag = false;
    ui.refresh(); //refreshed ui to displayFunction1(); then runs the function below
    //function2();
  }

  else if (functionSelect == 3){
    //run blah blah blah

    ui.drawString(0, 0, "Owie", COLOR_RGB565_PINK , COLOR_RGB565_BLACK, 5, 0);
  }

  else{
    page = 0;
    functionSelect = 0;
    drawnTextFlag = false; //need to reset flags to false to generate new screen.
    drawnSetupFlag = false;
    ui.refresh();
  }
}

void returnToHomePage(DFRobot_UI::sButton_t &btn){
  page = 0;
  functionSelect = 0;
  drawnTextFlag = false; //need to reset flags to false to generate new screen.
  drawnSetupFlag = false;
  ui.refresh();
}

/*

  _____  _           _                 _    _                      
 |  __ \(_)         | |               | |  | |                     
 | |  | |_ ___ _ __ | | __ _ _   _    | |__| | ___  _ __ ___   ___ 
 | |  | | / __| '_ \| |/ _` | | | |   |  __  |/ _ \| '_ ` _ \ / _ \
 | |__| | \__ \ |_) | | (_| | |_| |   | |  | | (_) | | | | | |  __/
 |_____/|_|___/ .__/|_|\__,_|\__, |   |_|  |_|\___/|_| |_| |_|\___|
              | |             __/ |                                
              |_|            |___/                                 
Display Home
Home page contains all the swipe gesture functionality.

*/


//Home page display
void displayHome(){
  if (drawnSetupFlag == false){
    ui.setGestureArea(50, 0, 270, 355);
    screen.setRotation(0); //makes the screen upright, to generate the run button, because if diff. rotation 
    //the location it is drawn and location it is detected will be diff.
    screen.fillScreen(COLOR_RGB565_BLACK);

    functionSelect = 0;

    
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

  else if (functionSelect == 1){
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

  else if (functionSelect == 2){
    if (drawnTextFlag == false){ //Checks if text was drawn alr to prevent screen redrawing in loop.
      screen.setRotation(3);
      //Clear Previous Text (don't need to touch)
      screen.drawRect(0,0,480,48,COLOR_RGB565_BLACK);
      screen.fillRect(0,0,480,48,COLOR_RGB565_BLACK);
      //Draw / Generate Strings
      ui.drawString(5, 0, "Manual Mode", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(400,305,"function: 2",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
      screen.setRotation(0);
      drawnTextFlag = true;
    }
  }

  else if (functionSelect == 3){
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

  else if (functionSelect == 4){
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


/*
  _____  _           _                ______                _   _             
 |  __ \(_)         | |              |  ____|              | | (_)            
 | |  | |_ ___ _ __ | | __ _ _   _   | |__ _   _ _ __   ___| |_ _  ___  _ __  
 | |  | | / __| '_ \| |/ _` | | | |  |  __| | | | '_ \ / __| __| |/ _ \| '_ \ 
 | |__| | \__ \ |_) | | (_| | |_| |  | |  | |_| | | | | (__| |_| | (_) | | | |
 |_____/|_|___/ .__/|_|\__,_|\__, |  |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
              | |             __/ |                                           
              |_|            |___/                                            
Display function

This part should contain all the text/graphics/buttons that need to be drawn on screen. 

*/

void displayFunction1(){
  if (drawnSetupFlag == false){

  //setup for Page w/ stopButton
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
  //Edit function name / Header / title here
  if (drawnTextFlag == false){ //Checks if text was drawn alr to prevent screen redrawing in loop.
      screen.setRotation(3);
      //Clear Previous Text (don't need to touch)
      screen.drawRect(0,0,480,48,COLOR_RGB565_BLACK);
      screen.fillRect(0,0,480,48,COLOR_RGB565_BLACK);
      //Draw / Generate Strings
      ui.drawString(5, 0, "Run full protocol", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(400,305,"function: 1",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
      screen.setRotation(0);
      //Include Extra Functions here (draw one time type)


      drawnTextFlag = true;
  }
  //Include Functions to loop here


  // if (funtionstep1 == 0){
  //     ui.drawString(5, 0, "Run full protocol", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
  //}
}

void displayFunction2(){
  if (drawnSetupFlag == false){
  //setup for Page w/ stopButton
    screen.fillScreen(COLOR_RGB565_BLACK);
    screen.setRotation(0);
    DFRobot_UI::sButton_t & btn0 = ui.creatButton();
        btn0.setText("STOP");
        btn0.bgColor = COLOR_RGB565_RED;
        btn0.setCallback(returnToHomePage);
        ui.draw(&btn0,/**x=*/50,/**y=*/360,/*width*/270,/*height*/115);
    screen.drawRect(50, 0, 270, 355, COLOR_RGB565_WHITE);
    screen.setRotation(3);
    ui.drawString(135,305,"Press Red Button to STOP",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0);
    screen.setRotation(0);
    drawnSetupFlag = true;
  }
  //Edit function name / Header / title here
  if (drawnTextFlag == false){ //Checks if text was drawn alr to prevent screen redrawing in loop.
      screen.setRotation(3);
      //Clear Previous Text (don't need to touch)
      screen.drawRect(0,0,480,48,COLOR_RGB565_BLACK);
      screen.fillRect(0,0,480,48,COLOR_RGB565_BLACK);
      //Draw / Generate Strings
      ui.drawString(5, 0, "Manual Mode", COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 3, 0);
      ui.drawString(400,305,"function: 2",COLOR_RGB565_WHITE , COLOR_RGB565_BLACK, 1, 0); //Change Function Number Accordingly
      screen.setRotation(0);

    //Include Extra Objects and Drawings here (draw one time type)
    drawValvesAndColourForFunc2();
    drawButtonsForFunc2();


  //Pump drawings
    screen.drawRect(160, 190, 60, 60, COLOR_RGB565_WHITE);
    screen.drawCircle(190, 220, 20, COLOR_RGB565_WHITE);
    screen.fillCircle(190, 220, 19, COLOR_RGB565_RED);
    

    screen.drawRect(160, 100, 60, 60, COLOR_RGB565_WHITE);
    screen.drawCircle(190, 130, 20, COLOR_RGB565_WHITE);
    screen.fillCircle(190, 130, 19, COLOR_RGB565_RED);
    

    screen.setRotation(3);
    ui.drawString(245, 230, "PUMP1", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(335, 230, "PUMP2", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(215, 140, "CW", COLOR_RGB565_BLUE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(390, 140, "ACW", COLOR_RGB565_BLUE, COLOR_RGB565_BLACK, 1, 0);
    screen.setRotation(0);

  //Pump controls
    //switch
    DFRobot_UI::sSwitch_t &sw1 = ui.creatSwitch();
      sw1.bgColor = COLOR_RGB565_GREEN;
      sw1.setCallback(setPump1Dir);
      //Draw switch at specified position
      ui.draw(&sw1,/*x = */160,/*y = */260,/*width=*/60,/*height=*/10);

    DFRobot_UI::sSwitch_t &sw2 = ui.creatSwitch();
      sw2.bgColor = COLOR_RGB565_GREEN;
      sw2.setCallback(setPump2Dir);
      //Draw switch at specified position
      ui.draw(&sw2,/*x = */160,/*y = */80,/*width=*/60,/*height=*/10);

    //pump buttons
    DFRobot_UI::sButton_t & btn12 = ui.creatButton();
      btn12.bgColor = COLOR_RGB565_BLACK;
      btn12.setCallback(toggleP1);
      ui.draw(&btn12,/**x=*/160+17,/**y=*/190+17,/*width*/28,/*height*/28);

    DFRobot_UI::sButton_t & btn13 = ui.creatButton();
      btn13.bgColor = COLOR_RGB565_BLACK;
      btn13.setCallback(toggleP2);
      ui.draw(&btn13,/**x=*/160+17,/**y=*/100+17,/*width*/28,/*height*/28);

    drawnTextFlag = true;
  }
  //check if 

}

/*
   ____            _                    ______                _   _                 
 |  __ \          | |                  |  ____|              | | (_)                
 | |  | | ___  ___| | __ _ _ __ ___    | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
 | |  | |/ _ \/ __| |/ _` | '__/ _ \   |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 | |__| |  __/ (__| | (_| | | |  __/   | |  | |_| | | | | (__| |_| | (_) | | | \__ \
 |_____/ \___|\___|_|\__,_|_|  \___|   |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                                                                          
Declare Functions                       
This part should contain all the functions that should run when the run button is pressed on a selected page.

*/                                              



void function1(){
  //blah blah blah
}
/*


  _____                 _   _ _   _              ______                _   _                 
 |  __ \               | | (_) | (_)            |  ____|              | | (_)                
 | |__) |___ _ __   ___| |_ _| |_ ___   _____   | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
 |  _  // _ \ '_ \ / _ \ __| | __| \ \ / / _ \  |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 | | \ \  __/ |_) |  __/ |_| | |_| |\ V /  __/  | |  | |_| | | | | (__| |_| | (_) | | | \__ \
 |_|  \_\___| .__/ \___|\__|_|\__|_| \_/ \___|  |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
            | |                                                                              
            |_|                                                                              

Functions that are very similar to each other
*/

void drawPumpsForFunc2(){
  screen.drawRect(160, 190, 60, 60, COLOR_RGB565_WHITE);
  screen.drawCircle(190, 220, 20, COLOR_RGB565_WHITE);
  screen.fillCircle(190, 220, 19, COLOR_RGB565_RED);
  

  screen.drawRect(160, 100, 60, 60, COLOR_RGB565_WHITE);
  screen.drawCircle(190, 130, 20, COLOR_RGB565_WHITE);
  screen.fillCircle(190, 130, 19, COLOR_RGB565_RED);
  

  screen.setRotation(3);
  ui.drawString(245, 230, "PUMP1", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
  ui.drawString(335, 230, "PUMP2", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
  ui.drawString(215, 140, "CW", COLOR_RGB565_BLUE, COLOR_RGB565_BLACK, 1, 0);
  ui.drawString(390, 140, "ACW", COLOR_RGB565_BLUE, COLOR_RGB565_BLACK, 1, 0);
  screen.setRotation(0);
}

void drawPumpButtonsForFunc2(){
  //Pump controls
  //switch
  DFRobot_UI::sSwitch_t &sw1 = ui.creatSwitch();
    sw1.bgColor = COLOR_RGB565_GREEN;
    sw1.setCallback(setPump1Dir);
    //Draw switch at specified position
    ui.draw(&sw1,/*x = */160,/*y = */260,/*width=*/60,/*height=*/10);

  DFRobot_UI::sSwitch_t &sw2 = ui.creatSwitch();
    sw2.bgColor = COLOR_RGB565_GREEN;
    sw2.setCallback(setPump2Dir);
    //Draw switch at specified position
    ui.draw(&sw2,/*x = */160,/*y = */80,/*width=*/60,/*height=*/10);

  //pump buttons
  DFRobot_UI::sButton_t & btn12 = ui.creatButton();
    btn12.bgColor = COLOR_RGB565_BLACK;
    btn12.setCallback(toggleP1);
    ui.draw(&btn12,/**x=*/160+17,/**y=*/190+17,/*width*/28,/*height*/28);

  DFRobot_UI::sButton_t & btn13 = ui.creatButton();
    btn13.bgColor = COLOR_RGB565_BLACK;
    btn13.setCallback(toggleP2);
    ui.draw(&btn13,/**x=*/160+17,/**y=*/100+17,/*width*/28,/*height*/28);
}

void setPump1Dir(DFRobot_UI::sSwitch_t &sw1){
  if (sw1.state == 0) {
    pump1Dir = true;

  }
  else {
    pump1Dir = false;
  }
}
void setPump2Dir(DFRobot_UI::sSwitch_t &sw2){
  if (sw2.state == 0) {
    pump2Dir = false;

  }
  else {
    pump2Dir = true;
  }
}

void toggleP1(){
  if (p1Toggle == false){
    run(1,200,0,pump1Dir);
    p1Toggle = true;
    screen.fillCircle(190, 220, 19, COLOR_RGB565_GREEN);
    }

  else{
    stop(1);
    p1Toggle = false;
    screen.fillCircle(190, 220, 19, COLOR_RGB565_RED);
    }
}

void toggleP2(){
  if (p2Toggle == false){
    run(2,200,0,pump2Dir);
    p2Toggle = true;
    screen.fillCircle(190, 130, 19, COLOR_RGB565_GREEN);
    }

  else{
    stop(2);
    p2Toggle = false;
    screen.fillCircle(190, 130, 19, COLOR_RGB565_RED);
    }
}

//Draw Valves For Function 2
void drawValvesAndColourForFunc2(){
  //LEFT VALVES
    screen.drawCircle(80, 290, 20, COLOR_RGB565_WHITE); //v1
    screen.drawCircle(130, 310, 20, COLOR_RGB565_WHITE); //v2
    screen.drawCircle(180, 320, 20, COLOR_RGB565_WHITE); //v3
    screen.drawCircle(230, 310, 20, COLOR_RGB565_WHITE); //v4
    screen.drawCircle(280, 290, 20, COLOR_RGB565_WHITE); //v5

    //RIGHT VALVES
    screen.drawCircle(80, 70, 20, COLOR_RGB565_WHITE); //v6
    screen.drawCircle(120, 50, 20, COLOR_RGB565_WHITE); //v7
    screen.drawCircle(160, 30, 20, COLOR_RGB565_WHITE); //v8
    screen.drawCircle(200, 30, 20, COLOR_RGB565_WHITE); //v9
    screen.drawCircle(240, 50, 20, COLOR_RGB565_WHITE); //v10
    screen.drawCircle(280, 70, 20, COLOR_RGB565_WHITE); //v11

    //Colour in Valves
    screen.fillCircle(80, 290, 19, COLOR_RGB565_RED);
    screen.fillCircle(130, 310, 19, COLOR_RGB565_RED);
    screen.fillCircle(180, 320, 19, COLOR_RGB565_RED);
    screen.fillCircle(230, 310, 19, COLOR_RGB565_RED);
    screen.fillCircle(280, 290, 19, COLOR_RGB565_RED);

    screen.fillCircle(80, 70, 19, COLOR_RGB565_RED);
    screen.fillCircle(120, 50, 19, COLOR_RGB565_RED);
    screen.fillCircle(160, 30, 19, COLOR_RGB565_RED);
    screen.fillCircle(200, 30, 19, COLOR_RGB565_RED);
    screen.fillCircle(240, 50, 19, COLOR_RGB565_RED);
    screen.fillCircle(280, 70, 19, COLOR_RGB565_RED);


    screen.setRotation(3);
    ui.drawString(215, 80, "V1", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(195, 130, "V2", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(185, 180, "V3", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(195, 230, "V4", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(215, 280, "V5", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);

    ui.drawString(370, 80, "V6", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(390, 120, "V7", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(410, 160, "V8", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(410, 200, "V9", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(390, 240, "V10", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    ui.drawString(370, 280, "V11", COLOR_RGB565_WHITE, COLOR_RGB565_BLACK, 1, 0);
    screen.setRotation(0);
}

//draw buttons
void drawButtonsForFunc2(){
    DFRobot_UI::sButton_t & btn1 = ui.creatButton();
      btn1.bgColor = COLOR_RGB565_BLACK;
      btn1.setCallback(toggleV1);
      ui.draw(&btn1,/**x=*/80-14,/**y=*/290-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn2 = ui.creatButton();
      btn2.bgColor = COLOR_RGB565_BLACK;
      btn2.setCallback(toggleV2);
      ui.draw(&btn2,/**x=*/130-14,/**y=*/310-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn3 = ui.creatButton();
      btn3.bgColor = COLOR_RGB565_BLACK;
      btn3.setCallback(toggleV3);
      ui.draw(&btn3,/**x=*/180-14,/**y=*/320-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn4 = ui.creatButton();
      btn4.bgColor = COLOR_RGB565_BLACK;
      btn4.setCallback(toggleV4);
      ui.draw(&btn4,/**x=*/230-14,/**y=*/310-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn5 = ui.creatButton();
      btn5.bgColor = COLOR_RGB565_BLACK;
      btn5.setCallback(toggleV5);
      ui.draw(&btn5,/**x=*/280-14,/**y=*/290-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn6 = ui.creatButton();
      btn6.bgColor = COLOR_RGB565_BLACK;
      btn6.setCallback(toggleV6);
      ui.draw(&btn6,/**x=*/80-14,/**y=*/70-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn7 = ui.creatButton();
      btn7.bgColor = COLOR_RGB565_BLACK;
      btn7.setCallback(toggleV7);
      ui.draw(&btn7,/**x=*/120-14,/**y=*/50-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn8 = ui.creatButton();
      btn8.bgColor = COLOR_RGB565_BLACK;
      btn8.setCallback(toggleV8);
      ui.draw(&btn8,/**x=*/160-14,/**y=*/30-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn9 = ui.creatButton();
      btn9.bgColor = COLOR_RGB565_BLACK;
      btn9.setCallback(toggleV9);
      ui.draw(&btn9,/**x=*/200-14,/**y=*/30-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn10 = ui.creatButton();
      btn10.bgColor = COLOR_RGB565_BLACK;
      btn10.setCallback(toggleV10);
      ui.draw(&btn10,/**x=*/240-14,/**y=*/50-14,/*width*/28,/*height*/28);
    DFRobot_UI::sButton_t & btn11 = ui.creatButton();
      btn11.bgColor = COLOR_RGB565_BLACK;
      btn11.setCallback(toggleV11);
      ui.draw(&btn11,/**x=*/280-14,/**y=*/70-14,/*width*/28,/*height*/28);

}



//Toggle Valves
void toggleV1(){
  if (v1Toggle == false){
    digitalWrite(valve1, LOW);
    v1Toggle = true;
    screen.fillCircle(80, 290, 19, COLOR_RGB565_GREEN);
    }

  else{
    digitalWrite(valve1, HIGH);
    v1Toggle = false;
    screen.fillCircle(80, 290, 19, COLOR_RGB565_RED);
    }
}

void toggleV2(){
  if (v2Toggle == false){
    digitalWrite(valve2, LOW);
    v2Toggle = true;
    screen.fillCircle(130, 310, 19, COLOR_RGB565_GREEN);} 
  else{
    digitalWrite(valve2, HIGH);
    v2Toggle = false;
    screen.fillCircle(130, 310, 19, COLOR_RGB565_RED);}}
void toggleV3(){
  if (v3Toggle == false){
    digitalWrite(valve3, LOW);
    v3Toggle = true;
    screen.fillCircle(180, 320, 19, COLOR_RGB565_GREEN);} 
  else{
    digitalWrite(valve3, HIGH);
    v3Toggle = false;
    screen.fillCircle(180, 320, 19, COLOR_RGB565_RED);}}
void toggleV4(){
  if (v4Toggle == false){
    digitalWrite(valve4, LOW);
    v4Toggle = true;
    screen.fillCircle(230, 310, 19, COLOR_RGB565_GREEN);} 
  else{
    digitalWrite(valve4, HIGH);
    v4Toggle = false;
    screen.fillCircle(230, 310, 19, COLOR_RGB565_RED);}}
void toggleV5(){
  if (v5Toggle == false){
    digitalWrite(valve5, LOW);
    v5Toggle = true;
    screen.fillCircle(280, 290, 19, COLOR_RGB565_GREEN);} 
  else{
    digitalWrite(valve5, HIGH);
    v5Toggle = false;
    screen.fillCircle(280, 290, 19, COLOR_RGB565_RED);}}
void toggleV6(){
  if (v6Toggle == false){
    digitalWrite(valve6, LOW);
    v6Toggle = true;
    screen.fillCircle(80, 70, 19, COLOR_RGB565_GREEN);} 
  else{
    digitalWrite(valve6, HIGH);
    v6Toggle = false;
    screen.fillCircle(80, 70, 19, COLOR_RGB565_RED);}}
void toggleV7(){
  if (v7Toggle == false){
    digitalWrite(valve7, LOW);
    v7Toggle = true;
    screen.fillCircle(120, 50, 19, COLOR_RGB565_GREEN);} 
  else{
    digitalWrite(valve7, HIGH);
    v7Toggle = false;
    screen.fillCircle(120, 50, 19, COLOR_RGB565_RED);}}
void toggleV8(){
  if (v8Toggle == false){
    digitalWrite(valve8, LOW);
    v8Toggle = true;
    screen.fillCircle(160, 30, 19, COLOR_RGB565_GREEN);} 
  else{
    digitalWrite(valve8, HIGH);
    v8Toggle = false;
    screen.fillCircle(160, 30, 19, COLOR_RGB565_RED);}}
void toggleV9(){
  if (v9Toggle == false){
    digitalWrite(valve9, LOW);
    v9Toggle = true;
    screen.fillCircle(200, 30, 19, COLOR_RGB565_GREEN);}
  else{
    digitalWrite(valve9, HIGH);
    v9Toggle = false;
    screen.fillCircle(200, 30, 19, COLOR_RGB565_RED);}}
void toggleV10(){
  if (v10Toggle == false){
    digitalWrite(valve10, LOW);
    v10Toggle = true;
    screen.fillCircle(240, 50, 19, COLOR_RGB565_GREEN);} 
  else{
    digitalWrite(valve10, HIGH);
    v10Toggle = false;
    screen.fillCircle(240, 50, 19, COLOR_RGB565_RED);}}
void toggleV11(){
  if (v11Toggle == false){
    digitalWrite(valve11, LOW);
    v11Toggle = true;
    screen.fillCircle(280, 70, 19, COLOR_RGB565_GREEN);}
  else{
    digitalWrite(valve11, HIGH);
    v11Toggle = false;
    screen.fillCircle(280, 70, 19, COLOR_RGB565_RED);}}

void initialiseValvePins(){
  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT);
  pinMode(valve4, OUTPUT);
  pinMode(valve5, OUTPUT);
  pinMode(valve6, OUTPUT);
  pinMode(valve7, OUTPUT);
  pinMode(valve8, OUTPUT);
  pinMode(valve9, OUTPUT);
  pinMode(valve10, OUTPUT);
  pinMode(valve11, OUTPUT);


  digitalWrite(valve1, HIGH);
  digitalWrite(valve2, HIGH);
  digitalWrite(valve3, HIGH);
  digitalWrite(valve4, HIGH);
  digitalWrite(valve5, HIGH);
  digitalWrite(valve6, HIGH);
  digitalWrite(valve7, HIGH);
  digitalWrite(valve8, HIGH);
  digitalWrite(valve9, HIGH);
  digitalWrite(valve10, HIGH);
  digitalWrite(valve11, HIGH);
}