#include <Arduino.h>
#include "TaskScheduler/TaskScheduler.h"
#include "Core/Core.h"
#include "PushButton/PushButton.h"

unsigned long tick = 0;

////////////////////////////////////////////////////////////
// 函数声明
////////////////////////////////////////////////////////////
static void pushButtonDetectTask(void);
static void autoWaterSupplyTask(void);
static void isWaterFullCheckTask(void);
static void waterSupplyStatusLEDToggleTask(void);

///////////////////////////////////////////////////////////
// 函数实现
///////////////////////////////////////////////////////////
void setup()
{
	Serial.begin(38400);
	initCore();

	Sch.init();
	Sch.addTask(pushButtonDetectTask, 0, 15, 1);
	Sch.addTask(waterSupplyStatusLEDToggleTask, 5, 150, 1);
	// Sch.addTask(autoWaterSupplyTask, 15, 2000, 0);
	Sch.addTask(isWaterFullCheckTask, 20, 1000, 1);
	Sch.start();

	tick = millis();
}

void loop()
{
	if (Serial.available())
	{
		commandLineProcess(Serial.readString().c_str());
	}
	else
	{
		Sch.dispatchTasks();
		if (millis() - tick >= 1000)
		{
			tick = millis();
			autoWaterSupplyTask();
		}
	}
}

static void pushButtonDetectTask(void)
{
	pushButtonDetect();
}

static void autoWaterSupplyTask(void)
{
	if (getOperatingMode() == Auto)
	{
		bool retFlag = isTimeUp();
		if (retFlag)
		{
			supplyWater();
		}
	}
}

static void isWaterFullCheckTask(void)
{
	if (isSupplyingWater())
	{
		bool retFlag = isWaterFull();
		if (retFlag)
		{
			stopSupplyingWater();
		}
	}
}

static void waterSupplyStatusLEDToggleTask(void)
{
	if (isSupplyingWater())
	{
		waterSupplyStatusLEDToggle();
	}
}
