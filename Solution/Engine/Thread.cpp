#include "stdafx.h"
#include "Thread.h"


namespace Easy3D
{
	Thread::Thread()
		: mySTDThread(nullptr)
		, myIsPaused(false)
		, myIsRunning(false)
	{
	}


	volatile bool Thread::IsRunning() const
	{
		return myIsRunning;
	}

	volatile bool Thread::IsPaused() const
	{
		return myIsPaused;
	}
}