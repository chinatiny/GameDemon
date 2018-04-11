#pragma once
#include "LibCommon.h"

class IAstartCheck
{
public:
	virtual bool AStartCheck(unsigned int uTargetID, GamePos &preGamePos, GamePos &nowGamePos) = 0;
};

