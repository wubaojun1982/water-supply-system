/*
 * Command.cpp
 *
 *  Created on: 2015年7月31日
 *      Author: Chris
 */
#include "Command.h"
#include "CommandHandler.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"

//
// 命令行最大个数
//
static const int MAX_CMDLINE_COUNT = 6;

//
// 命令名称最大长度
//
static const int MAX_CMD_LEN = 12;

//
// 参数最大长度
//
static const int MAX_ARG_LEN = 24;

//
// 命令列表实例
//
static CmdLineTable cmdLineTable[MAX_CMDLINE_COUNT];

static ErrorType queryCmd(const char* cmd, int* matchingIndex);
static void parseCmdLine(const char* cmdLine, char* cmd, char* args);

//
// 构建命令列表
//
void buildCmdLineTable(void)
{
	// 自动模式
	cmdLineTable[0].cmd = "auto";
	cmdLineTable[0].handlerFunc = autoModeCommandHandler;

	// 手动模式
	cmdLineTable[1].cmd = "manual";
	cmdLineTable[1].handlerFunc = manualModeCommandHandler;

	// 供水
	cmdLineTable[2].cmd = "supply";
	cmdLineTable[2].handlerFunc = supplyCommandHandler;

	// 停止供水
	cmdLineTable[3].cmd = "stop";
	cmdLineTable[3].handlerFunc = stopCommandHandler;

	// 校准时间
	cmdLineTable[4].cmd = "cali";
	cmdLineTable[4].handlerFunc = caliCommandHandler;

	// 设置定时
	cmdLineTable[5].cmd = "clock";
	cmdLineTable[5].handlerFunc = clockCommandHandler;
}

//
// 执行命令行
//
void executeCmdLine(const char* cmdLine)
{
	assert(cmdLine != NULL);

	char cmd[MAX_CMD_LEN];
	char args[MAX_ARG_LEN];

	parseCmdLine(cmdLine, cmd, args);

	int index = -1;
	ErrorType err = queryCmd(cmd, &index);
	switch(err)
	{
	case NoErr:
		cmdLineTable[index].handlerFunc(args);
		break;
	case NoMatchingCmd:
		break;
	}
	return;
}

//
// 查询命令名称是否在命令列表中。如果存在，设置匹配的命令索引值
//
static ErrorType queryCmd(const char* cmd, int* matchingIndex)
{
	assert(cmd != NULL && matchingIndex != NULL);

	for (int i = 0; i < MAX_CMDLINE_COUNT; i++)
	{
		if (strcmp(cmdLineTable[i].cmd, cmd) == 0)
		{
			*matchingIndex = i;
			return NoErr;
		}
	}
	*matchingIndex = -1;
	return NoMatchingCmd;
}

//
// 解析命令行，将命令名称和参数字符串分割开来
//
static void parseCmdLine(const char* cmdLine, char* cmd, char* args)
{
	assert(cmdLine != NULL && cmd != NULL && args != NULL);

	//
	// 提取的办法：首先查找有没有空格存在，如果存在空格，则表示命令含有参数；否则没有参数。
	// 没有空格时，则将命令直接给cmd即可。有空格时，获取第一个空格的位置，把第一个空格之前的
	// 字符拷贝给cmd，其他给args即可实现分离。
	//
	char* firstSpacePos = strchr(cmdLine, ' ');
	const char* cursor = cmdLine;

	if (firstSpacePos != NULL)
	{
		while (*cursor != '\0')
		{
			if (cursor < firstSpacePos)
			{
				*cmd++ = *cursor++;
			}
			else if (cursor == firstSpacePos)
			{
				*cmd = '\0';
				cursor++;
			}
			else
			{
				*args++ = *cursor++;
			}
		}
		*args = '\0';
	}
	else
	{
		strcpy(cmd, cmdLine);
	}
}

