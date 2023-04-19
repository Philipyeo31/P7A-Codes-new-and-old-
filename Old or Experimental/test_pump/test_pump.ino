#include "GravityPump.h"
#include "Button.h"//https://github.com/DFRobot/Button
#include <EEPROM.h>

GravityPump pump;
Button button;
bool run = true;
int debug = 1;


void setup()
{
    pump.setPin(13);
    button.init(7);
    Serial.begin(115200);
    pump.getFlowRateAndSpeed();
    
}

void loop()
{
    pump.update();
    button.update();
    if (debug == 1)
    //in debug mode the pump will do calibation.
    //if set the debug mode off then the function works.
    {
        pump.calFlowRate(.5);
        
        
    }

    if (debug == 2)
    //random testing
    {
        //float pumpDispense (float pVolume, int pSpeed){
          /* pSpeed = 0 to 180. pVolume is in mL
           * firs
           *  
           */
        
        
    }


    
    if (debug == 3){
        // EEPROM flowrate value
          int a = 0x24;
          int value;

          value = EEPROM.read(a);
          
          Serial.print(a);
          Serial.print("\t");
          Serial.print(value);
          Serial.println();
          
          a = a + 1;
          
          if (a == 512)
            a = 0;
          
          delay(500);
          
    }
    
    else
    {
        if(run)
        {
            
            float volume = 10; //0.5ml per sec
            float flowrate = 1;
            float runTimeA = volume/flowrate;
            
            //switch the function by using Comments.
            run = false;
            
            Serial.println(pump.timerPump(runTimeA));
            /*intervaltime = runTime*1000
             * 
             * for the flowpump thing, interval time = 1000*(volume/flowrate)
             * 
             * runTime = volume/flowrate
             * 
             * 
             */

            
            //Serial.println(pump.flowPump(10.00));
            //just put the number in ml then the pump will dosing the numbers of liquid to you.
            //and you can find the numbers from serial port.
            
            


        }

    }
    if(button.click())
    {
        Serial.println("click");
        //when you click the button the pump will stop immediately
        pump.stop();
        digitalWrite(4, LOW);
        
        
    }
    if(button.press())
    {
        Serial.println(pump.flowPump(100));
        //when you press the button the pump will continue working.
        Serial.println("press");
        //digitalWrite(5, HIGH);
    }
    else {
      //digitalWrite(5, LOW);
      digitalWrite(4, LOW);
    }
}
