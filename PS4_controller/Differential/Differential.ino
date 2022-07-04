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

//macros for pin definations
#define loco_dir1_pin 18
#define loco_pwm1_pin 16  //RX2
#define loco_dir2_pin 5
#define loco_pwm2_pin 17  //TX2

#define lagori_grab_dir_pin 22
#define lagori_grab_pwm_pin 19

#define lagori_pick_dir_pin 23
#define lagori_pick_pwm_pin 21

//limit switch
#define ls_1_pin 33  //release
#define ls_2_pin 34  //grab
#define ls_3_pin 13  //lift
#define ls_4_pin 35

//variables to store state of ps4 elements
int left_x = 0;
int left_y = 0;
int right_x = 0;
int right_y = 0;
byte r2 = 0;
byte l2 = 0;
byte r1 = 0;
byte l1 = 0;
byte square_button = 0;
byte cross_button = 0;
byte triangle_button = 0;
byte circle_button = 0;
byte battery = 0;

//variables for locomotion
int M1_pwm = 0;
byte M1_dir = 0;
int M2_pwm = 0;
byte M2_dir = 0;

//variables for lagori
byte lagoriPick_pwm = 0;
byte lagoriPick_dir = 0;
byte lagoriGrab_pwm = 0;
byte lagoriGrab_dir = 0;

//limit switch
byte ls_1 = 0;
byte ls_2 = 0;
byte ls_3 = 0;
byte ls_4 = 0;

//-----------------------------------------------------------------------------------------------//

void setup() {
  Serial.begin(115200);

  //mac address of blue ps4
  PS4.begin("54:8d:5a:88:d9:f1");

  //mac address of red ps4
  //PS4.begin("48:5f:99:09:76:b2");

  //locomotion setup
  ledcSetup(0, 5000, 8);            //loco1
  ledcSetup(1, 5000, 8);            //loco2
  ledcAttachPin(loco_pwm1_pin, 0);  //RX2
  ledcAttachPin(loco_pwm2_pin, 1);  //TX2
  pinMode(loco_dir1_pin, OUTPUT);   //loco1_dir D5
  pinMode(loco_dir2_pin, OUTPUT);   // loco2_dir D18

  //logori setup
  ledcSetup(2, 5000, 8);                  // Lagori Grab
  ledcSetup(3, 5000, 8);                  // Lagori Lift
  ledcAttachPin(lagori_grab_pwm_pin, 2);  // D19 -> Lagori Grab
  ledcAttachPin(lagori_pick_pwm_pin, 3);  // D21 -> Lagori Lift
  pinMode(lagori_grab_dir_pin, OUTPUT);   // D22 -> Lagori Grab Dir
  pinMode(lagori_pick_dir_pin, OUTPUT);   // D23 -> Lagori Lift Dir

  pinMode(ls_1_pin, INPUT);
  pinMode(ls_2_pin, INPUT);
  pinMode(ls_3_pin, INPUT);
  pinMode(ls_4_pin, INPUT);
}

//-----------------------------------------------------------------------------------------------//

void loop() {
  readValues();

  if (PS4.isConnected()) {
    calculateLocValues();
    driveLocMotors();

    calculateLagoriValues();
    driveLagoriMotor();
  } else {
    M1_pwm = 0;
    M2_pwm = 0;
    driveLocMotors();

    lagoriGrab_pwm = 0;
    lagoriPick_pwm = 0;
    driveLagoriMotor();
  }
  printValues();
  sendValues();
}

//-----------------------------------------------------------------------------------------------//

