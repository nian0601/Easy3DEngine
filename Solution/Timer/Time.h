#pragma once

#include <cassert>

namespace CommonUtilities
{
	typedef unsigned long long TimeUnit;

	class Time
	{
	public:
		Time(TimeUnit aMicroSeconds, TimeUnit aFrameMicroSeconds);
		~Time();
		
		TimeUnit GetHours() const;
		TimeUnit GetMinutes() const;
		TimeUnit GetSeconds() const;
		TimeUnit GetMilliseconds() const;
		TimeUnit GetMicroseconds() const;
		TimeUnit GetFPS() const;
		float GetFrameTime() const;
		static void DoSrand();
	private:
		TimeUnit myMicroSeconds;
		TimeUnit myFrameMicroSeconds;
	};
}

inline CommonUtilities::TimeUnit CommonUtilities::Time::GetHours() const
{
	return GetMinutes() / 60;
}

inline CommonUtilities::TimeUnit CommonUtilities::Time::GetMinutes() const
{
	return myMicroSeconds / 60000000;
}

inline CommonUtilities::TimeUnit CommonUtilities::Time::GetSeconds() const
{
	return myMicroSeconds / 1000000;
}

inline CommonUtilities::TimeUnit CommonUtilities::Time::GetMilliseconds() const
{
	return myMicroSeconds / 1000;
}

inline CommonUtilities::TimeUnit CommonUtilities::Time::GetMicroseconds() const
{
	return myMicroSeconds;
}

inline CommonUtilities::TimeUnit CommonUtilities::Time::GetFPS() const
{
	assert(myFrameMicroSeconds > 0 && "GetFPS div by 0, can't check FPS on paused timer.");
	return 1000000 / myFrameMicroSeconds;
}

inline float CommonUtilities::Time::GetFrameTime() const
{
	return myFrameMicroSeconds / 1000000.f;
}
