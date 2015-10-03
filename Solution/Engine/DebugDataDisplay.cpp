#include "stdafx.h"

#include "DebugMenu.h"
#include "DebugDataDisplay.h"
#include "Font.h"
#include "FontContainer.h"
#include "FrameTimeDebugger.h"
#include <InputWrapper.h>
#include <sstream>
#include "SystemMonitor.h"
#include "Text.h"
#include "VTuneApi.h"


Easy3D::DebugDataDisplay::DebugDataDisplay()
	: myFunctionTimersStartPos(0.f, 0.f)
	, myMemUsageStartPos(1000.f, 0.f)
	, myCPUUSageStartPos(1000.f, -30.f)
	, myFrameTimeStartPos(1000.f, -60.f)
	, myTextScale(0.75f) 
{
}

Easy3D::DebugDataDisplay::~DebugDataDisplay()
{
	delete myFrameDebugger;
	delete myDebugMenu;

	delete myText;
}

void Easy3D::DebugDataDisplay::Init()
{
	myText = new Text();
	myText->Init("Data/resources/font/font.dds");

	myFrameDebugger = new FrameTimeDebugger();
	myFrameDebugger->Init();

	myDebugMenu = new DebugMenu();
}

void Easy3D::DebugDataDisplay::StartFunctionTimer(const std::string& aFunc)
{
	myFrameDebugger->StartFunctionTimer(aFunc);
}

void Easy3D::DebugDataDisplay::EndFunctionTimer(const std::string& aFunc)
{
	myFrameDebugger->EndFunctionTimer(aFunc);
}

void Easy3D::DebugDataDisplay::RecordFrameTime(const float aDeltaTime)
{
	myLastDeltaTime = aDeltaTime;

	myFrameDebugger->EndFrame(aDeltaTime);
}

void Easy3D::DebugDataDisplay::Render()
{
	if (myBoolContainer.at(eBitSetEnum::FUNCTION_TIMERS))
	{
		RenderFunctionTimers();
	}

	if (myBoolContainer.at(eBitSetEnum::MEMORY_USAGE))
	{
		RenderMemoryUsage();
	}

	if (myBoolContainer.at(eBitSetEnum::CPU_USAGE))
	{
		RenderCPUUsage();
	}

	if (myBoolContainer.at(eBitSetEnum::FRAME_TIME))
	{
		RenderFrameTime();
	}

	myDebugMenu->Render();
}

void Easy3D::DebugDataDisplay::Update(CU::InputWrapper& aInputWrapper)
{
	if (aInputWrapper.KeyDown(DIK_F5))
	{
		ToggleFunctionTimers();
	}
	else if (aInputWrapper.KeyDown(DIK_F6))
	{
		ToggleMemoryUsage();
	}
	else if (aInputWrapper.KeyDown(DIK_F7))
	{
		ToggleCPUUsage();
	}
	else if (aInputWrapper.KeyDown(DIK_F8))
	{
		ToggleFrameTime();
	}

	if (myBoolContainer.at(eBitSetEnum::FUNCTION_TIMERS))
	{
		myFrameDebugger->Update(aInputWrapper);
	}

	myDebugMenu->Update(aInputWrapper);
}

void Easy3D::DebugDataDisplay::RenderFunctionTimers()
{
	VTUNE_EVENT_BEGIN(VTUNE::FUNCTION_TIMERS);

	myFrameDebugger->Render();
	
	VTUNE_EVENT_END();
}

void Easy3D::DebugDataDisplay::RenderMemoryUsage()
{
	TIME_FUNCTION;

	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "Memory: " << Easy3D::SystemMonitor::GetMemoryUsageMB() << "mb" << std::endl;

	myText->Render(myStringStream.str().c_str(), myMemUsageStartPos, { myTextScale, myTextScale });
}

void Easy3D::DebugDataDisplay::RenderCPUUsage()
{
	TIME_FUNCTION;

	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "CPU: "
		<< Easy3D::SystemMonitor::GetCPUUsage() << "%" << std::endl;;

	myText->Render(myStringStream.str().c_str(), myCPUUSageStartPos, { myTextScale, myTextScale });
}

void Easy3D::DebugDataDisplay::RenderFrameTime()
{
	TIME_FUNCTION;

	float FPS = 1.f / myLastDeltaTime;
	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "FPS: " << FPS << std::endl;

	myText->Render(myStringStream.str().c_str(), myFrameTimeStartPos, { myTextScale, myTextScale });


	float frameTimeMS = myLastDeltaTime * 1000.f;
	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "FrameTime: " << frameTimeMS << "ms" << std::endl;

	myText->Render(myStringStream.str().c_str()
		, { myFrameTimeStartPos.x, myFrameTimeStartPos.y - 30.f }, { myTextScale, myTextScale });
}