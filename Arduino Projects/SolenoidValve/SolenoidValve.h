/*
 * SolenoidValve.h
 *
 *  Created on: 2015Äê8ÔÂ6ÈÕ
 *      Author: Chris
 */

#ifndef SOLENOIDVALVE_SOLENOIDVALVE_H_
#define SOLENOIDVALVE_SOLENOIDVALVE_H_

class SolenoidValve
{
public:
	void init(const int ctrlPin);
	void start(void);
	void stop(void);
private:
	int pin;
	bool state;
};

#endif /* SOLENOIDVALVE_SOLENOIDVALVE_H_ */
