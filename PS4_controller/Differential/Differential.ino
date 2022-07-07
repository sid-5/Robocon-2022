/*                                   
          |-------|                               |-------|
          |       |                               |       |
          |  R1   |                               |   R2  |
          |       |                               |       |
          |-------|                               |-------|
              |                                       |
              |                                       |
              |                                       |
              |                                       |
              |                                       |
              |                                       |
              |                                       |
              |                                       |
              |                                       |
              |                                       |
              |        LS_1----------------LS_2       |
              |                                       |
              |    LS_3                               |
          |-------|                               |-------|
          |       |                               |       |
          |   M1  |-------------------------------|   M2  |
          |       |                               |       |
          |-------|                               |-------|
*/

//library
#include <PS4Controller.h>

int conn = 0;

int pwm_high = 230;
int pwm_low = 160;

int loco_dir1 = 5;
int loco_pwm1 = 16; //RX2
int loco_dir2 = 18;
int loco_pwm2 = 17; //TX2
 
int lagori_grab_dir = 22;

int lagori_grab_pwm = 19;

int lagori_lift_dir = 23;
int lagori_lift_pwm = 21;

int ball_pass_dir = 26;
int ball_pass_pwm = 27;

int ball_pick_dir = 25;
int ball_pick_pwm = 32;

int curr_pwm = 0;
bool grab_flag = 0;
bool ball_pick_flag = 0;
int action = 0;
int prev_time = 0;
//-----------------------------------------------------------------------------------------------//

void setup() {
  // put your setup code here, to run once:
  ledcSetup(0, 5000, 8); //loco1 
  ledcSetup(1, 5000, 8); //loco2
  ledcAttachPin(loco_pwm1, 0); //RX2
  ledcAttachPin(loco_pwm2, 1); //TX2
  pinMode(loco_dir1, OUTPUT); //loco1_dir D5
  pinMode(loco_dir2, OUTPUT); // loco2_dir D18

  ledcSetup(2, 5000, 8); // Lagori Grab
  ledcSetup(3, 5000, 8); // Lagori Lift

  ledcAttachPin(lagori_grab_pwm, 2); // D19 -> Lagori Grab
  ledcAttachPin(lagori_lift_pwm, 3); // D21 -> Lagori Lift
  pinMode(lagori_grab_dir, OUTPUT); // D22 -> Lagori Grab Dir
  pinMode(lagori_lift_dir, OUTPUT); // D23 -> Lagori Lift Dir

  ledcSetup(6, 5000, 8); // Ball Pass
  ledcSetup(5, 5000, 8); // Ball Pick

  ledcAttachPin(ball_pass_pwm, 5); // D27 -> Ball Pass
  ledcAttachPin(ball_pick_pwm, 6); // D32 -> Ball Pick
  pinMode(ball_pick_dir, OUTPUT); // D25 -> Ball Pick Dir
  pinMode(ball_pass_dir, OUTPUT); // D26 -> Ball Pass Dir

  pinMode(35, INPUT); // Top Limit Switch
  pinMode(34, INPUT); // Bottom Limit Switch

  
  Serial.begin(115200);
  PS4.begin("54:8d:5a:88:d9:f1");
//-----------------------------------------------------------------------------------------------//
}

