/*
 * PushButton.cpp
 *
 *  Created on: 2015年8月9日
 *      Author: Chris
 */
#include "PushButton.h"
#include <Arduino.h>
#include "../Board/Board.h"

#define PUSH_BUTTON_MAX_COUNT	3

//
// 按键按下时处理函数指针
//
static handlerFunc pushButtonPressedEventHandler = NULL;
static handlerFunc pushButtonReleasedEventHandler = NULL;
static KeyCodeTable keyCodeTable[PUSH_BUTTON_MAX_COUNT + 1] =
{
		{ SWITCH_MODE_PUSH_BUTTON_PIN, 0 },
		{ SUPPLY_WATER_PUSH_BUTTON_PIN, 1 },
		{ STOP_SUPPLYING_PUSH_BUTTON_PIN, 2 },
		{ -1, -1 }	// End of the keyCodeTable
};

//
// 触发事件
//
static void raisePushButtonPressedEvent(int keyCode);
static void raisePushButtonReleasedEvent(int keyCode);

//
// 管脚初始化
// 使用内部上拉电阻
//
void pushButtonInitAll(void)
{
	for (int i = 0; i < PUSH_BUTTON_MAX_COUNT + 1; i++)
	{
		if (keyCodeTable[i].pushButtonPin != -1)
		{
			pinMode(keyCodeTable[i].pushButtonPin, INPUT_PULLUP);
		}
	}
}

//
// 按键检测
// 放于定时循环中，建议定时周期为10ms以上
//
int pushButtonDetect(void)
{
	// 假设每次最多只能响应一个按键按下的事件
	// 当某个按键按下后，需要等待10ms以上消抖
	// 最后确定按键是否真的是按下了
	// 一次完整的按键过程是： 按下--过度--抬起
	static bool isPressed = false;
	static int pushButtonPin = -1;
	static int keyCode = -1;

	if (pushButtonPin == -1)
	{
		for (int i = 0; i < PUSH_BUTTON_MAX_COUNT; i++)
		{
			if (keyCodeTable[i].keyCode != -1)
			{
				if (digitalRead(keyCodeTable[i].pushButtonPin) == 0)
				{
					pushButtonPin = keyCodeTable[i].pushButtonPin;
					keyCode = keyCodeTable[i].keyCode;
					return -1;
				}
			}
		}
		keyCode = -1;
		pushButtonPin = -1;
	}
	else
	{
		// 15ms定时到达，此时完成消抖过程
		// 再次检测被记录的键是否仍然处于低电平状态，如果是，则确认是按下了
		// 此时再响应即可
		if (!isPressed)
		{
			if (digitalRead(pushButtonPin) == 0)
			{
				isPressed = true;
				raisePushButtonPressedEvent(keyCode);
				return keyCode;
			}
		}
		else
		{
			if (digitalRead(pushButtonPin) == 1)
			{
				isPressed = false;
				raisePushButtonReleasedEvent(keyCode);
				pushButtonPin = -1;
				keyCode = -1;
			}
		}
	}

	return -1;
}

void registerPushButtonPressedEventHandler(handlerFunc handler)
{
	if (handler != NULL)
	{
		pushButtonPressedEventHandler = handler;
	}
}

void raisePushButtonPressedEvent(int keyCode)
{
	if (pushButtonPressedEventHandler != NULL)
	{
		pushButtonPressedEventHandler(keyCode);
	}
}

void registerPushButtonReleasedEventHandler(handlerFunc handler)
{
	if (handler != NULL)
	{
		pushButtonReleasedEventHandler = handler;
	}
}

void raisePushButtonReleasedEvent(int keyCode)
{
	if (pushButtonReleasedEventHandler != NULL)
	{
		pushButtonReleasedEventHandler(keyCode);
	}
}
