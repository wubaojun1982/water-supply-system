/*
 * Core.h
 *
 *  Created on: 2015年8月6日
 *      Author: Chris
 */

#ifndef CORE_CORE_H_
#define CORE_CORE_H_

/////////////////////////////////////////////////////////////
// 供水操作模式
/////////////////////////////////////////////////////////////
typedef enum
{
	Auto,
	Manual
} OperatingMode;

extern void initCore(void);
extern void initRtc(void);
extern void commandLineProcess(const char* commandLine);
extern void setClock(const char* time);
extern void calibrateTime(const char* longTime);
extern bool isTimeUp(void);
extern void supplyWater(void);
extern bool isWaterFull(void);
extern void stopSupplyingWater(void);
extern void switchToManualMode(void);
extern void switchToAutoMode(void);
extern void waterSupplyStatusLEDToggle(void);
extern OperatingMode getOperatingMode(void);
extern bool isSupplyingWater(void);
extern void pushButtonReleased(int keyCode);

#endif /* CORE_CORE_H_ */