void calculateLocValues() {
  // 1. mapped left_y joystick for translation and right_x for rotation - linear mapping
  //M1_pwm = 2 * (left_y + right_x);
  //M2_pwm = 2 * (left_y - right_x);

  // 2. mapped left_x joystick for rotation and r2 for trottle and l2 for reverse -linear mapping
  //M1_pwm = 2 * (((r2 + -l2) / 2) + left_x);
  //M2_pwm = 2 * (((r2 + -l2) / 2) - left_x);


  // 3. step mapping, haven't used equations cause need need to fine tune each situation
  //cannot use both sticks simultaneously
  if (((left_y > 27) || (left_y < -27)) && ((right_x < 27) && (right_x > -27))) {
    //forward
    if (left_y > 77) {
      M1_pwm = 255;
      M2_pwm = 255;
    } else if (left_y > 27 && left_y < 77) {
      M1_pwm = 150;
      M2_pwm = 150;
    }
    //reverse
    else if (left_y < -27 && left_y > -77) {
      M1_pwm = -150;
      M2_pwm = -150;
    } else if (left_y < -77) {
      M1_pwm = -255;
      M2_pwm = -255;
    }
  } else if (((left_y < 27) && (left_y > -27)) && ((right_x > 27) || (right_x < -27))) {
    //right
    if (right_x > 77) {
      M1_pwm = 255;
      M2_pwm = -255;
    } else if (right_x > 27 && right_x < 77) {
      M1_pwm = 150;
      M2_pwm = -150;
    }
    //left
    else if (right_x < -27 && right_x > -77) {
      M1_pwm = -150;
      M2_pwm = 150;
    } else if (right_x < -77) {
      M1_pwm = -255;
      M2_pwm = 255;
    }
  } else {
    M1_pwm = 0;
    M2_pwm = 0;
  }

  //direction deduction from mapped pwm values
  //dir = 0 bot moves forward
  if (M1_pwm > 0) {
    M1_dir = 0;
  } else {
    M1_dir = 1;
    M1_pwm = abs(M1_pwm);
  }

  if (M2_pwm > 0) {
    M2_dir = 0;
  } else {
    M2_dir = 1;
    M2_pwm = abs(M2_pwm);
  }

  M1_pwm = constrain(M1_pwm, 0, 255);
  M2_pwm = constrain(M2_pwm, 0, 255);
}

//-----------------------------------------------------------------------------------------------//

void driveLocMotors() {
  //at dir = 0 bot moves forward
  digitalWrite(loco_dir1_pin, M1_dir);
  ledcWrite(0, M1_pwm);

  digitalWrite(loco_dir2_pin, M2_dir);
  ledcWrite(1, M2_pwm);
}

//-----------------------------------------------------------------------------------------------//

void calculateLagoriValues() {
  //r1 and l1 = 1 => button is pressed
  //ls_1 and ls_4 = 0 => limit switch triggered

  //lagoriGrab_pwm = 150 * (!l1 & r1 & ls_2) | (l1 & !r1 & ls_1);
  lagoriGrab_pwm = 100 * ((!l1 & r1) | (l1 & !r1 & ls_1));  //limt switch 2 not working

  lagoriGrab_dir = r1;  //dir = 1, lagori is grabbed

  lagoriPick_pwm = 255 * ((!r2 & l2) | (r2 & !l2 & ls_3));
  lagoriPick_dir = l2;  // dir = 0, lagori is picked up
}

//-----------------------------------------------------------------------------------------------//

void driveLagoriMotor() {
  ledcWrite(2, lagoriGrab_pwm);
  digitalWrite(lagori_grab_dir_pin, lagoriGrab_dir);

  ledcWrite(3, lagoriPick_pwm);
  digitalWrite(lagori_pick_dir_pin, lagoriPick_dir);
}

//-----------------------------------------------------------------------------------------------//

void readValues() {
  left_x = PS4.LStickX();
  left_y = PS4.LStickY();

  right_x = PS4.RStickX();
  right_y = PS4.RStickY();

  r2 = PS4.R2();
  l2 = PS4.L2();

  r1 = PS4.R1();
  l1 = PS4.L1();

  circle_button = PS4.Circle();
  square_button = PS4.Square();
  triangle_button = PS4.Triangle();
  cross_button = PS4.Cross();

  battery = PS4.Battery();

  ls_1 = digitalRead(ls_1_pin);
  ls_2 = digitalRead(ls_2_pin);
  ls_3 = digitalRead(ls_3_pin);
  ls_4 = digitalRead(ls_4_pin);
}

//-----------------------------------------------------------------------------------------------//

void printValues() {
  Serial.print("\t");
  Serial.print(ls_1);
  Serial.print("\t");
  Serial.print(ls_2);
  Serial.print("\t");
  Serial.print(ls_3);
  Serial.print("\t");
  Serial.print(lagoriGrab_pwm);
  Serial.print("\t");
  Serial.print(lagoriPick_pwm);
  Serial.print("\t");
  Serial.println(battery);
}

//-----------------------------------------------------------------------------------------------//

void sendValues() {
  PS4.setLed(0, 255, 255);
  PS4.sendToController();
}

//-----------------------------------------------------------------------------------------------//
