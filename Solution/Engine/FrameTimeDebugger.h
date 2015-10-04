#pragma once

#include <StaticArray.h>
#include <sstream>
#include <unordered_map>

#define NUM_FRAMES_TO_RECORD 120

namespace CommonUtilities
{
	class InputWrapper;
}

namespace Easy3D
{
	class Camera;
	class GraphRenderer;
	class Text;

	class FrameTimeDebugger
	{
	public:
		FrameTimeDebugger();
		~FrameTimeDebugger();

		void Init();

		void StartFunctionTimer(const std::string& aFunc);
		void EndFunctionTimer(const std::string& aFunc);

		void EndFrame(float aDeltaTime);

		void Update(const CU::InputWrapper& aInputWrapper);
		void Render();

		void SetFocusedFrameIndex(int aFrameIndex);

	private:
		void RecordFrameTime();

		struct FunctionData
		{
			Text* myNameText = nullptr;
			Text* myTimeText = nullptr;
			int myHitCount = 0;
			float myMS = 0.f;
			unsigned long long myStart = 0;
			bool myEnabled = false;
			std::string myNameString;
		};

		typedef std::unordered_map<std::string, FunctionData> FrameData;


		CU::StaticArray<FrameData, NUM_FRAMES_TO_RECORD> myFrameDatas;
		int myCurrentFrameIndex;
		int myFocusedFrameIndex;
		unsigned long long myCPUFrequency;

		std::stringstream myStringStream;


		CU::GrowingArray<float> myFrameTimes;
		float mySampleTimer;
		float myTimeBetweenSamples;
		int myFrameTimeIndex;
		int myFrameCounter;
		bool myNewGraphData;

		GraphRenderer* myGraphRenderer;

		CU::Vector2<float> myGraphTopLeft;
		CU::Vector2<float> myGraphSize;
	};

	inline void FrameTimeDebugger::SetFocusedFrameIndex(int aFrameIndex)
	{
		myFocusedFrameIndex = aFrameIndex;

		if (myFocusedFrameIndex > NUM_FRAMES_TO_RECORD)
		{
			myFocusedFrameIndex = NUM_FRAMES_TO_RECORD;
		}

		if (myFocusedFrameIndex < -1)
		{
			myFocusedFrameIndex = -1;
		}
	}
}