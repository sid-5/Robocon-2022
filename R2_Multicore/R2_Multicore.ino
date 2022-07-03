#include <PS4Controller.h>

int conn = 0;

int prev_time = 0;

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
bool connectedd = 0;

//int triangle, circle, cross, r1, r2, l1, l2, LStickY, RStickX;
//int up=0;int down=0;int right=0;int left=0;

int action = -1;

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


  
  xTaskCreatePinnedToCore(
                    LagoriPickCode,   /* Task function. */
                    "LagoriPick",     /* name of task. */
                    40000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    3,           /* priority of the task */
                    NULL,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */  
  delay(500);
  
  xTaskCreatePinnedToCore(
                    TasksCode,   /* Task function. */
                    "Tasks",     /* name of task. */
                    40000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    3,           /* priority of the task */
                    NULL,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */  
  delay(500);
}


//void readValues(){
//  triangle = PS4.Triangle();
//  circle = PS4.Circle();
//  cross = PS4.Cross();
//  l1 = PS4.L1();
//  l2 = PS4.L2();
//  r1 = PS4.R1();
//  r2 = PS4.R2();
//  LStickY = PS4.LStickY();
//  RStickX = PS4.RStickX();
//  up = PS4.Up();
//  down = PS4.Down();
//}


void TasksCode(void * pvParameters) {
  PS4.begin("54:8d:5a:88:d9:f1");
  Serial.println("core 1 out");
  for(;;){
    if (millis()-prev_time>150){
  if (PS4.isConnected()) {
    Serial.println("core 1");
    
         if (80>PS4.LStickY()>16){
        ledcWrite(0, pwm_low);
        ledcWrite(1, pwm_low);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, LOW);  
      }
      else if (80<PS4.LStickY()){
        ledcWrite(0, pwm_high);
        ledcWrite(1, pwm_high);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, LOW);  
      }
      else if (-80<PS4.LStickY()<-16){
        ledcWrite(0, pwm_low);
        ledcWrite(1, pwm_low);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, HIGH); 
      }
      else if (-80>PS4.LStickY()) {
        ledcWrite(0, pwm_high);
        ledcWrite(1, pwm_high);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, HIGH); 
      }
      else if (80>PS4.RStickX()>16){
        ledcWrite(0, pwm_low);
        ledcWrite(1, pwm_low);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, LOW); 
      }
      else if (80<PS4.RStickX()){
        ledcWrite(0, pwm_high);
        ledcWrite(1, pwm_high);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, LOW); 
      }
      else if (-80<PS4.RStickX()<-16){
        ledcWrite(0, pwm_low);
        ledcWrite(1, pwm_low);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, HIGH); 
      }
      else if (-80>PS4.RStickX()){
        ledcWrite(0, pwm_high);
        ledcWrite(1, pwm_high);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, HIGH); 
      }
       else if (80>PS4.LStickY()>16){
        ledcWrite(0, pwm_low);
        ledcWrite(1, pwm_low);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, LOW);  
      }
      else if (80<PS4.LStickY()){
        ledcWrite(0, pwm_high);
        ledcWrite(1, pwm_high);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, LOW);  
      }
      else if (-80<PS4.LStickY()<-16){
        ledcWrite(0, pwm_low);
        ledcWrite(1, pwm_low);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, HIGH); 
      }
      else if (-80>PS4.LStickY()) {
        ledcWrite(0, pwm_high);
        ledcWrite(1, pwm_high);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, HIGH); 
      }
      else if (80>PS4.RStickX()>16){
        ledcWrite(0, pwm_low);
        ledcWrite(1, pwm_low);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, LOW); 
      }
      else if (80<PS4.RStickX()){
        ledcWrite(0, pwm_high);
        ledcWrite(1, pwm_high);
        digitalWrite(loco_dir1, LOW);
        digitalWrite(loco_dir2, LOW); 
      }
      else if (-80<PS4.RStickX()<-16){
        ledcWrite(0, pwm_low);
        ledcWrite(1, pwm_low);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, HIGH); 
      }
      else if (-80>PS4.RStickX()){
        ledcWrite(0, pwm_high);
        ledcWrite(1, pwm_high);
        digitalWrite(loco_dir1, HIGH);
        digitalWrite(loco_dir2, HIGH); 
      }
      
      else{
        ledcWrite(0, 0);
        ledcWrite(1, 0);
      }

      if (PS4.Up()){
        action = 1;
      }
      else if (PS4.Down()){
        action = 2;
      }
      else if (PS4.Left()){
        action = 3;
      }
      else if (PS4.Right()){
        action = 4;
      }
      else{
        action = 0;
      }

      if (PS4.L1()) {
        Serial.println("L1");
        ledcWrite(2, 120);
        digitalWrite(lagori_grab_dir, LOW);
        curr_pwm = 120;
        grab_flag = 1;
      }

      else if (PS4.R1()) {
        Serial.println("R1");
        ledcWrite(2, 120);
        digitalWrite(lagori_grab_dir, HIGH);
        curr_pwm = 120;
        grab_flag = 1;
      }

      else if (PS4.L2() && (digitalRead(35))) {
        Serial.println("L2");
        ledcWrite(3, 255);
        digitalWrite(lagori_lift_dir, LOW);
      }

      else if (PS4.R2() && (digitalRead(34))) {
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
//        ledcWrite(4, 160);
//        digitalWrite(ball_pick_dir, LOW);
          if (ball_pick_flag==0){
            ball_pick_flag=1;
          }
          else{
            ball_pick_flag=0;
          }
          Serial.println(ball_pick_flag);
      }
      else{
        ledcWrite(2, 0);
        ledcWrite(3, 0);
        // ledcWrite(4, 0);
        ledcWrite(5, 0);
        if (ball_pick_flag){
            ledcWrite(6, 160);
            Serial.println(ball_pick_flag);
        }
        else{
          ledcWrite(6, 0);
          ball_pick_flag = 0;
          //Serial.println("0");
        }
        grab_flag = 0;      
      }
    }
    vTaskDelay(50);
    prev_time = millis();
  }
  }
}


void LagoriPickCode(void * pvParameters){
    for(;;){
      if(action==1){
        ledcWrite(3,255);
        digitalWrite(lagori_lift_dir,HIGH);
        Serial.println(action);
        vTaskDelay(2000);
        ledcWrite(3,0);
      }
      else if(action==2){
        ledcWrite(3,255);
        digitalWrite(lagori_lift_dir,LOW);
        Serial.println(action);
        vTaskDelay(3000);
        ledcWrite(3,0);
      }
      else{
        ledcWrite(3,0);
      }
      vTaskDelay(50);
    }
}

void loop() {
  // put your main code here, to run repeatedly:

}
