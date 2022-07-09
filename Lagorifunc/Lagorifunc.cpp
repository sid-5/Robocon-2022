#include <Arduino.h>
#include <esp_system.h>
#include "Lagorifunc.h"
Lagorifunc::Lagorifunc(int grabChannel, int grabPWM, int grabDir,int liftChannel, int liftPWM, int liftDir, int frequency, int channel_bit){
	grab_pwm = grabPWM;
	grab_dir = grabDir;
	lift_pwm = liftPWM;
	lift_dir = liftDir;
	lift_channel = liftChannel;
	grab_channel = grabChannel;
	ledcSetup(grab_channel, frequency, channel_bit); // Lagori Grab
	ledcSetup(lift_channel, frequency, channel_bit); // Lagori Lift
	ledcAttachPin(grab_pwm, grab_channel); // D19 -> Lagori Grab
	ledcAttachPin(lift_pwm, lift_channel); // D21 -> Lagori Lift
	pinMode(grab_dir, OUTPUT); // D22 -> Lagori Grab Dir
	pinMode(lift_dir, OUTPUT); // D23 -> Lagori Lift Dir
  }
void Lagorifunc::grab(int dir){  //1-grab, 0-release
	if(dir){        
	digitalWrite(grab_dir, LOW);
	ledcWrite(grab_channel, 100);        
	}else{
	digitalWrite(grab_dir, HIGH);
	 ledcWrite(grab_channel, 100);        
	}
  }

void Lagorifunc::lift(int dir){ //1-up, 0-down
	if(dir){
		digitalWrite(lift_dir, LOW);
		ledcWrite(lift_channel, 255); 
	}else{
		digitalWrite(lift_dir, HIGH);
		ledcWrite(lift_channel, 255);
	}
  }

void Lagorifunc::allLagoriZero(){
    
	ledcWrite(grab_channel, 0);
	ledcWrite(lift_channel, 0);
  }
