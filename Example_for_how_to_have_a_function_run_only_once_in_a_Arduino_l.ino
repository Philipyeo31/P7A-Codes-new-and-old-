#include <LiquidCrystal.h>
#define PowerLED_PIN 12
#define PowerSwitch_PIN 13
#define SensorLED_PIN 10
#define SensorSwitch_PIN 11
const int rs = 4, en = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int caseState = 0;
bool CLR = false;
int clrFlag = 1;
int prevcaseState = 0;

void setup() 
{
  pinMode(PowerLED_PIN, OUTPUT);
  pinMode(SensorLED_PIN, OUTPUT);
  pinMode(SensorSwitch_PIN, INPUT);
  pinMode(PowerSwitch_PIN, INPUT);
  lcd.begin(16,2);
  lcd.print("");
}

void loop() {

  ( (digitalRead(PowerSwitch_PIN) == HIGH) && (digitalRead(SensorSwitch_PIN) == HIGH) ) ? (caseState = 2, CLR = true) : ( (digitalRead(PowerSwitch_PIN) == HIGH) ? (caseState = 1, CLR = true) : (caseState = 0, CLR = true) );

  //basically an if else statement, essentially  <this statement true?> ? <if true, do this> : <else, do this> 


  if (caseState != prevcaseState){
    clrFlag=1;
  }

  //detect change of caseState and sets flag to 1

  if (CLR == true && clrFlag == 1){
    lcd.clear();
  }

  //only clears when flag is == 1

   if (clrFlag >10){
    clrFlag = 2; //set to 2 so it doesn't keep flashing the screen when set to 1
   }
    
  switch(caseState){
    case (0):
      digitalWrite(PowerLED_PIN, LOW);
      digitalWrite(SensorLED_PIN, LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      break;

      // 0, both switches are off, repeatedly clears lcd
    
    case (1):
      digitalWrite(PowerLED_PIN, HIGH);
      digitalWrite(SensorLED_PIN,LOW);
      lcd.setCursor(0,0);
      lcd.print("Device Ready");
      clrFlag++;  //clrFlag is increased from 1 to some other number so the clear lcd function cannot run when repeated
      CLR = false; //turn off the CLR
      prevcaseState = caseState; //sets the variable prevcaseState to the current one to show a change in case state for the clr function to run
      break;

      
      
    case (2):
      digitalWrite(PowerLED_PIN, HIGH);
      digitalWrite(SensorLED_PIN, HIGH);
      lcd.setCursor(0,0);
      lcd.print("Stick Detected");
      clrFlag++;
      CLR = false;
      prevcaseState = caseState;
      break;
  } //clrFlag++ so the clrFlag won't be = 1 when the next loop happens, so the screen won't flicker
}

//follow me on ig thx @flaffy_the_Dog
  
