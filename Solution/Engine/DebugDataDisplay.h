#pragma once
#define TIME_FUNCTION Prism::Debug_BlockTimer funcTimer__(__FUNCTION__);
#define BEGIN_TIME_BLOCK(NAME) Prism::Engine::GetInstance()->GetDebugDisplay()->StartFunctionTimer(NAME);
#define END_TIME_BLOCK(NAME) Prism::Engine::GetInstance()->GetDebugDisplay()->EndFunctionTimer(NAME);


#include <bitset>
#include "Debug_BlockTimer.h"
#include "GraphRenderer.h"
#include <sstream>
#include <unordered_map>

namespace CommonUtilities
{
	class InputWrapper;
}

namespace Prism
{
	class Camera;
	class DebugMenu;
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

		void ToggleFunctionTimers();
		void ToggleMemoryUsage();
		void ToggleCPUUsage();
		void ToggleFrameTime();

		void Update(CU::InputWrapper& aInputWrapper);

		DebugMenu* GetDebugMenu();

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
		DebugMenu* myDebugMenu;
	};
}

inline void Prism::DebugDataDisplay::ToggleFunctionTimers()
{
	myBoolContainer[eBitSetEnum::FUNCTION_TIMERS].flip();
}

inline void Prism::DebugDataDisplay::ToggleMemoryUsage()
{
	myBoolContainer[eBitSetEnum::MEMORY_USAGE].flip();
}

inline void Prism::DebugDataDisplay::ToggleCPUUsage()
{
	myBoolContainer[eBitSetEnum::CPU_USAGE].flip();
}

inline void Prism::DebugDataDisplay::ToggleFrameTime()
{
	myBoolContainer[eBitSetEnum::FRAME_TIME].flip();
}

inline Prism::DebugMenu* Prism::DebugDataDisplay::GetDebugMenu()
{
	return myDebugMenu;
}