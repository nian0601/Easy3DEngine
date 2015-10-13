#pragma once
#define TIME_FUNCTION Easy3D::Debug_BlockTimer funcTimer__(__FUNCTION__)
#define BEGIN_TIME_BLOCK(NAME) Easy3D::Engine::GetInstance()->GetDebugDisplay()->StartFunctionTimer(NAME)
#define END_TIME_BLOCK(NAME) Easy3D::Engine::GetInstance()->GetDebugDisplay()->EndFunctionTimer(NAME)


#include <bitset>
#include "Debug_BlockTimer.h"
#include "GraphRenderer.h"
#include <sstream>
#include <unordered_map>

namespace CommonUtilities
{
	class InputWrapper;
}

namespace Easy3D
{
	class Camera;
	class RadialDebugMenu;
	class Font;
	class FrameTimeDebugger;
	class Text;


	class DebugDataDisplay
	{
	public:
		DebugDataDisplay();
		~DebugDataDisplay();

		void Init();

		void StartFunctionTimer(const std::string& aFunc);
		void EndFunctionTimer(const std::string& aFunc);

		void RecordFrameTime(const float aDeltaTime);
		void Render();

		void Update(CU::InputWrapper& aInputWrapper);

		RadialDebugMenu* GetDebugMenu();

	private:
		enum eBitSetEnum
		{
			FUNCTION_TIMERS,
			MEMORY_USAGE,
			CPU_USAGE,
			FRAME_TIME,
			NEW_GRAPH_DATA,
			count
		};

		void ToggleFunctionTimers();
		void ToggleMemoryUsage();
		void ToggleCPUUsage();
		void ToggleFrameTime();

		void RenderFunctionTimers();
		void RenderMemoryUsage();
		void RenderCPUUsage();
		void RenderFrameTime();

		Text* myText;

		std::bitset<eBitSetEnum::count> myBoolContainer;

		CU::Vector2<float> myFunctionTimersStartPos;
		CU::Vector2<float> myMemUsageStartPos;
		CU::Vector2<float> myCPUUSageStartPos;
		CU::Vector2<float> myFrameTimeStartPos;
		float myTextScale;
		float myLastDeltaTime;
		std::stringstream myStringStream;

		FrameTimeDebugger* myFrameDebugger;
		RadialDebugMenu* myDebugMenu;
	};
}

inline void Easy3D::DebugDataDisplay::ToggleFunctionTimers()
{
	myBoolContainer[eBitSetEnum::FUNCTION_TIMERS].flip();
}

inline void Easy3D::DebugDataDisplay::ToggleMemoryUsage()
{
	myBoolContainer[eBitSetEnum::MEMORY_USAGE].flip();
}

inline void Easy3D::DebugDataDisplay::ToggleCPUUsage()
{
	myBoolContainer[eBitSetEnum::CPU_USAGE].flip();
}

inline void Easy3D::DebugDataDisplay::ToggleFrameTime()
{
	myBoolContainer[eBitSetEnum::FRAME_TIME].flip();
}

inline Easy3D::RadialDebugMenu* Easy3D::DebugDataDisplay::GetDebugMenu()
{
	return myDebugMenu;
}