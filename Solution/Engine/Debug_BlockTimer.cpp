#include "stdafx.h"
#include "Debug_BlockTimer.h"
#include "DebugDataDisplay.h"


Easy3D::Debug_BlockTimer::Debug_BlockTimer(const char* aName)
	: myName(aName)
{
	Engine::GetInstance()->GetDebugDisplay()->StartFunctionTimer(myName);
}


Easy3D::Debug_BlockTimer::~Debug_BlockTimer()
{
	Engine::GetInstance()->GetDebugDisplay()->EndFunctionTimer(myName);
}
