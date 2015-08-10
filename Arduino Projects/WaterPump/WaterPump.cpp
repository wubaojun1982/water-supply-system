/*
 * WaterPump.cpp
 *
 *  Created on: 2015年8月6日
 *      Author: Chris
 */
#include <Arduino.h>
#include "WaterPump.h"

void WaterPump::init(const int ctrlPin)
{
	state = false;	// 该状态记水泵是否工作：false表示没有工作。
	pin = ctrlPin;
	pinMode(ctrlPin, OUTPUT);
	digitalWrite(pin, HIGH);
}

void WaterPump::start(void)
{
	if (!state)
	{
		digitalWrite(pin, LOW);
		state = true;
	}
}

void WaterPump::stop(void)
{
	if (state)
	{
		digitalWrite(pin, HIGH);
		state = false;
	}
}
