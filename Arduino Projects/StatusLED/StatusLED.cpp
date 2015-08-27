/*
 * StatusLED.cpp
 *
 *  Created on: 2015Äê8ÔÂ5ÈÕ
 *      Author: Chris
 */
#include "StatusLED.h"
#include <Arduino.h>

StatusLED::StatusLED(int pin)
{
	this->pin = pin;
	this->flag = false;
	return;
}

void StatusLED::init(void)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

void StatusLED::on(void)
{
	if (this->flag == false)
	{
		digitalWrite(pin, HIGH);
		flag = true;
	}

	return;
}

void StatusLED::off(void)
{
	if (this->flag == true)
	{
		digitalWrite(pin, LOW);
		flag = false;
	}

	return;
}

void StatusLED::toggle(void)
{
	if (this->flag == true)
	{
		digitalWrite(pin, LOW);
	}
	else
	{
		digitalWrite(pin, HIGH);
	}

	flag = !flag;

	return;
}
