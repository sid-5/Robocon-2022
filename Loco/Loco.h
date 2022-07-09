#ifndef Loco_h
#define Loco_h
#include <Arduino.h>
#include <esp_system.h>
class Loco {
public:
	Loco(int m1Channel, int m1PWM, int m1Dir, int m2Channel, int m2PWM, int m2Dir, int freq, int channel_bit, int pwmVal);
	void forward();
	void backward();
	void left();
	void right();
	void allLocoZero();
	int m1_pwm;
	int m1_dir;
	int m2_pwm;
	int m2_dir;
	int m1_channel;
	int m2_channel;
	int pwm_val;

	
};
#endif
