#
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <ILI9488.h>

#define TFT_CS          9
#define TFT_RST        10
#define TFT_DC          8


//CLK pin 13
//mosi pin 11
//miso pin 12

//all these are LCD pin initialisations


//morning ritual when using this machine, plug in serial connector, open up serial monitor ctrl+shift+m, open all the valves manually e.g {v1:on,A1}.

ILI9488 tft = ILI9488(TFT_CS, TFT_DC, TFT_RST); //initialise and call LCD



int EN = 2;  //Definition RS485 shield enable terminal (the 2nd digital IO ports),
//high for the sending state, the low level of receiving state
int LED = 6;
int AirPump = 5;
int PMT = 4;
int sysStart = 3;

void setup()
{
  Serial.begin(115200); //baud rate(frequency)
  pinMode(EN, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(AirPump, OUTPUT);
  pinMode(PMT, OUTPUT);
  pinMode(sysStart, INPUT);
  tft.begin();
}




void loop()
{
  delay(10);
  lcdSetup();
  digitalWrite(AirPump, LOW);
  digitalWrite(PMT, LOW);
  digitalWrite(LED, LOW);

  bool start = HIGH;
  digitalWrite(EN, LOW);   //Enable low, RS485 shield waiting to receive data
  while (start) {
    if (digitalRead(sysStart) == HIGH) {
      start = LOW;
      break;
    }
  }
  digitalWrite(EN, HIGH);   //Enable high, RS485 shield waiting to transmit data
  Serial.println("{v1:shut}");
  delay(2500);
  Serial.println("{v2:shut}");
  //purgeSnDTubing();
  //Step1();
  //Step2();
  //Step3();
  //Step4();
  //Step5();
  //Step6();
  //Step7();
  //Step7new();
  //WashSubAndDevTubing();
  //systemReading();
  //systemReading2();
  //purgeSnDTubing();
  //step1345();

  start = HIGH;
  while (start) {
    if (digitalRead(sysStart) == HIGH) {
      start = LOW;
      break;
    }
  }
}

void lcdSetup() {
  tft.setRotation(3);
  tft.fillScreen(ILI9488_BLACK); //colour
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Please press button to\nbegin.");
}

void step1345()
{
  Step1();
  Step3();
  Step4();
  Step5();
}
void Step1()
{
  //Step 1: Water to Impinger
  tft.fillScreen(ILI9488_BLACK);
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 1: Water to impinger");

  Serial.println("{v1:shut}");
  delay(2500);
  Serial.println("{v2:shut}");
  delay(2500);
  Serial.println("{p1:setspeed,200}");
  delay(2500);
  Serial.println("{v1:on,A1}"); //water reservoir
  delay(2500);
  Serial.println("{v1:on,A2}");//impinger
  delay(2500);

  //total run 107, but need to split because of p1 issue
  Serial.println("{p1:run,25}");//add 20ml into impinger 1
  delay(15000);
  Serial.println("{p1:run,25}");//add 20ml into impinger 2
  delay(15000);
  Serial.println("{p1:run,25}");//add 20ml into impinger 3
  delay(15000);
  Serial.println("{p1:run,25}");//add 20ml into impinger 4
  delay(15000);
  /*Serial.println("{p1:run,12}");//add 20ml into impinger 5
    delay(10000);*/

  Serial.println("{v1:off,A1}");//turn off water reservoir
  delay(2500);
  Serial.println("{v1:on,B1}");//open air
  delay(2500);
  Serial.println("{p1:run,25}");//push water out
  delay(15000);
  Serial.println("{v1:off,B1}");//shut
  delay(2500);
  Serial.println("{v1:off,A2}");// ^^
  delay(2500);


  //clear text
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 1: Water to impinger");
}

void Step2()
{
  //Step 2: Air Sampling
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 2: Air sampling");

  //2 hour timer
  digitalWrite(AirPump, HIGH);
  delay(100);
  for (float timer = 0; timer < 7200; timer ++) {
    tft.setCursor(0, 200);
    tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
    tft.setTextSize(2);
    String printPercent = String ((timer * 100.0) / 7200.0);
    printPercent.concat("%");
    tft.println(printPercent);
    delay(1000);
  }
  digitalWrite(AirPump, LOW);

  //clear text
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 2: Air sampling");
}



void Step3()
{

  //Step 3: Sample Transfer
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 3: Sample transfer");

  Serial.println("{v1:shut}");
  delay(2500);
  Serial.println("{v1:on,A2}");//impinger open
  delay(2500);
  Serial.println("{p1:setspeed,-50}");
  delay(2500);


  Serial.println("{v1:on,A3}");//waste
  delay(2500);

  //run 25 total
  Serial.println("{p1:run,25}");// remove 10ml to waste part 1
  delay(25000);
  Serial.println("{p1:run,25}");// remove 10ml to waste part 2
  delay(25000);

  Serial.println("{v1:off,A3}");//off waste
  delay(3000);



  Serial.println("{v1:on,A4}");//open reactor
  delay(2500);
  Serial.println("{p1:run,5.5}");//push remaining 1ml into reactor (og value is 30, 4.5)now 7 bc when p1 got error, it rotates 7 times gives us 950ul
  delay(7000);  //og value is 40000
  Serial.println("{v1:off,A4}");//off reactor
  delay(2500);


  Serial.println("{v1:on,A3}");//open waste
  delay(2500);


  Serial.println("{p1:run,25}");//remove excess water 1
  delay(15000);
  Serial.println("{p1:run,25}");//remove excess water 2
  delay(15000);
  Serial.println("{p1:run,25}");//remove excess water 3
  delay(15000);
  Serial.println("{p1:run,25}");//remove excess water 4
  delay(15000);

  Serial.println("{v1:shut}");
  delay(2500);



  //clear text
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 3: Sample transfer");
}

void purgeSnDTubing()
{
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_PINK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Purging Sub and Dev Tubing");

  //clear air within reactor to waste2
  Serial.println("{v2:shut}");
  delay(2500);
  Serial.println("{v2:on,A3}"); //open reactor
  delay(2500);
  Serial.println("{v2:on,A4}"); //open waste2
  delay(2500);
  Serial.println("{p2:setspeed,200}");
  delay(2500);
  Serial.println("{p2:run, 50}");//move remaining water from reactor to waste2
  delay(20000);


  //push all sub back to reservoir
  DisplayPurgeStepA();
  Serial.println("{v2:shut}");
  delay(2500);
  Serial.println("{v2:on,A3}");//reactor
  delay(2500);

  Serial.println("{v2:shut}");
  delay(2500);
  Serial.println("{p2:setspeed,200}");
  delay(2500);
  Serial.println("{v2:on,B2}");//air
  delay(2500);
  Serial.println("{v2:on,A1}");//substrate
  delay(2500);
  Serial.println("{p2:run,20}");//move excess substrate back into substrate reservoir
  delay(14000);
  Serial.println("{v2:shut}");
  ClearPurgeStepA();


  //push all developer back into reservoir
  DisplayPurgeStepB();
  Serial.println("{v2:on,A2}");
  delay(2500);
  Serial.println("{v2:on,A3}");
  delay(2500);


  /*Serial.println("{p2:setspeed,-50}");
    delay(2500);
    Serial.println("{p2:run,4.5}");
    delay(7300);
    Serial.println("{v2:off,A2}");
    delay(2500);
    Serial.println("{v2:on,B1}");
    delay(2500);
    Serial.println("{p2:run,10}");
    delay(18000);*/


  Serial.println("{v2:shut}");
  delay(2500);
  Serial.println("{p2:setspeed,200}");
  delay(2500);
  Serial.println("{v2:on,B2}");
  delay(2500);
  Serial.println("{v2:on,A2}");
  delay(2500);
  Serial.println("{p2:run,10}");
  delay(18000);
  Serial.println("{v2:shut}");
  ClearPurgeStepB();


  DisplayPurgeStepC();

  //purge left side also
  //purge reactor to impinger
  Serial.println("{v1:on,A2}");
  delay(2500);
  Serial.println("{v1:on,A4}");
  delay(2500);
  Serial.println("{p1:setspeed,200}");
  delay(2500);
  Serial.println("{p1:run,20}");
  delay(20000);
  Serial.println("{v1:shut}");
  delay(2500);
  ClearPurgeStepC();

  DisplayPurgeStepD();

  //purge impinger to waste
  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{v1:on,A4}");
  delay(2500);
  Serial.println("{p1:setspeed,-200}");
  delay(2500);
  Serial.println("{p1:run,20}");
  delay(20000);

  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Purging Sub and Dev Tubing");

}

void Step4()
{
  //Step 4: Substrate Loading

  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 4: Substrate loading");


  Serial.println("{v2:shut}");
  delay(2500);
  Serial.println("{v2:on,A1}");//substrate valve
  delay(2500);

  Serial.println("{v2:on,A3}");//reactor
  delay(2500);


  Serial.println("{p2:setspeed,-50}");
  delay(2500);
  Serial.println("{p2:run,2.35}");//loads 300microL into the reactor (1.91 original value)
  delay(10000);
  Serial.println("{v2:off,A1}");//close substrate
  delay(2500);


  Serial.println("{v2:on,B1}");//air
  delay(2500);
  Serial.println("{p2:run,10}");//move all substrate into reactor
  delay(18000);


  Serial.println("{v2:shut}");
  delay(2500);
  Serial.println("{p2:setspeed,200}");
  delay(2500);
  Serial.println("{v2:on,B2}");//air
  delay(2500);
  Serial.println("{v2:on,A1}");//substrate
  delay(2500);
  Serial.println("{p2:run,10}");//move excess substrate back into substrate reservoir
  delay(18000);
  Serial.println("{v2:shut}");



  //clear text
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 4: Substrate loading");



  //step 5 incubation
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 5: Incubation");

  //30min incubation time
  for (float timer = 0; timer < 1800; timer ++) {
    tft.setCursor(0, 200);
    tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
    tft.setTextSize(2);
    String printPercent = String ((timer * 100.0) / 1800.0);
    printPercent.concat("%");
    tft.println(printPercent);
    delay(1000);
  }


  //clear text
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 5: Incubation");


  //step 6 developer loading
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 6: Developer loading");
  Serial.println("{v2:shut}");
  delay(2500);
  Serial.println("{v2:on,A2}");
  delay(2500);

  Serial.println("{v2:on,A3}");
  delay(2500);
  Serial.println("{p2:setspeed,-50}");
  delay(2500);
  Serial.println("{p2:run,4.5}");
  delay(7300);
  Serial.println("{v2:off,A2}");
  delay(2500);
  Serial.println("{v2:on,B1}");
  delay(2500);
  Serial.println("{p2:run,10}");
  delay(18000);


  Serial.println("{v2:shut}");
  delay(2500);
  Serial.println("{p2:setspeed,200}");
  delay(2500);
  Serial.println("{v2:on,B2}");
  delay(2500);
  Serial.println("{v2:on,A2}");
  delay(2500);
  Serial.println("{p2:run,10}");
  delay(18000);
  Serial.println("{v2:shut}");

  //clear text
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 6: Developer loadin");

}


void Step5()
{
  //Step 5: Signal Reading
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 7: Signal reading");


  float totalReading = 0.0;
  float averageReading = 0.0;

  digitalWrite(PMT, HIGH);
  digitalWrite(LED, HIGH);
  for (int x = 0; x < 25; x++)
  {
    // Read the value of the sensor on A0
    int sensorVal = (analogRead(A0));
    String percentage = String (sensorVal * (100.0 / 916.0));
    Serial.println(sensorVal * (5.0 / 1023.0));
    tft.setCursor(0, 200);
    tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
    tft.setTextSize(2);
    tft.println("instantaneous reading is: ");
    //percentage.concat("%");
    tft.println(sensorVal);
    totalReading += sensorVal;
    delay(150);
  }
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("instantaneous reading is: ");
  averageReading = totalReading / 25.0;
  String avg = String(averageReading);
  //avg.concat("%");
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_GREEN, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Reading Done!");
  tft.println("average reading is: ");
  tft.println(String(avg));
  delay(10);
  digitalWrite(PMT, LOW);
  digitalWrite(LED, LOW);

  //clear text
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 7: Signal reading");
}

void Step6()
{
  //clear please press button blah blah
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Please press button to\nbegin.");

  //Step 6: Removal of liquid substance in Reactor
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 8: Washing");


  Serial.println("{v2:on,A3}");
  delay(2500);
  Serial.println("{v2:on,A4}");
  delay(2500);
  Serial.println("{p2:setspeed,187.5}");
  delay(2500);
  Serial.println("{p2:run,30}");
  delay(13500);
  Serial.println("{v2:shut}");
  delay(2500);
}

void Step7()
{
  //Step 7: Wash 1
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Please press button to\nbegin.");

  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_PINK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 8: Washing");

  
  

  //water reservoir to impinger
  DispayWashStep1a();
  delay(3000);
  Serial.println("v1:shut");
  delay(2500);
  Serial.println("v2:shut");  //this is the only bit that varies from Step7();
  delay(2500);

  Serial.println("{p1:setspeed,187.5}");
  delay(2500);
  Serial.println("{v1:on,A1}");
  delay(2500);
  Serial.println("{v1:on,A2}");
  delay(2500);


  Serial.println("{p1:run,120}");
  delay(53000);
  Serial.println("{v1:off,A1}");
  delay(2500);

  //clear text
  ClearWashStep1a();

  //air to clear remaining water in tubes
  DispayWashStep1b();
  Serial.println("{v1:on,B1}");
  delay(2500);
  Serial.println("{p1:run,5}");
  delay(3000);
  Serial.println("{v1:off,B1}");
  delay(2500);
  Serial.println("{v1:off,A2}");
  delay(2500);

  ClearWashStep1b();


  //impinger to waste
  DisplayWashStep1c();
  Serial.println("{v1:on,A2}");
  delay(2500);

  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{p1:setspeed,-50}");
  delay(2500);
  Serial.println("{p1:run,95.5}");
  delay(128000);
  ClearWashStep1c();

  //impinger to reactor
  DisplayWashStep1d();
  Serial.println("{v1:off,A3}");
  delay(3000);
  Serial.println("{v1:on,A4}");
  delay(2500);
  Serial.println("{p1:run,30}");
  delay(40000);
  ClearWashStep1d();


  //reactor to waste
  DisplayWashStep1e();
  Serial.println("{v1:off,A4}");
  delay(2500);
  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{p1:run,5}");
  delay(7000);
  Serial.println("{v1:shut}");
  delay(2500);
  ClearWashStep1e();

  //reactor to waste
  DisplayWashStep1f();
  Serial.println("{v2:on,A3}");
  delay(2500);
  Serial.println("{v2:on,A4}");
  delay(2500);
  Serial.println("{p2:setspeed,187.5}");
  delay(2500);
  Serial.println("{p2:run,30}");
  delay(13500);
  Serial.println("{v2:shut}");
  delay(2500);
  ClearWashStep1f();

  //Step 7: Wash 2
  Serial.println("{p1:setspeed,187.5}");
  delay(2500);
  Serial.println("{v1:on,A1}");
  delay(2500);
  Serial.println("{v1:on,A2}");
  delay(2500);
  Serial.println("{p1:run,120}");
  delay(53000);
  Serial.println("{v1:off,A1}");
  delay(2500);
  Serial.println("{v1:on,B1}");
  delay(2500);
  Serial.println("{p1:run,5}");
  delay(3000);
  Serial.println("{v1:off,B1}");
  delay(2500);
  Serial.println("{v1:off,A2}");
  delay(2500);

  Serial.println("{v1:on,A2}");
  delay(2500);


  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{p1:setspeed,-50}");
  delay(2500);
  Serial.println("{p1:run,93.5}");
  delay(128000);
  Serial.println("{v1:off,A3}");
  delay(3000);
  Serial.println("{v1:on,A4}");
  delay(2500);
  Serial.println("{p1:run,30}");
  delay(54000);
  Serial.println("{v1:off,A4}");
  delay(2500);
  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{p1:run,5}");
  delay(7000);
  Serial.println("{v1:shut}");
  delay(2500);

  Serial.println("{v2:on,A3}");
  delay(2500);
  Serial.println("{v2:on,A4}");
  delay(2500);
  Serial.println("{p2:setspeed,187.5}");
  delay(2500);
  Serial.println("{p2:run,30}");
  delay(13500);
  Serial.println("{v2:shut}");
  delay(2500);

  //clear text




}






void Step7new()
{
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Please press button to\nbegin.");

  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_PINK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 8: Washing");


  //Step 7: Wash 1
  //display opened valves and what's being cleaned

  

  DispayWashStep1a(); //water reservoir to impinger
  delay(3000);
  Serial.println("v1:shut");
  delay(2500);
  Serial.println("v2:shut");  //this is the only bit that varies from Step7();
  delay(2500);

  //new new code
  //remove previous sample from reactor to waste2
  Serial.println("{p1:setspeed,187.5}");
  delay(2500);
  Serial.println("{v2:on,A3}");
  delay(2500);
  Serial.println("{v2:on,A4}");
  delay(2500);
  Serial.println("{p2:run,25}");
  delay(15000);
  Serial.println("{p2:run,25}");
  delay(15000);
  Serial.println("{p2:run,25}");
  delay(15000);
  
  

  Serial.println("{p1:setspeed,187.5}");
  delay(2500);
  Serial.println("{v1:on,A1}");
  delay(2500);
  Serial.println("{v1:on,A2}");
  delay(2500);


  Serial.println("{p1:run,25}");//res to imp 1
  delay(15000);
  Serial.println("{p1:run,25}");//res t imp 2
  delay(15000);
  Serial.println("{p1:run,25}");//ares to imp 3
  delay(15000);
  Serial.println("{p1:run,25}");//ret to imp 4
  delay(15000);
  Serial.println("{v1:off,A1}");
  delay(2500);

  //clear text
  ClearWashStep1a();

  //air to clear remaining water in tubes
  DispayWashStep1b();
  Serial.println("{v1:on,B1}");
  delay(2500);
  Serial.println("{p1:run,5}");
  delay(3000);
  Serial.println("{v1:off,B1}");
  delay(2500);
  Serial.println("{v1:off,A2}");
  delay(2500);

  ClearWashStep1b();


  //impinger to waste
  DisplayWashStep1c();
  Serial.println("{v1:on,A2}");
  delay(2500);

  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{p1:setspeed,-150}"); //og -50
  delay(2500);

  Serial.println("{p1:run,25}");//imp to waste 1
  delay(15000);
  Serial.println("{p1:run,25}");//imp to waste 2
  delay(15000);

  /*Serial.println("{p1:run,60}"); //og 95.5
    delay(50000); //og 128000*/
  ClearWashStep1c();

  //impinger to reactor
  DisplayWashStep1d();
  Serial.println("{v1:off,A3}");
  delay(3000);
  Serial.println("{v1:on,A4}");
  delay(2500);
  Serial.println("{p1:run,30}"); //og 30
  delay(54000);
  ClearWashStep1d();


  //reactor to waste
  DisplayWashStep1e();
  Serial.println("{v1:off,A4}");
  delay(2500);
  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{p1:run,15}");
  delay(7000);
  Serial.println("{v1:shut}");
  delay(2500);
  ClearWashStep1e();

  //reactor to waste2
  DisplayWashStep1f();
  Serial.println("{v2:on,A3}");
  delay(2500);
  Serial.println("{v2:on,A4}");
  delay(2500);
  Serial.println("{p2:setspeed,187.5}");
  delay(2500);
  Serial.println("{p2:run,25}"); //og 30
  delay(15000);
  Serial.println("{p2:run,25}"); //og 30
  delay(15000);
  Serial.println("{v2:shut}");
  delay(2500);
  ClearWashStep1f();



  //Step 7: Wash 2
  //display opened valves and what's being cleaned
  DispayWashStep1a(); //water reservoir to impinger
  delay(3000);
  Serial.println("v1:shut");
  delay(2500);
  Serial.println("v2:shut");  //this is the only bit that varies from Step7();
  delay(2500);

  Serial.println("{p1:setspeed,187.5}");
  delay(2500);
  Serial.println("{v1:on,A1}");
  delay(2500);
  Serial.println("{v1:on,A2}");
  delay(2500);


  Serial.println("{p1:run,25}");//res to imp 1
  delay(15000);
  Serial.println("{p1:run,25}");//res t imp 2
  delay(15000);
  Serial.println("{p1:run,25}");//ares to imp 3
  delay(15000);
  Serial.println("{p1:run,25}");//ret to imp 4
  delay(15000);
  Serial.println("{v1:off,A1}");
  delay(2500);

  //clear text
  ClearWashStep1a();

  //air to clear remaining water in tubes
  DispayWashStep1b();
  Serial.println("{v1:on,B1}");
  delay(2500);
  Serial.println("{p1:run,5}");
  delay(3000);
  Serial.println("{v1:off,B1}");
  delay(2500);
  Serial.println("{v1:off,A2}");
  delay(2500);

  ClearWashStep1b();


  //impinger to waste
  DisplayWashStep1c();
  Serial.println("{v1:on,A2}");
  delay(2500);

  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{p1:setspeed,-150}"); //og -50
  delay(2500);

  Serial.println("{p1:run,25}");//imp to waste 1
  delay(15000);
  Serial.println("{p1:run,25}");//imp to waste 2
  delay(15000);

  /*Serial.println("{p1:run,60}"); //og 95.5
    delay(50000); //og 128000*/
  ClearWashStep1c();

  //impinger to reactor
  DisplayWashStep1d();
  Serial.println("{v1:off,A3}");
  delay(3000);
  Serial.println("{v1:on,A4}");
  delay(2500);
  Serial.println("{p1:run,30}"); //og 30
  delay(40000);
  ClearWashStep1d();


  //reactor to waste
  DisplayWashStep1e();
  Serial.println("{v1:off,A4}");
  delay(2500);
  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{p1:run,15}");
  delay(7000);
  Serial.println("{v1:shut}");
  delay(2500);
  ClearWashStep1e();

  //reactor to waste2
  DisplayWashStep1f();
  Serial.println("{v2:on,A3}");
  delay(2500);
  Serial.println("{v2:on,A4}");
  delay(2500);
  Serial.println("{p2:setspeed,187.5}");
  delay(2500);
  Serial.println("{p2:run,25}"); //og 30
  delay(15000);
  Serial.println("{p2:run,25}"); //og 30
  delay(15000);
  Serial.println("{v2:shut}");
  delay(2500);
  ClearWashStep1f();


  //new new code
  //completely remove to waste
  
  Serial.println("{v1:on,A2}");
  delay(2500);

  Serial.println("{v1:on,A3}");
  delay(2500);
  Serial.println("{p1:setspeed,-150}"); //og -50
  delay(2500);

  Serial.println("{p1:run,25}");//imp to waste 1
  delay(15000);
  Serial.println("{p1:run,25}");//imp to waste 2
  delay(15000);
  Serial.println("{p1:run,25}");//imp to waste 2
  delay(15000);
  Serial.println("{p1:run,25}");//imp to waste 2
  delay(15000);
  Serial.println("{v1:shut}");
  delay(2500);
  Serial.println("{v2:shut}");
  delay(2500);

  
  //clear text
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 8: Washing");


  purgeSnDTubing();


  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Washing complete .\nPress button to continue.");
}


void WashSubAndDevTubing()
{
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Please press button to\nbegin.");

  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_GREEN, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Washing Sub and Dev Tubes.");


  Serial.println("v1:shut");
  delay(2500);
  Serial.println("v2:shut");
  delay(2500);

  Serial.println("{v2:on,A1}"); //open sub valve
  delay(2500);
  Serial.println("{v2:on,A3}"); //open reactor valve
  delay(2500);
  Serial.println("{p2:setspeed, -187.5}");
  delay(2500);
  Serial.println("{p2:run,30}");//brings sub to reactor
  delay(13500);
  Serial.println("{v2:off,A1}"); //close sub valve
  delay(2500);
  Serial.println("{v2:on,A4}"); //pen waste2 valve
  delay(2500);
  Serial.println("{p2:setspeed, 187.5}");
  delay(2500);
  Serial.println("{p2:run,30}");//brings sub from reactor to waste
  delay(13500);
  Serial.println("{v2:off,A4}"); //close waste2 valve
  delay(2500);

  //wash dev
  Serial.println("{v2:on,A2}"); //open dev valve
  delay(2500);
  Serial.println("{p2:setspeed, -187.5}");
  delay(2500);
  Serial.println("{p2:run,30}");//bring dev to reactor
  delay(13500);
  Serial.println("{v2:off,A2}"); //close dev valve
  delay(2500);
  Serial.println("{v2:on,A4}"); //open waste2 valve
  delay(2500);
  Serial.println("{p2:setspeed, 187.5}");
  delay(2500);
  Serial.println("{p2:run,50}");//brings sub from reactor to waste
  delay(17000);
  //Serial.println("{v2:off,A4}"); //close waste2 valve
  //delay(2500);

  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Washing Sub and Dev Tubes.");

}

void systemReading()
{
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 7: Signal reading");


  float totalReading = 0.0;
  float averageReading = 0.0;

  digitalWrite(PMT, HIGH);
  digitalWrite(LED, HIGH);
  for (int x = 0; x < 1000; x++)
  {
    // Read the value of the sensor on A0
    int sensorVal = (analogRead(A0));
    //String percentage = String (sensorVal * (100.0 / 916.0));
    Serial.println(sensorVal * (5.0 / 1023.0));
    tft.setCursor(0, 200);
    tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
    tft.setTextSize(2);
    tft.println("instantaneous reading is: ");
    //percentage.concat("%");
    //tft.println(percentage);
    tft.println(sensorVal);
    //totalReading += sensorVal * (100.0 / 916.0);
    delay(300);

    tft.setCursor(0, 200);
    tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
    tft.setTextSize(2);
    tft.println("instantaneous reading is: ");
    //percentage.concat("%");
    //tft.println(percentage);
    tft.println(sensorVal);



    //display how many count has passed
    tft.setCursor(240, 100);
    tft.setTextColor(ILI9488_PURPLE, ILI9488_BLACK);
    tft.setTextSize(1);
    tft.println(x);
  }
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("instantaneous reading is: ");
  averageReading = totalReading / 25.0;
  String avg = String(averageReading);
  avg.concat("%");
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_GREEN, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Reading Done!");
  tft.println("average reading is: ");
  //tft.println(String(avg));
  delay(10);
  digitalWrite(PMT, LOW);
  digitalWrite(LED, LOW);

  //clear text
  tft.setCursor(0, 130);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(3);
  tft.println("Step 7: Signal reading");
}

// All washing step displays

void DispayWashStep1a()
{
  //v1a1 v1a2 water reservoir to impinger
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1A1, V1A2");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: nil ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Water to impinger");
}

