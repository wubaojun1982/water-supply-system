/*
 * CommandHandler.cpp
 *
 *  Created on: 2015年7月31日
 *      Author: Chris
 */
#include "CommandHandler.h"
#include "../Core/Core.h"
#include "assert.h"
#include "string.h"

void autoModeCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute auto command.");
	switchToAutoMode();
}

void manualModeCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute manual command.");
	switchToManualMode();
}

void supplyCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute supply command.");
	supplyWater();
}

void stopCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute stop command.");
	stopSupplyingWater();
}

void caliCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute cali command.");
	// DBGLN(args);
	// 解析得到年-月-日-时-分-秒
	calibrateTime(args);
}

void clockCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute clock command.");
	// DBGLN(args);
	setClock(args);
}