void loop() {
  if (PS4.isConnected()) {
    if (100>PS4.LStickY() && PS4.LStickY()>25){
      ledcWrite(0, pwm_low);
      ledcWrite(1, pwm_low);
      digitalWrite(loco_dir1, LOW);
      digitalWrite(loco_dir2, LOW); 
      Serial.print("In ");
      Serial.println(PS4.LStickY()); 
    }
    else if (100<PS4.LStickY()){
      ledcWrite(0, pwm_high);
      ledcWrite(1, pwm_high);
      digitalWrite(loco_dir1, LOW);
      digitalWrite(loco_dir2, LOW);  
      Serial.println(PS4.LStickY());
    }
    else if (-100<PS4.LStickY() && PS4.LStickY()<-25){
      ledcWrite(0, pwm_low);
      ledcWrite(1, pwm_low);  
      digitalWrite(loco_dir1, HIGH);
      digitalWrite(loco_dir2, HIGH); 
      Serial.println(PS4.LStickY());
    }
    else if (-100>PS4.LStickY()) {
      ledcWrite(0, pwm_high);
      ledcWrite(1, pwm_high);
      digitalWrite(loco_dir1, HIGH);
      digitalWrite(loco_dir2, HIGH); 
      Serial.println(PS4.LStickY());
    }
    else if (100>PS4.RStickX() && PS4.RStickX()>25){
      ledcWrite(0, pwm_low);
      ledcWrite(1, pwm_low);
      digitalWrite(loco_dir1, HIGH);
      digitalWrite(loco_dir2, LOW);
      Serial.println(PS4.RStickX()); 
    }
    else if (100<PS4.RStickX()){
      ledcWrite(0, pwm_high);
      ledcWrite(1, pwm_high);
      digitalWrite(loco_dir1, HIGH);
      digitalWrite(loco_dir2, LOW); 
      Serial.println(PS4.RStickX());
    }
    else if (-100<PS4.RStickX() && PS4.RStickX()<-25){
      ledcWrite(0, pwm_low);
      ledcWrite(1, pwm_low);
      digitalWrite(loco_dir1, LOW);
      digitalWrite(loco_dir2, HIGH);
      Serial.println(PS4.RStickX()); 
    }
    else if (-100>PS4.RStickX()){
      ledcWrite(0, pwm_high);
      ledcWrite(1, pwm_high);
      digitalWrite(loco_dir1, LOW);
      digitalWrite(loco_dir2, HIGH); 
      Serial.println(PS4.RStickX());
    }      
    else{
      ledcWrite(0, 0);
      ledcWrite(1, 0);
    }

    if (PS4.Up()){
      Serial.println("UP");
      action = 1;
      prev_time=millis();
    }
    else if (PS4.Down()){
      action = -1;
      prev_time=millis();
    }
    else if (PS4.Left()){
      action = 3;
      prev_time=millis();
    }
    else if (PS4.Right()){
      action = 2;
      prev_time=millis();
    }

    if (PS4.L1()) {
      Serial.println("L1");
      ledcWrite(2, 100);
      digitalWrite(lagori_grab_dir, LOW);
      curr_pwm = 120;
      grab_flag = 1;
    }
    else if (PS4.R1()) {
      Serial.println("R1");
      ledcWrite(2, 100);
      digitalWrite(lagori_grab_dir, HIGH);
      curr_pwm = 120;
      grab_flag = 1;
    }
    else if (PS4.L2() && (digitalRead(13))) { //&& (digitalRead(35))
      Serial.println("L2");
      ledcWrite(3, 255);
      digitalWrite(lagori_lift_dir, LOW);
    }
    else if (PS4.R2()) {
      Serial.println("R2");
      ledcWrite(3, 255);
      digitalWrite(lagori_lift_dir, HIGH);
    }
    else if (PS4.Triangle()){ // Ball pass
      Serial.println("Triangle");
      ledcWrite(5, 20);
      digitalWrite(ball_pass_dir, LOW);
    }
    else if (PS4.Cross()){ // Ball Pass
      Serial.println("Cross");
      ledcWrite(5, 20);
      digitalWrite(ball_pass_dir, HIGH);
    }
    else if ((PS4.Circle()) ){ // ball pick
      Serial.println("Circle");
//      ledcWrite(4, 160);
//      digitalWrite(ball_pick_dir, LOW);
      if (ball_pick_flag==0){
        ball_pick_flag=1;
      }
      else{
        ball_pick_flag=0;
      }
      if (ball_pick_flag){
        ledcWrite(6, 160);
        Serial.println(ball_pick_flag);
      }
      else{
        ledcWrite(6, 0);
        ball_pick_flag = 0;
        //Serial.println("0");
      }
    } 
    else{
      ledcWrite(2, 0);
      ledcWrite(3, 0);
      // ledcWrite(4, 0);
      ledcWrite(5, 0);
      grab_flag = 0;    
    }
    if (action==1) {
      if (millis()-prev_time<1000) {
        digitalWrite(lagori_lift_dir,HIGH);
        Serial.println(action);
        ledcWrite(3,255);
        //prev_time = millis();
      }
      else{
        action=0;
        ledcWrite(3, 0);
      }
    }
    else if (action==2) {
      // int prev_time=millis();
      if (millis()-prev_time<2000) {
        digitalWrite(lagori_lift_dir,HIGH);
        Serial.println(action);
        ledcWrite(3,255);
        // prev_time = millis();
      }
      else{
        action=0;
        ledcWrite(3, 0);
      }
    }
    else if (action==3) {
//      int prev_time=millis();
      if (millis()-prev_time<3000) {
        digitalWrite(lagori_lift_dir,HIGH);
        Serial.println(action);
        ledcWrite(3,255);
        // prev_time = millis();
      }
      else{
        action=0;
        ledcWrite(3, 0);
      }
    }
    else if (action==-1){
      if (!digitalRead(13)){
        digitalWrite(lagori_lift_dir,LOW);
        Serial.println(action);
        ledcWrite(3,255);
      }
      else{
        ledcWrite(3, 0);
        action=0;
      }
    }
  }
  delay(50);
}
