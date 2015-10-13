#include "stdafx.h"

#include <stdlib.h>
#include "Time.h"
#include <time.h>

CommonUtilities::Time::Time(CommonUtilities::TimeUnit aMicroSeconds, CommonUtilities::TimeUnit aFrameMicroSeconds)
{
	myMicroSeconds = aMicroSeconds;
	myFrameMicroSeconds = aFrameMicroSeconds;
}

CommonUtilities::Time::~Time()
{
}

void CommonUtilities::Time::DoSrand()
{
	srand(static_cast<unsigned int>(time(0)));
}