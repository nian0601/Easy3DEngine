#pragma once

namespace std
{
	class thread;
}

namespace Easy3D
{
	class Thread
	{
	public:
		Thread();

		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;
		virtual void UnPause() = 0;
		
		volatile bool IsRunning() const;
		volatile bool IsPaused() const;

	protected:
		std::thread* mySTDThread;
		volatile bool myIsRunning;
		volatile bool myIsPaused;
	};
}
