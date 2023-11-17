void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
  //pinMode(5, OUTPUT);
  //pinMode(11, OUTPUT);

  //TCCR1A=_BV(COM1A1)|_BV(COM1B1);
  //TCCR1B=_BV(WGM13)|_BV(CS11);
  //analogWrite(3, 200);
  //analogWrite(5, 10);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int a = analogRead(A1);
  //float b = a*5.0/1024.0;
  Serial.println(a);
  
  delay(100);
}
