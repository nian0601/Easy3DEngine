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


Prism::DebugDataDisplay::DebugDataDisplay()
	: myFunctionTimersStartPos(0.f, 0.f)
	, myMemUsageStartPos(1000.f, 0.f)
	, myCPUUSageStartPos(1000.f, -30.f)
	, myFrameTimeStartPos(1000.f, -60.f)
	, myTextScale(0.75f) 
{
}

Prism::DebugDataDisplay::~DebugDataDisplay()
{
	delete myFrameDebugger;
	delete myDebugMenu;

	delete myText;
}

void Prism::DebugDataDisplay::Init()
{
	myText = new Text();
	myText->Init(Engine::GetInstance()->GetFontContainer()->GetFont("Data/resources/font/font.dds"));

	myFrameDebugger = new FrameTimeDebugger();
	myFrameDebugger->Init();

	myDebugMenu = new DebugMenu();
}

void Prism::DebugDataDisplay::StartFunctionTimer(const std::string& aFunc)
{
	myFrameDebugger->StartFunctionTimer(aFunc);
}

void Prism::DebugDataDisplay::EndFunctionTimer(const std::string& aFunc)
{
	myFrameDebugger->EndFunctionTimer(aFunc);
}

void Prism::DebugDataDisplay::RecordFrameTime(const float aDeltaTime)
{
	myLastDeltaTime = aDeltaTime;

	myFrameDebugger->EndFrame(aDeltaTime);
}

void Prism::DebugDataDisplay::Render()
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

void Prism::DebugDataDisplay::Update(CU::InputWrapper& aInputWrapper)
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

void Prism::DebugDataDisplay::RenderFunctionTimers()
{
	VTUNE_EVENT_BEGIN(VTUNE::FUNCTION_TIMERS);

	myFrameDebugger->Render();
	
	VTUNE_EVENT_END();
}

void Prism::DebugDataDisplay::RenderMemoryUsage()
{
	TIME_FUNCTION

	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "Memory: " << Prism::SystemMonitor::GetMemoryUsageMB() << "mb" << std::endl;

	myText->Render(myStringStream.str().c_str(), myMemUsageStartPos.x, myMemUsageStartPos.y, myTextScale);
}

void Prism::DebugDataDisplay::RenderCPUUsage()
{
	TIME_FUNCTION

	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "CPU: "
		<< Prism::SystemMonitor::GetCPUUsage() << "%" << std::endl;;

	myText->Render(myStringStream.str().c_str(), myCPUUSageStartPos.x, myCPUUSageStartPos.y, myTextScale);
}

void Prism::DebugDataDisplay::RenderFrameTime()
{
	TIME_FUNCTION

	float FPS = 1.f / myLastDeltaTime;
	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "FPS: " << FPS << std::endl;

	myText->Render(myStringStream.str().c_str(), myFrameTimeStartPos.x, myFrameTimeStartPos.y, myTextScale);


	float frameTimeMS = myLastDeltaTime * 1000.f;
	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "FrameTime: " << frameTimeMS << "ms" << std::endl;

	myText->Render(myStringStream.str().c_str()
		, myFrameTimeStartPos.x, myFrameTimeStartPos.y - 30.f, myTextScale);
}