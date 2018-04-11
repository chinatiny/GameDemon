#include "TimeWheel.h"

clock_t TimeWheel::s_clockTicket;
std::list<TimeWheel::RegCallData> TimeWheel::s_RegisteredFuncs;
bool TimeWheel::s_isLoop = true;

void TimeWheel::Loop()
{
	while (s_isLoop)
	{
		s_clockTicket = clock();
		//·Ö·¢
		std::list<RegCallData>::iterator begin = s_RegisteredFuncs.begin();
		std::list<RegCallData>::iterator end = s_RegisteredFuncs.end();
		for (; begin != end; begin++)
		{
			if (s_clockTicket >= (begin->nextRunTickets))
			{
				(begin->fpRegFunc)();
				(begin->nextRunTickets) += (begin->nIntervalTickets);
			}
		}
	}
}

void TimeWheel::ShutDown()
{
	s_isLoop = false;
}


void TimeWheel::RegRunFunc(fpRegFuncType lpFunc, int nIntervalMs)
{
	RegCallData regData;
	regData.fpRegFunc = lpFunc;
	regData.nIntervalTickets = nIntervalMs;
	regData.nextRunTickets = s_clockTicket + nIntervalMs;
	s_RegisteredFuncs.push_back(regData);
}

clock_t TimeWheel::GetRunTickets()
{
	return s_clockTicket;
}

void TimeWheel::Chase()
{
	s_clockTicket = clock();
	std::list<RegCallData>::iterator begin = s_RegisteredFuncs.begin();
	std::list<RegCallData>::iterator end = s_RegisteredFuncs.end();
	for (; begin != end; begin++)
	{
		begin->nextRunTickets = s_clockTicket + begin->nIntervalTickets;
	}
}
