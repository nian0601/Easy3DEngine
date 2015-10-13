#include "stdafx.h"

#include "Timer.h"

CommonUtilities::Timer::Timer()
{
	myTotalTime = 0;
	myFrameTime = 0;
	myPaused = false;
}

CommonUtilities::Timer::~Timer()
{
}

void CommonUtilities::Timer::AddTime(TimeUnit aMicroSeconds)
{
	if (myPaused == false)
	{
		myTotalTime += aMicroSeconds;
		myFrameTime = aMicroSeconds;
	}
	else
	{
		myFrameTime = 0;
	}
}