void ClearWashStep1a()
{
  //v1a1 v1a2 water reservoir to impinger
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1A1, V1A2");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: nil ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Water to impinger");
}

void DispayWashStep1b()
{
  //v1a1 v1a2 water reservoir to impinger
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1B1, V1A2");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: V1A1 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Air to clear liquid in tubes");
}

void ClearWashStep1b()
{
  //v1a1 v1a2 water reservoir to impinger
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1B1, V1A2");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: V1A1 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Air to clear liquid in tubes");
}

void DisplayWashStep1c()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1A2, V1A3");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: V1B1 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Impinger to Waste");
}

void ClearWashStep1c()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1A2, V1A3");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: V1B1 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Impinger to Waste");
}

void DisplayWashStep1d()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1A4");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: V1A3 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("impinger to reactor");
}

void ClearWashStep1d()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1A4");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: V1A3 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("impinger to reactor");
}

void DisplayWashStep1e()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1A3");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: V1A4 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Reactor to Waste");
}

void ClearWashStep1e()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V1A3");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: V1A4 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Reactor to Waste");
}

void DisplayWashStep1f()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V2A3, V2A4");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: all of V1 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Reactor to waste P2");
}

void ClearWashStep1f()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve opened: V2A3, V2A4");
  tft.setCursor(0, 235);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Valve closed: all of V1 ");
  tft.setCursor(0, 270);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Reactor to Waste P2");
}

void DisplayPurgeStepA()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Purging Substrate");
}
void ClearPurgeStepA()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Purging Substrate");
}
void DisplayPurgeStepB()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Purging Developer");
}
void ClearPurgeStepB()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Purging Developer");
}

void DisplayPurgeStepC()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Purging Reactor to Impinger");
}
void ClearPurgeStepC()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Purging Reactor to Impinger");
}

void DisplayPurgeStepD()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_YELLOW, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Purging Impinger to Waste");
}
void ClearPurgeStepD()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9488_BLACK, ILI9488_BLACK);
  tft.setTextSize(2);
  tft.println("Purging Impinger to Waste");
}
