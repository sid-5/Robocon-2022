#ifndef Lagorifunc_h
#define Lagorifunc_h
#include <Arduino.h>
#include <esp_system.h>
class Lagorifunc {
public:
	Lagorifunc(int grabChannel, int grabPWM, int grabDir,int liftChannel, int liftPWM, int liftDir, int frequency, int channel_bit);
	void grab(int dir);
	void lift(int dir);
	void allLagoriZero();
	int grab_pwm;
	int grab_dir;
	int lift_pwm;
	int lift_dir;
	int grab_channel;
	int lift_channel;
	
};
#endif
