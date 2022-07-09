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
//-----------------------------------------------------------------------------------------------//
Lagorifunc l(2,lagori_grab_pwm,lagori_grab_dir,3, lagori_lift_pwm,lagori_lift_dir,5000,8);
Loco high(0, m1_pwm, m1_dir, 1, m2_pwm, m2_dir, 5000, 8, 230);
Loco low(0, m1_pwm, m1_dir, 1, m2_pwm, m2_dir, 5000, 8, 100);

void setup() {
  

  ledcSetup(4, 5000, 8); // Ball Pass
  ledcSetup(5, 5000, 8); // Ball Pick

  ledcAttachPin(ball_pass_pwm, 5); // D27 -> Ball Pass
  ledcAttachPin(ball_pick_pwm, 4); // D32 -> Ball Pick
  pinMode(ball_pick_dir, OUTPUT); // D25 -> Ball Pick Dir
  pinMode(ball_pass_dir, OUTPUT); // D26 -> Ball Pass Dir
  Serial.begin(115200);
  PS4.begin("54:8d:5a:88:d9:f1");
  pinMode(35, INPUT); // Top Limit Switch
  pinMode(34, INPUT); // Bottom Limit Switch
}

void loop() {
  if (PS4.isConnected()) {
    if (100>PS4.LStickY() && PS4.LStickY()>25){
      low.forward();
    }
    else if (100<PS4.LStickY()){
      high.forward();
    }
    else if (-100<PS4.LStickY() && PS4.LStickY()<-25){
      low.backward();
    }
    else if (-100>PS4.LStickY()) {
      high.backward();
    }
    else if (100>PS4.RStickX() && PS4.RStickX()>25){
      low.right();
    }
    else if (100<PS4.RStickX()){
      high.left();
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
    }

    /////////////////////////////////////////////////////////////////////////
    if (PS4.L1()) {
      l.grab(0);
    }
    else if (PS4.R1()) {
      l.grab(1);
    }
    else if (PS4.L2()){// && (digitalRead(13))) { //&& (digitalRead(35))
      l.lift(0);
    }
    else if (PS4.R2()) {
      l.lift(1);
    }
    else if(PS4.Square()){    // ball pick
      if(!pass_flag && ((millis() - pass_time)>200)){
        ledcWrite(4, 50);
        pass_time = millis();
        pass_flag = 1;
      }else{
        pass_flag = 0;
        ledcWrite(4, 0);
      }  
    }
    else if (PS4.Triangle()){ // Ball pass
      digitalWrite(ball_pass_dir, LOW);
      ledcWrite(5, 20);
    }
    else if (PS4.Cross()){ // Ball Pass
      digitalWrite(ball_pass_dir, HIGH);
      ledcWrite(5, 20);
      
    }
    else if ((PS4.Circle()) ){ //free button
      
    } 
    else{
      l.allLagoriZero();
      ledcWrite(5, 0);    
    }

    //semi automation
    if(millis() - prev_time>1000){
    if (PS4.Up()){
      action = 2;
      prev_time=millis();
    }
    else if (PS4.Down()){
      action = 4;
      prev_time=millis();
    }
    else if (PS4.Left()){
      action = 1;
      prev_time=millis();
    }
    else if (PS4.Right()){
      action = 3;
      prev_time=millis();
    }
    }

    switch(action){
      case 1:
        if(millis() - prev_time>=1000){
          l.allLagoriZero();
          action = 0;
        }else{
          l.lift(1);
        }
        break;
      case 2:
        if(millis() - prev_time>=2000){
          l.allLagoriZero();
          action = 0;
        }else{
          l.lift(1);
        }
        break;
      case 3:
        if(millis() - prev_time>=2500){
          l.allLagoriZero();
          action = 0;
        }else{
          l.lift(1);
        }
        break;
      case 4:
        action = 0;
        break;
    }
}
}
