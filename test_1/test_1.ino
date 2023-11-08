void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(11, OUTPUT);

  //TCCR1A=_BV(COM1A1)|_BV(COM1B1);
  //TCCR1B=_BV(WGM13)|_BV(CS11);
  analogWrite(3, 200);
  analogWrite(5, 10);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  
  delay(100);
}
