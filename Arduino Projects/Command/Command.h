/*
 * Command.h
 *
 *  Created on: 2015年7月31日
 *      Author: Chris
 */

#ifndef COMMAND_H_
#define COMMAND_H_

//
// 错误类型
//
typedef enum
{
	NoErr,
	NoMatchingCmd,
}ErrorType;

//
// 命令行列表
//
class CmdLineTable
{
public:
	const char* cmd;	// 命令名称
	void (*handlerFunc)(const char* args);	// 命令对应的执行函数
};

extern void buildCmdLineTable(void);

extern void executeCmdLine(const char* cmdLine);

#endif /* COMMAND_H_ */
