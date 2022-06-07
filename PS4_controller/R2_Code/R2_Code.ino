#include <PS4Controller.h>
int prev_time = 0;

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

void setup() {
  // put your setup code here, to run once:
  // PS4.begin("30:94:35:32:df:3e");
  PS4.begin("18:26:49:32:57:0C");
  // PS4.begin("cc:66:0a:c9:41:d1");
  Serial.begin(115200);
  
  ledcSetup(0, 5000, 8); //loco1 
  ledcSetup(1, 5000, 8); //loco2
  ledcAttachPin(loco_pwm1, 0); //RX2
  ledcAttachPin(loco_pwm2, 1); //TX2
  pinMode(loco_dir1, OUTPUT); //loco1_dir D5
  pinMode(loco_dir2, OUTPUT); // loco2_dir D18

  ledcSetup(2, 5000, 8); // Lagori Grab
  ledcSetup(3, 5000, 8); // Lagori Lift

  ledcAttachPin(lagori_grab_pwm, 2); // D32 -> Lagori Grab
  ledcAttachPin(lagori_lift_pwm, 3); // D25 -> Lagori Lift
  pinMode(lagori_grab_dir, OUTPUT); // D33 -> Lagori Grab Dir
  pinMode(lagori_lift_dir, OUTPUT); // D26 -> Lagori Lift Dir

  ledcSetup(4, 5000, 8); // Ball Pass
  ledcSetup(5, 5000, 8); // Ball Pick

  ledcAttachPin(ball_pass_pwm, 5); // D4 -> Ball Pick
  ledcAttachPin(ball_pick_pwm, 4); // D16 -> Ball Pass
  pinMode(ball_pick_dir, OUTPUT); // Ball Pick Dir
  pinMode(ball_pass_dir, OUTPUT); // Ball Pass Dir

  pinMode(35, INPUT); // Top Limit Switch
  pinMode(34, INPUT); // Bottom Limit Switch
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis()-prev_time>70){
    if (PS4.isConnected()){
      if (PS4.Up()){
        ledcWrite(0, 160);
        ledcWrite(1, 160);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, LOW);      
      }
      
      else if (PS4.Down()){
        ledcWrite(0, 160);
        ledcWrite(1, 160);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, HIGH);  
      }
      
      else if (PS4.Left()){
        ledcWrite(0, 160);
        ledcWrite(1, 160);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, HIGH);  
      }
      
      else if (PS4.Right()){
        ledcWrite(0, 160);
        ledcWrite(1, 160);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, LOW);  
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
    
      if (PS4.Square()){ //Lagori Grab
          Serial.println("Square");
          ledcWrite(2, 100);
          digitalWrite(lagori_grab_dir, LOW);
          curr_pwm = 160;
          grab_flag = 1;
      }
      else if ((PS4.Triangle()) && (digitalRead(35))){ // Not kadhlay for testing. LIMIT SWITCH lavla ki parat taaka. 
        // Lagori Lift
        Serial.println("Triangle");
        ledcWrite(3, 160);
        digitalWrite(lagori_lift_dir, HIGH);
      }
      else if (PS4.Circle()){ //Lagori Grab
        Serial.println("Circle");
        ledcWrite(2, 100);
        digitalWrite(lagori_grab_dir, HIGH);
        curr_pwm = 160;
        grab_flag = 1;
      }
      else if ((PS4.Cross()) && (digitalRead(34))){ // Not kadhlay for testing. LIMIT SWITCH lavla ki parat taaka.
        // Lagori Lift down
        Serial.println("Cross");
        ledcWrite(3, 160);
        digitalWrite(lagori_lift_dir, LOW);
      }
      else if (PS4.L1()){ // Ball pass
        Serial.println("L1");
        ledcWrite(5, 20);
        digitalWrite(ball_pass_dir, LOW);
      }
      else if (PS4.L2()){ // Ball Pass
        Serial.println("L2");
        ledcWrite(5, 20);
        digitalWrite(ball_pass_dir, HIGH);
      }
      else if (PS4.R1()){ // ball pick
        Serial.println("R1");
//        ledcWrite(4, 160);
//        digitalWrite(ball_pick_dir, LOW);
          ball_pick_flag = (!ball_pick_flag) ? 1 : 0;
          Serial.println(ball_pick_flag);
      }
      else if (PS4.R2()){ // Ball Pick
        Serial.println("R2");
//        ledcWrite(4, 160);
//        digitalWrite(ball_pick_dir, HIGH);
      }
      else{
        // ledcWrite(2, 0);
        ledcWrite(3, 0);
        // ledcWrite(4, 0);
        ledcWrite(5, 0);
        if (ball_pick_flag){
            ledcWrite(4, 160);
        }
        else{
          ledcWrite(4, 0);
          ball_pick_flag = 0
        }
        grab_flag = 0;
//        Serial.println(grab_flag);
//        Serial.println(curr_pwm);
//        while (curr_pwm>0){
//          cur_pwm -= 20;
//          ledcWrite(5, curr_pwm);
//          delay(500); //Use software interrupt later. Te,porary used de;ay fpr the controlled stopping.
//        }
      }
    }
    
    while ((curr_pwm>0) && (!grab_flag)){
        curr_pwm -= 80;
        if (curr_pwm<0){
          curr_pwm=0;
        }
        ledcWrite(2, curr_pwm);
        delay(100); //Use software interrupt later. Temporary used delay fpr the controlled stopping.
        Serial.println(curr_pwm);
      }
//    curr_pwm=0;
//    ledcWrite(5, curr_pwm);
    prev_time = millis();
  }
}
