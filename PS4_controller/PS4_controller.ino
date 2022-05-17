//Do not connect anything to D11 as it keeps reseting connected to internal SPI flash

#include <PS4Controller.h>
int prev_time = 0;
int curr_time = 0;
int flag =1;
int ig52Debounce = 0;
int ig = 0;
int shaft = 0;
int onn = 0;
int shaftDebounce = 0;
void setup() {
 
  Serial.begin(115200);
 //54:8d:5a:88:d9:f1
 //18:26:49:32:57:0C
  PS4.begin("18:26:49:32:57:0C");
  Serial.println("Ready.");
  ledcSetup(0, 5000, 8); //loco1 
  ledcSetup(1, 5000, 8); //loco2
   ledcAttachPin(16, 0); //RX2
   
   ledcAttachPin(17, 1); //TX2
   pinMode(5, OUTPUT); //loco1_dir D5
   pinMode(18, OUTPUT); //loco2_dir D18

    ledcSetup(2, 5000, 8); //horizontal_motor_pwm 
   ledcAttachPin(19, 2); //D19 
   pinMode(22, OUTPUT); //horizontal_motor_dir D22

   ledcSetup(3, 5000, 8); //vertical_motor_pwm
   ledcAttachPin(21, 3); //D21
   pinMode(23, OUTPUT); //vertical_motor_dir D23

   ledcSetup(4, 5000, 8); //shaft_pwm
   ledcAttachPin(32, 4); //D32
   pinMode(25, OUTPUT); //shaft_dir D9
   digitalWrite(25,LOW);

    ledcSetup(5, 5000, 8); //ig32_motor_pwm
   ledcAttachPin(27, 5); //D10
   pinMode(26, OUTPUT); //ig32_motor_dir D13
   digitalWrite(26,LOW);

   pinMode(35, INPUT); //Limit_switch D35
   pinMode(34,INPUT); //horizontal Limit_switch D34

   pinMode(13,INPUT); //limit_switch_horizontal D13
}
int data[3] = {};
void loop() {
  if(millis()-prev_time>70){
  // Below has all accessible outputs from the controller
  
  if(PS4.isConnected()) {
    Serial.println("in");
//    if(PS4.Up() && PS4.Triangle()){
//      ledcWrite(0, 110);
//      ledcWrite(1, 110);
//      digitalWrite(5, LOW);
//      digitalWrite(18, HIGH);
//      ledcWrite(3, 230);
//      digitalWrite(23, HIGH);
//    }
//    else if(PS4.Up() && PS4.Cross() && !digitalRead(35)){
//       ledcWrite(0, 110);
//      ledcWrite(1, 110);
//      digitalWrite(5, HIGH);
//      digitalWrite(18, LOW);
//      ledcWrite(3, 230);
//      digitalWrite(23, LOW);
//    }
//    else if(PS4.Down() && PS4.Triangle()){
//       ledcWrite(0, 110);
//      ledcWrite(1, 110);
//      digitalWrite(5, HIGH);
//      digitalWrite(18, LOW);
//      ledcWrite(3, 230);
//      digitalWrite(23, HIGH);
//    }
//    else if(PS4.Down() && PS4.Cross() && !digitalRead(35)){
//       ledcWrite(0, 110);
//      ledcWrite(1, 110);
//      digitalWrite(5, HIGH);
//      digitalWrite(18, LOW);
//      ledcWrite(3, 230);
//      digitalWrite(23, LOW);
//    }
//    else if(PS4.Triangle() && PS4.Circle()  && !digitalRead(13)){
//       data[0] = 20;
//      data[1]= 0;
//      data[2] = 0;
//      Serial.write(data[0]);
//      Serial.write(data[1]);
//      Serial.write(data[2]);
//       ledcWrite(3, 230);
//      digitalWrite(23, HIGH);
//      ledcWrite(2, 100);
//      digitalWrite(22, LOW);
//      
//    }
//    else if(PS4.Triangle() && PS4.Square() && !digitalRead(34)){
//       data[0] = 21;
//      data[1]= 0;
//      data[2] = 0;
//      Serial.write(data[0]);
//      Serial.write(data[1]);
//      Serial.write(data[2]);
//       ledcWrite(3, 230);
//      digitalWrite(23, HIGH);
//      ledcWrite(2, 100);
//      digitalWrite(22, HIGH);
//      
//    }else if(PS4.Cross() && !digitalRead(35) && PS4.Square() && !digitalRead(34)){
//       data[0] = 22;
//      data[1]= 0;
//      data[2] = 0;
//      Serial.write(data[0]);
//      Serial.write(data[1]);
//      Serial.write(data[2]);
//      ledcWrite(3, 230);
//      digitalWrite(23, LOW);
//      ledcWrite(2, 100);
//      digitalWrite(22, HIGH);
//      
//  
//    }else if(PS4.Cross()  && !digitalRead(35) && PS4.Circle()  && !digitalRead(13)){
//       data[0] = 23;
//      data[1]= 0;
//      data[2] = 0;
//      Serial.write(data[0]);
//      Serial.write(data[1]);
//      Serial.write(data[2]);
//      ledcWrite(3, 230);
//      digitalWrite(23, LOW);
//      ledcWrite(2, 100);
//      digitalWrite(22, LOW);
//      
//    }
    if (PS4.Up()){
      Serial.println("debug");
      data[0] = 1;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(0, 160);
      ledcWrite(1, 160);
      digitalWrite(5, LOW);
      digitalWrite(18, HIGH);
    }

    else if (PS4.Down()){
      data[0] = 2;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(0, 160);
      ledcWrite(1, 160);
      digitalWrite(5, HIGH);
      digitalWrite(18, LOW);
    }

    else if (PS4.Left()){
      data[0] = 3;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(0, 160);
      ledcWrite(1, 160);
      digitalWrite(5, HIGH);
      digitalWrite(18, HIGH);
    }

    else if (PS4.Right()){
      data[0] = 4;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(0, 160);
      ledcWrite(1, 160);
      digitalWrite(5, LOW);
      digitalWrite(18, LOW);
    }

    else if (PS4.Square() && !digitalRead(34)){
      data[0] = 5;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
       ledcWrite(2, 190);
      digitalWrite(22, HIGH);
    }

    else if (PS4.Circle()  && !digitalRead(13)){
      data[0] = 6;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(2, 190);
      digitalWrite(22, LOW);
    }
    else if (PS4.L1()){
    data[0] = 7;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      if(millis()-shaftDebounce>500 && onn == 0){
        onn = 1;  
        ledcWrite(4, 220);
        shaftDebounce=millis();
      }else if(millis()-shaftDebounce>500 && onn == 1){
        onn = 0;
        ledcWrite(4, 0);
        shaftDebounce=millis();
      }
  }
  else if (PS4.L2()){
    data[0] = 8;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
  }
  else if (PS4.R1()){
    data[0] = 9;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(5, 20);
      digitalWrite(26,LOW);
      
  }else if (PS4.R2()){
    data[0] = 10;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      digitalWrite(26,HIGH);
      ledcWrite(5, 20);
  }else if(PS4.Triangle()){
    data[0] = 11;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(3, 230);
      digitalWrite(23, HIGH);
  }else if(PS4.Cross()  && !digitalRead(35)){
    data[0] = 12;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(3, 230);
      digitalWrite(23, LOW);
  }else{
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
    ledcWrite(5, 0);
  }
      data[0] = 0;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      
    
  }
    
  prev_time = millis();

  }
}
