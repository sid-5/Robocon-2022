#include <Arduino.h>
#include <esp_system.h>
#include "Loco.h"
Loco::Loco(int m1Channel, int m1PWM, int m1Dir, int m2Channel, int m2PWM, int m2Dir, int freq, int channel_bit, int pwmVal) {
	m1_pwm = m1PWM;
	m1_dir = m1Dir;
	m2_pwm = m2PWM;
	m2_dir = m2Dir;
	m1_channel = m1Channel;
	m2_channel = m2Channel;
	pwm_val = pwmVal;
	ledcSetup(m1_channel, freq, channel_bit); // Loco 1
	ledcSetup(m2_channel, freq, channel_bit); // Loco 2
	ledcAttachPin(m1_pwm, m1_channel); // D19 -> Loco 1
	ledcAttachPin(m2_pwm, m2_channel); // D21 -> Loco 2
	pinMode(m1_dir, OUTPUT); 
	pinMode(m2_dir, OUTPUT); 
	}
void Loco::forward(){
    ledcWrite(m1_channel, pwm_val);
      ledcWrite(m2_channel, pwm_val);
      digitalWrite(m1_dir, LOW);
      digitalWrite(m2_dir, LOW); 
  }

void Loco::backward(){
    ledcWrite(m1_channel, pwm_val);
      ledcWrite(m2_channel, pwm_val);  
      digitalWrite(m1_dir, HIGH);
      digitalWrite(m2_dir, HIGH); 
  }

void Loco::left(){
    ledcWrite(m1_channel, pwm_val);
      ledcWrite(m2_channel, pwm_val);  
      digitalWrite(m1_dir, LOW);
      digitalWrite(m2_dir, HIGH);
  }

void Loco::right(){
    ledcWrite(m1_channel, pwm_val);
      ledcWrite(m2_channel, pwm_val);  
      digitalWrite(m1_dir, HIGH);
      digitalWrite(m2_dir, LOW);
  }

void Loco::allLocoZero(){
    ledcWrite(m1_channel, 0);
      ledcWrite(m2_channel, 0);
  }
