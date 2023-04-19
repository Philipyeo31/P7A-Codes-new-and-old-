#include <Arduino.h>
#include <stdio.h>

int EN = 2;


//byte example[] = {EF, addr, len, W, J, speed1stbyte, speed2ndbyte, full/normal speed and start/stop, dir, xor};
byte example[] = {233, 1, 6, 87, 74, 1, 244, 0, 1, 238};
bool x = 1;
byte sendToPump[] = {};
byte pumpStop[] = {233, 1, 6, 87, 74, 1, 244, 0, 1, 238};





void pump(int address, int speed /*RPM in units of 0.1. e.g 60.0RPM is 600*/, float revolutions, bool direction){
  byte length = 0x6; //lenth of pdu for sending instructions always 6 bytes
  
  //convert speed from INT to HEX
  int speed_ones_16e0 = 0;
  int speed_tens_16e1 = 0;
  int speed_tens_onwards_16e1 = 0;
  int speed_hundreds_16e2 = 0;


  speed_ones_16e0 = speed % 16;
  speed_tens_onwards_16e1 = speed / 16;
  speed_hundreds_16e2 = speed_tens_onwards_16e1 / 16;
  speed_tens_16e1 = speed_tens_onwards_16e1 % 16;


  //converting the base 16 INT to 0-255 base 10 INT
  int speed_first_byte = 0;
  int speed_second_byte = 0;

  speed_first_byte = speed_hundreds_16e2;
  speed_second_byte = (speed_tens_16e1 * 16) + (speed_ones_16e0);
  
  /*
  Serial.println(speed_ones_16e0);
  Serial.println(speed_tens_16e1);
  Serial.println(speed_tens_onwards_16e1);
  Serial.println(speed_hundreds_16e2);
  Serial.println(" ");
  Serial.println(speed_first_byte);
  Serial.println(speed_second_byte);

  Serial.println("\n");
  */

  //XOR calculation
  int xorCheck = 0;
  xorCheck = address ^ length ^ 87 ^ 74 ^ speed_first_byte ^ speed_second_byte ^ 1 ^ direction;
  

  // Serial.println(xorCheck);
  
  sendToPump[0] = 233; //start flag
  sendToPump[1] = address; //Address for pump
  sendToPump[2] = 6; // instructions to the pump (PDU) to run is always 6 byte long
  sendToPump[3] = 87; //W ascii HEX code but in base 10 INT
  sendToPump[4] = 74; //J ascii HEX code but in base 10 INT
  sendToPump[5] = speed_first_byte; //speed first byte
  sendToPump[6] = speed_second_byte; //speed second byte
  sendToPump[7] = 1; //start at normal speed
  sendToPump[8] = direction; //1 = Clockwise , 0 = Counter-Clockwise
  sendToPump[9] = xorCheck; //XOR of address, length and pdu


  //send instructions to pump
  for (int i=0; i<=9; i++){
    Serial.write(sendToPump[i]);
  }

  Serial.println("");

  //run duration calculation
  unsigned long runDurationms = 0;
  
  runDurationms = revolutions / speed * 600000; //6,000 instead of 60,000 is because rpm is in resolution of .1



  delay(runDurationms);

  //send code to pump
  for (int i=0; i<=9; i++){
    Serial.write(pumpStop[i]);
  }


}



void pumpread(int address){

}

void setup() {
  delay(500);
  Serial.begin(9600, SERIAL_8E1);
  pinMode(EN,OUTPUT);
  pump(1,100,2.0,0);



}

void loop() {
  if (x==1){
    // digitalWrite(EN,HIGH);
    // for (byte i = 0; i < sizeof(example); i++){
    //   Serial.write(example[i]);
    // }
    
    x=0;

  }

  


  
  
  if (Serial.available() > 0) {
    byte data = Serial.read();
    Serial.println(data, HEX);
  }


}


