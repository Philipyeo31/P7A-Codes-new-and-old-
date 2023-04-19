int relay1 = 2;
int relay2 = 3;
int relay3 = 4;
int relay4 = 5;
  
void setup() {
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);


  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);

}

void loop() {
  
  delay(500);
  digitalWrite(relay1, LOW);
  delay(500);
  digitalWrite(relay2, LOW);
  delay(500);
  digitalWrite(relay3, LOW);
  delay(500);
  digitalWrite(relay4, LOW);
  delay(500);
  digitalWrite(relay1, HIGH);
  delay(200);
  digitalWrite(relay2, HIGH);
  delay(200);
  digitalWrite(relay3, HIGH);
  delay(200);
  digitalWrite(relay4, HIGH);
  
  
  
  

}
