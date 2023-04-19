#include <SPI.h>
#include <SD.h>
#include "GravityPump.h"
#include "Button.h"//https://github.com/DFRobot/Button
#include <EEPROM.h>


GravityPump pump;
Button button;
bool run = true;
int debug = 1;

File pdataf;

bool p7A_readingsOK;
bool fileAlrExists;

String DateAndTime = ("5th Dec 2022, 14:31");
String prevData = ("822");

//int relay = 9;





void setup()
{


    //pinMode(relay, OUTPUT);
    pump.setPin(13);
    button.init(7);
    delay(2000);
    Serial.begin(9600);
    pump.getFlowRateAndSpeed();




    //SD card testing

    //Initialising the SD card
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  
    Serial.print("Initializing SD card...");
  
    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
      while (1);
    }
    Serial.println("initialization done. \n");
   

////////////////////////////////////////////////////////////////////////////////////////

    //create directory
    p7A_readingsOK = SD.mkdir("p7A"); //creates folder within SD card for reading
    if (p7A_readingsOK){
      Serial.println("Dir created");
    }
    else{
      Serial.println("Dir not ok");
    }

////////////////////////////////////////////////////////////////////////////////////////////

    //Creates file and writes to it
    pdataf = SD.open("p7a/pdata.txt",FILE_WRITE);
    
    if (pdataf){
      Serial.println("writing");
      pdataf.println('[' + DateAndTime + ']' + ' ' +  prevData); //writes into prevReadings.txt DateAndTime and prevData
      pdataf.close();
      Serial.println("done writing");
    }
    else{
      Serial.println("File not created / opened");
    }
    
////////////////////////////////////////////////////////////////////////////////////////////
    
    //Read file called pdata.txt
    pdataf = SD.open("p7a/pdata.txt"); 
    if (pdataf) {
      Serial.println("p7a/pdata.txt: \n"); //header
      while (pdataf.available()){
        Serial.write(pdataf.read()); //write onto serial monitor the prevReadings
      }
      pdataf.close();
    }
    else{
      Serial.println("data cannot be read");
    }

/////////////////////////////////////////////////////////////////////////////////////////////


}


void loop()
{
    pump.update();
    //button.update();
    if (debug == 1)
    //in debug mode the pump will do calibation.
    //if set the debug mode off then the function works.
    {
       
        delay(2500);
        Serial.println("{p2:setspeed,200}");
        delay(2500);
        Serial.println("{p2:run,100}");
        delay(5000);
        Serial.println("{p1:setspeed,100}");
        delay(2500);
        Serial.println("{p1:run,100}");

/*
        delay (2500);
        digitalWrite(relay, HIGH);
        delay (2500);
        digitalWrite(relay, HIGH);
*/
        


        
        
    }
/*
    if (debug == 2)
    //random testing
    {
        float pumpDispense (float pVolume, int pSpeed)
        {
           pSpeed = 0 to 180. pVolume is in mL
           * flowrate address is 32, value is 255
           * trying to get pSpeed to be uL/s, so must find ul/s for the speed
           *//*
           _pumpservo.write(
          

        
        }  
    }
*/

    
    if (debug == 3)
    {
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

    if (debug == 4)
    {
          
                    
          
          
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
            
            //Serial.println(pump.timerPump(runTimeA));
            /*intervaltime = runTime*1000
             * 
             * for the flowpump thing, interval time = 1000*(volume/flowrate)
             * 
             * runTime = volume/flowrate
             * 
             * 
             */

            
            Serial.println(pump.flowPump(1));
            //just put the number in ml then the pump will dosing the numbers of liquid to you.
            //and you can find the numbers from serial port.
            
            


        }
        

    }
    /*
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
    */
    
}
