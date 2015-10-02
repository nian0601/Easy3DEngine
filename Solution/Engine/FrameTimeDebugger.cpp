#include "stdafx.h"

#include "FrameTimeDebugger.h"
#include "FontContainer.h"
#include "GraphRenderer.h"
#include <InputWrapper.h>
#include "Text.h"

namespace Easy3D
{
	FrameTimeDebugger::FrameTimeDebugger()
		: myCurrentFrameIndex(0)
		, myFocusedFrameIndex(-1)
		, myGraphSize(1000.f, 200.f)
		, myGraphTopLeft( 200.f, -500.f )
		, myLastDeltaTime(0.f)
		, mySampleTimer(0.f)
		, myTimeBetweenSamples(0.05f)
		, myFrameTimeIndex(0)
		, myFrameCounter(0)
	{
		for (int i = 0; i < NUM_FRAMES_TO_RECORD; ++i)
		{
			for (auto it = myFrameDatas[i].begin(); it != myFrameDatas[i].end(); ++it)
			{
				it->second.myNameText = nullptr;
				it->second.myTimeText = nullptr;
			}
		}
	}

	FrameTimeDebugger::~FrameTimeDebugger()
	{
		for (int i = 0; i < NUM_FRAMES_TO_RECORD; ++i)
		{
			for (auto it = myFrameDatas[i].begin(); it != myFrameDatas[i].end(); ++it)
			{
				delete it->second.myNameText;
				delete it->second.myTimeText;
			}

			myFrameDatas[i].clear();
		}

		delete myGraphRenderer;
	}

	void FrameTimeDebugger::Init()
	{
		LARGE_INTEGER largeInteger;
		QueryPerformanceFrequency(&largeInteger);
		myCPUFrequency = largeInteger.QuadPart;

		myGraphRenderer = new GraphRenderer();
		myGraphRenderer->Init();

		myFrameTimes.Init(NUM_FRAMES_TO_RECORD);
	}

	void FrameTimeDebugger::StartFunctionTimer(const std::string& aFunc)
	{
		if (myFocusedFrameIndex != -1)
		{
			return;
		}

		FunctionData& it = myFrameDatas[myCurrentFrameIndex][aFunc];

		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		it.myStart = time.QuadPart;
	}

	void FrameTimeDebugger::EndFunctionTimer(const std::string& aFunc)
	{
		if (myFocusedFrameIndex != -1)
		{
			return;
		}

		auto it = myFrameDatas[myCurrentFrameIndex].find(aFunc);
		
		if (it != myFrameDatas[myCurrentFrameIndex].end())
		{
			if (it->second.myNameText == nullptr)
			{
				myStringStream.clear();
				myStringStream.str(std::string());
				myStringStream << aFunc << ":" << std::endl;
				it->second.myNameString = myStringStream.str();
		
		
				it->second.myNameText = new Text();
				it->second.myNameText->Init(
					Engine::GetInstance()->GetFontContainer()->GetFont("Data/resources/font/font.dds"));
		
				it->second.myTimeText = new Text();
				it->second.myTimeText->Init(
					Engine::GetInstance()->GetFontContainer()->GetFont("Data/resources/font/font.dds"));
			}
		
			LARGE_INTEGER time;
			QueryPerformanceCounter(&time);
		
			++it->second.myHitCount;
			it->second.myEnabled = true;
		
			float ms = static_cast<float>(
				((time.QuadPart * 1000000 / myCPUFrequency) - (it->second.myStart * 1000000 / myCPUFrequency)) / 1000.f);
			it->second.myMS += ms;
		}
	}

