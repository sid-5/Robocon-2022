#include <PS4Controller.h>

//34 - 39 -----no PWM
//passing 32
//20 picking
int prev_time = 0;
int curr_time = 0;
int flag =1;
int passing_debounce = 0;
int stepperDebounce = 0;
int angle = 0;
int picking = 0;
int bldc_debounce = 0;
int bldc = 0;
int passing = 0;

void setup() {
 
  Serial.begin(115200);
 //54:8d:5a:88:d9:f1
 //30:94:35:32:df:3e
  PS4.begin("30:94:35:32:df:3e");
  Serial.println("Ready.");
  ledcSetup(0, 5000, 8); //loco1 
  ledcSetup(1, 5000, 8); //loco2
   ledcAttachPin(16, 0); //RX2
   
   ledcAttachPin(17, 1); //TX2
   pinMode(5, OUTPUT); //loco1_dir D5
   pinMode(18, OUTPUT); //loco2_dir D18

    ledcSetup(2, 5000, 8); //Passing_motor_pwm 
   ledcAttachPin(19, 2); //D19 
   pinMode(22, OUTPUT); //Passing_motor_dir D22

   ledcSetup(3, 5000, 8); //Picking_motor_pwm
   ledcAttachPin(21, 3); //D21
   pinMode(23, OUTPUT); //Picking_motor_dir D23

   //servo
    ledcSetup(4, 50, 16); // channel 1, 50 Hz, 16-bit width
   ledcAttachPin(11, 1);   // GPIO 22 assigned to channel 1

   //servo to 0 angle
   ledcWrite(4, 5);

   //linear Actautor
   ledcSetup(5, 5000, 8);
   ledcAttachPin(10, 5); //linear_actuator D10
   pinMode(9, OUTPUT); //Linear_Actuator_dir D9

   //BLDC
      pinMode(12, OUTPUT);
      pinMode(13, OUTPUT);
   
}
int data[3] = {};
void loop() {
  if(millis()-prev_time>100){
  // Below has all accessible outputs from the controller
  
  if(PS4.isConnected()) {
    Serial.println("in");
    if (PS4.Up()){
      data[0] = 1;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(0, 150);
      ledcWrite(1, 150);
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
      ledcWrite(0, 150);
      ledcWrite(1, 150);
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
      ledcWrite(0, 150);
      ledcWrite(1, 150);
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
      ledcWrite(0, 150);
      ledcWrite(1, 150);
      digitalWrite(5, LOW);
      digitalWrite(18, LOW);
    }

    else if (PS4.Square() && millis()-passing_debounce>500){
      data[0] = 5;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      //picking
      if(passing ==0){
       ledcWrite(2, 82);
      digitalWrite(22, HIGH);
      passing = 1;
      }else if(passing ==1){
        ledcWrite(2, 0);
      digitalWrite(22, HIGH);
      passing = 0;
      }
      passing_debounce = millis();
    }

    else if (PS4.Circle() && millis() - stepperDebounce>500){ //servo
      data[0] = 6;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      //servo
      if(angle == 0){
      ledcWrite(4, 13);
      angle = 1;
      }else if(angle == 1){
      ledcWrite(4, 5);
      angle = 0;
      }
      stepperDebounce = millis();
    }
    else if (PS4.L1()){
    data[0] = 7;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(5, 255);
       digitalWrite(9, LOW);
  }
  else if (PS4.L2()){
    data[0] = 8;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      //BLDC
     if(millis()- bldc_debounce>500 && bldc == 0){
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      bldc = 1;
      bldc_debounce = millis();
     }else if(millis()- bldc_debounce>500 && bldc == 1){
       digitalWrite(12, LOW);
      digitalWrite(13, LOW);
        bldc = 0;
      bldc_debounce = millis();
     }
  }
  else if (PS4.R1()){
    data[0] = 9;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
        ledcWrite(5, 255);
      digitalWrite(9, HIGH);
  }else if (PS4.R2()){
    data[0] = 10;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
  }else if(PS4.Triangle()){
    data[0] = 11;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      //picking
      ledcWrite(3, 51);
      digitalWrite(23, HIGH);
  }else if(PS4.Cross()){
    data[0] = 12;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      ledcWrite(3, 51);
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
