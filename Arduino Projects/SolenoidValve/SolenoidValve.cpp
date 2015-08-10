/*
 * SolenoidValve.cpp
 *
 *  Created on: 2015年8月6日
 *      Author: Chris
 */
#include <Arduino.h>
#include "SolenoidValve.h"

void SolenoidValve::init(const int ctrlPin)
{
	state = false;	// 该状态记录电磁阀是否工作：false表示没有工作。
	pin = ctrlPin;
	pinMode(ctrlPin, OUTPUT);
	digitalWrite(pin, HIGH);
}

void SolenoidValve::start(void)
{
	if (!state)
	{
		digitalWrite(pin, LOW);
		state = true;
	}
}

void SolenoidValve::stop(void)
{
	if (state)
	{
		digitalWrite(pin, HIGH);
		state = false;
	}
}



