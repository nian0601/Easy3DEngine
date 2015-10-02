#pragma once

#include "Time.h"

namespace CommonUtilities
{
	class Timer
	{
	public:
		Timer();
		~Timer();
		void AddTime(TimeUnit aMicroSeconds);

		Time GetTime() const;
		void Pause();
		void Start();
	private:
		bool myPaused;
		TimeUnit myTotalTime;
		TimeUnit myFrameTime;
	};
}

inline CommonUtilities::Time CommonUtilities::Timer::GetTime() const
{
	return CommonUtilities::Time(myTotalTime, myFrameTime);
}

inline void CommonUtilities::Timer::Pause()
{
	myPaused = true;
}

inline void CommonUtilities::Timer::Start()
{
	myPaused = false;
}