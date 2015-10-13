#include "stdafx.h"

#include <assert.h>
#include "Time.h"
#include "TimerManager.h"
#include <Windows.h>

CommonUtilities::TimerManager* CommonUtilities::TimerManager::myInstance = nullptr;

void CommonUtilities::TimerManager::Create()
{
	myInstance = new TimerManager();
}

void CommonUtilities::TimerManager::Destroy()
{
	delete myInstance;
}

CommonUtilities::TimerManager* CommonUtilities::TimerManager::GetInstance()
{
	return myInstance;
}

CommonUtilities::TimerManager::TimerManager()
{
	LARGE_INTEGER largeInteger;
	QueryPerformanceFrequency(&largeInteger);
	myFrequency = largeInteger.QuadPart;

	QueryPerformanceCounter(&largeInteger);
	myLastTime = largeInteger.QuadPart * 1000000 / myFrequency;
}


CommonUtilities::TimerManager::~TimerManager()
{
}

void CommonUtilities::TimerManager::Update()
{
	TimeUnit time = GetTime();

	myMasterTimer.AddTime(time);

	for (unsigned int i = 0; i < myTimerList.size(); ++i)
	{
		myTimerList[i].AddTime(time);
	}


	myLastTime += time;
}

void CommonUtilities::TimerManager::StartTimer(const std::string& aName)
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	myTimers[aName] = current.QuadPart * 1000000 / myFrequency;
}

CommonUtilities::Time CommonUtilities::TimerManager::StopTimer(const std::string& aName)
{
	if (myTimers.find(aName) == myTimers.end())
		return Time(0, 0);

	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	TimeUnit resultTime = (current.QuadPart * 1000000 / myFrequency) - myTimers[aName];

	return Time(resultTime, 0);
}

const CommonUtilities::Timer& CommonUtilities::TimerManager::GetMasterTimer() const
{
	return myMasterTimer;
}

CommonUtilities::TimeUnit CommonUtilities::TimerManager::GetTime()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	return (current.QuadPart * 1000000 / myFrequency) - myLastTime;
}

CommonUtilities::TimerHandle CommonUtilities::TimerManager::CreateTimer()
{
	myTimerList.push_back(Timer());
	return myTimerList.size() - 1;
}


const CommonUtilities::Timer& CommonUtilities::TimerManager::GetTimer(CommonUtilities::TimerHandle aId) const
{
	assert(aId < myTimerList.size() && "GetTimer handle out of bounds.");
	return myTimerList[aId];
}


void CommonUtilities::TimerManager::PauseAll()
{
	for (unsigned int i = 0; i < myTimerList.size(); ++i)
	{
		myTimerList[i].Pause();
	}
}

void CommonUtilities::TimerManager::StartAll()
{
	for (unsigned int i = 0; i < myTimerList.size(); ++i)
	{
		myTimerList[i].Start();
	}
}