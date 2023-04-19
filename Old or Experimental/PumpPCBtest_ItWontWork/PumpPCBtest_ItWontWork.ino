#include <Arduino.h>


int EN = 2;


//byte example[] = {EF, addr, len, W, J, speed1stbyte, speed2ndbyte, full/normal speed and start/stop, dir, xor};
byte example[] = {233, 1, 6, 87, 74, 1, 244, 1, 1, 239};
// example[] ={0xE9, 0x01, 0x06, 0x57, 0x4A, 0x01, 0xF4, 0x01, 0x01, 0xEF};
byte sendToPump[] = {};
byte exampleStop[] = {0xE9, 0x01, 0x06, 0x57, 0x4A, 0x01, 0xF4, 0x00, 0x01, 0xEE};


// byte sendToPump[9] = {};
byte pumpStop[] = {};
// byte receivedByte[5] = {}; //Initialise rturn byte from PCB to check command has went through





void pump(int address, int speed /*RPM in units of 0.1. e.g 60.0RPM is 600*/, float revolutions, bool direction){
  byte length = 0x6; //lenth of pdu for sending instructions always 6 bytes
  
  //run duration calculation
  unsigned long runDurationms = 0;
  runDurationms = revolutions / speed * 600000; //6,000 instead of 60,000 is because rpm is in resolution of .1
  
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
  
  //debug display
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

  int xorCheckStop = 0;
  xorCheckStop = address ^ length ^ 87 ^ 74 ^ speed_first_byte ^ speed_second_byte ^ 0 ^ direction;

  int xorReceivedByte = 0;
  xorReceivedByte = address ^ 2 ^ 87 ^ 74;
  //Serial.println(xorCheck);

  //start instructions
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

  

  //stop instructions
  pumpStop[0] = 233; //start flag
  pumpStop[1] = address; //Address for pump
  pumpStop[2] = 6; // instructions to the pump (PDU) to run is always 6 byte long
  pumpStop[3] = 87; //W ascii HEX code but in base 10 INT
  pumpStop[4] = 74; //J ascii HEX code but in base 10 INT
  pumpStop[5] = speed_first_byte; //speed first byte
  pumpStop[6] = speed_second_byte; //speed second byte
  pumpStop[7] = 0; //STOP
  pumpStop[8] = direction; //1 = Clockwise , 0 = Counter-Clockwise
  pumpStop[9] = xorCheckStop; //XOR of address, length and pdu


  //send instructions to pump
  for (int i=0; i<=9; i++)
    Serial.write(sendToPump[i]);
  

  // Serial.println("");
  // Serial.println(sendToPump[0]);
  // Serial.println(sendToPump[1]);
  // Serial.println(sendToPump[2]);
  // Serial.println(sendToPump[3]);
  // Serial.println(sendToPump[4]);
  // Serial.println(sendToPump[5]);
  // Serial.println(sendToPump[6]);
  // Serial.println(sendToPump[7]);
  // Serial.println(sendToPump[8]);
  // Serial.println(sendToPump[9]);
  

  /*
  if (Serial.available() > 0) {
    byte receivedByte = Serial.read();    
  }

  //Test if signal received
  if (receivedByte[0] == 233     &&
      receivedByte[1] == address &&
      receivedByte[2] == 2       &&
      receivedByte[3] == 87      &&
      receivedByte[4] == 74      &&
      receivedByte[5] == xorReceivedByte){
        Serial.println("Return Signal from PCB received");        
  }
  else{
        Serial.println("NO return signal from PCB");
  }
*/
//   delay(runDurationms);

//   //send code to pump
//   for (int i=0; i<=9; i++){
//     Serial.write(pumpStop[i]);
//   }
}



void pumpread(int address){

}

void setup() {
  Serial.begin(9600, SERIAL_8E1);
  delay(500); 
  pinMode(EN,OUTPUT);
  pump(1,400,2.0,1);
  delay(500);
}

bool x = 0;



void loop() {
  if (x==1){
    digitalWrite(EN,HIGH);
    for (byte i = 0; i < sizeof(example); i++){
      Serial.write(example[i]);
    }
    delay(5000);

    digitalWrite(EN,HIGH);
    for (byte i = 0; i < sizeof(exampleStop); i++){
      Serial.write(exampleStop[i]);
    }
    x=0;
  }
  // if (Serial.available() > 0) {
  //   byte data = Serial.read();
  //   Serial.println(data);
  // }
}


