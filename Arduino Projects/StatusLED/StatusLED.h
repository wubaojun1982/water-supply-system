/*
 * StatusLED.h
 *
 *  Created on: 2015Äê8ÔÂ5ÈÕ
 *      Author: Chris
 */

#ifndef STATUSLED_H_
#define STATUSLED_H_

class StatusLED
{
public:
	StatusLED(int pin);
	void init(void);
	void on(void);
	void off(void);
	void toggle(void);
private:
	bool flag;
	int pin;
};

#endif /* STATUSLED_H_ */
