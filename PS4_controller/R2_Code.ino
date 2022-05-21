#include <PS4Controller.h>
int prev_time = 0;

void setup() {
  // put your setup code here, to run once:
  // PS4.begin("30:94:35:32:df:3e");
  // PS4.begin("18:26:49:32:57:0C");
  PS4.begin("cc:66:0a:c9:41:d1");
  Serial.begin(115200);
  
  ledcSetup(0, 5000, 8); //loco1 
  ledcSetup(1, 5000, 8); //loco2
  ledcAttachPin(16, 0); //RX2
  ledcAttachPin(17, 1); //TX2
  pinMode(5, OUTPUT); //loco1_dir D5
  pinMode(18, OUTPUT); // loco2_dir D18

  ledcSetup(2, 5000, 8); // Lagori Grab
  ledcSetup(3, 5000, 8); // Lagori Lift

  ledcAttachPin(32, 2); // D32 -> Lagori Grab
  ledcAttachPin(25, 3); // D25 -> Lagori Lift
  pinMode(33, OUTPUT); // D33 -> Lagori Grab Dir
  pinMode(26, OUTPUT); // D26 -> Lagori Lift Dir

  ledcSetup(4, 5000, 8); // Ball Pass
  ledcSetup(5, 5000, 8); // Ball Pick

  ledcAttachPin(4, 4); // D4 -> Ball Pick
  ledcAttachPin(16, 5); // D16 -> Ball Pass
  pinMode(2, OUTPUT); // Ball Pick Dir
  pinMode(17, OUTPUT); // Ball Pass Dir

  pinMode(13, INPUT); // Top Limit Switch
  pinMode(12, INPUT); // Bottom Limit Switch
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis()-prev_time>70){
    if (PS4.isConnected()){
      if (PS4.Up()){
        ledcWrite(0, 160);
        ledcWrite(1, 160);
        digitalWrite(5, HIGH);
        digitalWrite(18, LOW);      
      }
      
      else if (PS4.Down()){
        ledcWrite(0, 160);
        ledcWrite(1, 160);
        digitalWrite(5, LOW);
        digitalWrite(18, HIGH);  
      }
      
      else if (PS4.Left()){
        ledcWrite(0, 160);
        ledcWrite(1, 160);
        digitalWrite(5, HIGH);
        digitalWrite(18, HIGH);  
      }
      
      else if (PS4.Right()){
        ledcWrite(0, 160);
        ledcWrite(1, 160);
        digitalWrite(5, LOW);
        digitalWrite(18, LOW);  
      }
  
      else{
        ledcWrite(0, 0);
        ledcWrite(1, 0);
      }
  
  //  Square -> Lagori In
  //  Circle -> Lagori Release
  //  Triangle -> Lagori Up
  //  Cross -> Lagori Down
  //  L1&L2 -> Ball Pick
  //  R1&R2 -> Ball Pass
    
      if (PS4.Square()){
          Serial.println("Square");
          ledcWrite(2, 160);
          digitalWrite(33, HIGH);
      }
      else if ((PS4.Triangle()) && (!digitalRead(13))){
        Serial.println("Triangle");
        ledcWrite(3, 160);
        digitalWrite(26, HIGH);
      }
      else if (PS4.Circle()){
        Serial.println("Circle");
        ledcWrite(2, 160);
        digitalWrite(33, HIGH);
      }
      else if ((PS4.Cross()) && (!digitalRead(12))){
        Serial.println("Cross");
        ledcWrite(3, 160);
        digitalWrite(26, LOW);
      }
      else if (PS4.L1()){
        Serial.println("L1");
        ledcWrite(4, 160);
        digitalWrite(2, LOW);
      }
      else if (PS4.L2()){
        Serial.println("L2");
        ledcWrite(4, 160);
        digitalWrite(2, HIGH);
      }
      else if (PS4.R1()){
        Serial.println("R1");
        ledcWrite(5, 160);
        digitalWrite(17, LOW);
      }
      else if (PS4.R2()){
        Serial.println("R2");
        ledcWrite(5, 160);
        digitalWrite(17, HIGH);
      }
      else{
        ledcWrite(2, 0);
        ledcWrite(3, 0);
        ledcWrite(4, 0);
        ledcWrite(5, 0);
      }
    }
    prev_time = millis();
  }
}
