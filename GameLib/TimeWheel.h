#pragma once

#include <time.h>
#include <list>

class TimeWheel
{
private:
	TimeWheel(){}
public:
	typedef void(*fpRegFuncType)();
	static void RegRunFunc(fpRegFuncType lpFunc,int nIntervalTickets);
	static clock_t GetRunTickets();
	static void Loop();
	static void ShutDown();
	static void Chase(); //把回调模块的下次执行执行修改为当前时间

private:
	static bool s_isLoop;
	static clock_t s_clockTicket;
	static clock_t s_clockInterval;
	typedef struct  
	{
		fpRegFuncType fpRegFunc;
		int nIntervalTickets;
		clock_t  nextRunTickets;
	}RegCallData;
	static std::list<RegCallData> s_RegisteredFuncs;
};
