void setup() {
  // put your setup code here, to run once:
    pinMode(3,OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(2)){
  analogWrite(3, 229);
  analogWrite(5, 229);
  }else if(!digitalRead(2)){
    analogWrite(3, 0);
  analogWrite(5, 0);
  }
}
