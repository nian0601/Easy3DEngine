#include "stdafx.h"
#include "Debug_BlockTimer.h"
#include "DebugDataDisplay.h"


Prism::Debug_BlockTimer::Debug_BlockTimer(const char* aName)
	: myName(aName)
{
	Engine::GetInstance()->GetDebugDisplay()->StartFunctionTimer(myName);
}


Prism::Debug_BlockTimer::~Debug_BlockTimer()
{
	Engine::GetInstance()->GetDebugDisplay()->EndFunctionTimer(myName);
}
