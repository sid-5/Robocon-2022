#include <PS4Controller.h>
#include <Loco.h>
#include <Lagorifunc.h>



int pwm_high = 230;
int pwm_low = 160;

int m1_dir = 5;
int m1_pwm = 16; //RX2

int m2_dir = 18;
int m2_pwm = 17; //TX2
 
int lagori_grab_dir = 22;
int lagori_grab_pwm = 19;

int lagori_lift_dir = 23;
int lagori_lift_pwm = 21;

int ball_pass_dir = 26;
int ball_pass_pwm = 27;

int ball_pick_dir = 25;
int ball_pick_pwm = 32;

int pass_time = 0;
int pass_flag = 0;

int action = 0;
int prev_time = 0;

int pick_bottom_limit = 34;
int grabber_inner_limit = 35;
int grabber_outer_limit = 33;
int pick_top_switch =13;

int red = 2;
int blue = 4;
int green = 14;
//-----------------------------------------------------------------------------------------------//
Lagorifunc l(2,lagori_grab_pwm,lagori_grab_dir, 160 ,3, lagori_lift_pwm,lagori_lift_dir,5000,8, 255);
Loco high(0, m1_pwm, m1_dir, 1, m2_pwm, m2_dir, 5000, 8, 230);
Loco low(0, m1_pwm, m1_dir, 1, m2_pwm, m2_dir, 5000, 8, 100);

void setup() {
  

  ledcSetup(4, 5000, 8); // Ball Pass
  ledcSetup(5, 5000, 8); // Ball Pick

  ledcAttachPin(ball_pass_pwm, 5); // D27 -> Ball Pass
  ledcAttachPin(ball_pick_pwm, 4); // D32 -> Ball Pick
  pinMode(ball_pick_dir, OUTPUT); // D25 -> Ball Pick Dir
  pinMode(ball_pass_dir, OUTPUT); // D26 -> Ball Pass Dir
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(115200);
  PS4.begin("54:8d:5a:88:d9:f1");
  pinMode(grabber_inner_limit, INPUT); 
  pinMode(pick_bottom_limit, INPUT); // Bottom Limit Switch
  pinMode(grabber_outer_limit,INPUT); // horizontal Limit Switch
  pinMode(pick_top_switch,INPUT); // Top Limit Switch
}

void loop() {
  
  if (PS4.isConnected()) {
    if (100>PS4.LStickY() && PS4.LStickY()>25){
      low.forward();
      digitalWrite(red,HIGH);
      digitalWrite(blue,LOW);
      digitalWrite(green,LOW);
    }
    else if (100<PS4.LStickY()){
      high.forward();
       digitalWrite(red,LOW);
      digitalWrite(blue,HIGH);
      digitalWrite(green,LOW);
    }
    else if (-100<PS4.LStickY() && PS4.LStickY()<-25){
      low.backward();
      digitalWrite(red,LOW);
      digitalWrite(blue,LOW);
      digitalWrite(green,HIGH);
    }
    else if (-100>PS4.LStickY()) {
      high.backward();
      digitalWrite(red,HIGH);
      digitalWrite(blue,HIGH);
      digitalWrite(green,LOW);
    }
    else if (100>PS4.RStickX() && PS4.RStickX()>25){
      low.left();
      digitalWrite(red,HIGH);
      digitalWrite(blue,LOW);
      digitalWrite(green,HIGH);
    }
    else if (100<PS4.RStickX()){
      high.left();
      digitalWrite(red,LOW);
      digitalWrite(blue,HIGH);
      digitalWrite(green,HIGH);
    }
    else if (-100<PS4.RStickX() && PS4.RStickX()<-25){
      low.right();
    }
    else if (-100>PS4.RStickX()){
     high.right();
    }      
    else{
      low.allLocoZero();
      high.allLocoZero();
      digitalWrite(blue,LOW);
      digitalWrite(red,LOW);
      digitalWrite(green,LOW);
    }

    /////////////////////////////////////////////////////////////////////////
    if (PS4.L1() && (digitalRead(33))) {
      l.grab(0);
    }
    else if (PS4.R1()  && (digitalRead(35))) {
      l.grab(1);
    }
    else if (PS4.L2() && (digitalRead(34))) { 
      l.lift(0);
    }
    else if (PS4.R2()) {
      l.lift(1);
    }else if(PS4.Square() && ((millis() - pass_time)>1000)){
      if(!pass_flag){
        digitalWrite(ball_pick_dir,HIGH);
        ledcWrite(4, 160);
        pass_time = millis();
        pass_flag = 1;
      }else{
         pass_time = millis();
        pass_flag = 0;
        ledcWrite(4, 0);
      }  
    }
    else if (PS4.Triangle()){ // Ball pass
      digitalWrite(ball_pass_dir, LOW);
      ledcWrite(5, 80);
    }
    else if (PS4.Cross()){ // Ball Pass
      digitalWrite(ball_pass_dir, HIGH);
      ledcWrite(5, 80);
    }else{
      if(!action){
        l.allLagoriZero();
      }
      ledcWrite(5, 0);    
    }

    //////////////////////////////////////////////////////semi automation
    if(millis() - prev_time>500){
    if (PS4.Up() && !(digitalRead(34))){
      action = 2;
      prev_time=millis();
    }
    else if (PS4.Down()){
      action = 4;
      prev_time=millis();
    }
    else if (PS4.Left() && !(digitalRead(34))){
      action = 1;
      prev_time=millis();
    }
    else if (PS4.Right() && !(digitalRead(34))){
      action = 3;
      prev_time=millis();
    }else if (PS4.Circle() && !(digitalRead(34)) ){
      action = 5;
      prev_time=millis();
    }
    }


   ///////////////////////////////////////////////////////////////////////////////////////////
    switch(action){
      case 1:
        if(millis() - prev_time>=1500){
          l.allLagoriZero();
          action = 0;
        }else{
          l.lift(1);
        }
        break;
      case 2:
        if(millis() - prev_time>=2400){
          l.allLagoriZero();
          action = 0;
        }else{
          l.lift(1);
        }
        break;
      case 3:
        if(millis() - prev_time>=3000){
          l.allLagoriZero();
          action = 0;
        }else{
          l.lift(1);
        }
        break;
      case 4:
        if(digitalRead(34)){
          l.lift(0);
        }else{
          l.allLagoriZero();
          action = 0;
        }
        break;
      case 5:
        if(millis() - prev_time>=4000 || !digitalRead(13)){
          l.allLagoriZero();
          action = 0;
        }else{
          l.lift(1);
        }
        break;
    }
}
}