	void FrameTimeDebugger::EndFrame(float aDeltaTime)
	{
		if (myFocusedFrameIndex != -1)
		{
			return;
		}

		myNewGraphData = false;

		myLastDeltaTime = aDeltaTime;
		mySampleTimer += aDeltaTime;
		++myFrameCounter;

		if (mySampleTimer > myTimeBetweenSamples)
		{
			FrameData& oldData = myFrameDatas[myCurrentFrameIndex];

			for (auto it = oldData.begin(); it != oldData.end(); ++it)
			{
				it->second.myHitCount /= myFrameCounter;
				it->second.myMS /= myFrameCounter;
			}

			++myCurrentFrameIndex;

			if (myCurrentFrameIndex >= NUM_FRAMES_TO_RECORD)
			{
				myCurrentFrameIndex = 0;
			}

			FrameData& newData = myFrameDatas[myCurrentFrameIndex];

			for (auto it = newData.begin(); it != newData.end(); ++it)
			{
				it->second.myHitCount = 0;
				it->second.myMS = 0;
			}

			RecordFrameTime();
		}

	}

	void FrameTimeDebugger::Update(const CU::InputWrapper& aInputWrapper)
	{
		CU::Vector2<float> mousePos = aInputWrapper.GetMousePosition();
		mousePos.y = -mousePos.y;

		if (mousePos.x > myGraphTopLeft.x && //Left
			mousePos.y < myGraphTopLeft.y && //Top
			mousePos.x < myGraphTopLeft.x + myGraphSize.x && //Right
			mousePos.y > myGraphTopLeft.y - myGraphSize.y) //Bottom
		{
			float widthPerColum = static_cast<int>((myGraphSize.x) / myFrameTimes.Size()) + 0.75f;


			float lastX = myGraphTopLeft.x;
			float currX = myGraphTopLeft.x + widthPerColum;
			int index = 0;
			bool done = false;
			while (done == false)
			{
				if (lastX < mousePos.x && currX >= mousePos.x)
				{
					done = true;
				}
				else
				{
					lastX = currX;
					currX += widthPerColum;
					++index;
				}
			}

			myFocusedFrameIndex = index;
		}
		else
		{
			myFocusedFrameIndex = -1;
		}
	}

	void FrameTimeDebugger::Render()
	{
		TIME_FUNCTION

		myGraphRenderer->Render(myFrameTimes, myGraphTopLeft, 
		{ myGraphSize.x, myGraphSize.y }, 16.f, myNewGraphData);


		if (myFocusedFrameIndex == -1)
		{
			return;
		}


		CU::Vector2<float> drawPos = { 0.f, 0.f };

		FrameData& frameData = myFrameDatas[myFocusedFrameIndex];

		for (auto it = frameData.begin(); it != frameData.end(); ++it)
		{
			if (it->second.myEnabled == true)
			{
				CU::Vector4<float> timeColor;
				if (it->second.myMS < 1.f)
				{
					timeColor = { 0.f, 1.f, 0.f, 1.f };
				}
				else if (it->second.myMS < 5.f)
				{
					timeColor = { 1.f, 0.39f, 0.28f, 1.f };
				}
				else
				{
					timeColor = { 1.f, 0.f, 0.f, 1.f };
				}


				myStringStream.clear();
				myStringStream.str(std::string());
				myStringStream << it->second.myMS << " ms";
				if (it->second.myHitCount > 1)
					myStringStream << " (" << it->second.myHitCount << " calls)";
		
				myStringStream << std::endl;
		
				it->second.myNameText->Render(it->second.myNameString.c_str(), drawPos.x, drawPos.y, { 1.f, 1.f, 1.f, 1.f }, 0.75f);
		
				it->second.myTimeText->Render(myStringStream.str().c_str()
					, drawPos.x + it->second.myNameText->GetTextWidth() + 10.f, drawPos.y, timeColor, 0.75f);
				drawPos.y -= 30.f;
			}
		}
	}

	void FrameTimeDebugger::RecordFrameTime()
	{
		myNewGraphData = true;

		float frameTime = mySampleTimer / myFrameCounter;
		mySampleTimer = 0.f;
		myFrameCounter = 0;

		if (myFrameTimes.Size() < NUM_FRAMES_TO_RECORD)
		{
			myFrameTimes.Add(frameTime * 1000.f);
			myFrameTimeIndex;

		}
		else
		{
			if (myFrameTimeIndex >= myFrameTimes.Size())
				myFrameTimeIndex = 0;

			myFrameTimes[myFrameTimeIndex] = frameTime * 1000.f;
			++myFrameTimeIndex;
		}
	}

}