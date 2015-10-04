#include "stdafx.h"

#include <Camera.h>
#include <DebugDataDisplay.h>
#include <DebugMenu.h>
#include <Engine.h>
#include <FileWatcher.h>
#include "Game.h"
#include <InputWrapper.h>
#include <Instance.h>
#include <Model.h>
#include <ModelLoader.h>
#include "ModelProxy.h"
#include <TimerManager.h>
#include <Sprite.h>
#include <SystemMonitor.h>

Game::Game()
	: myInputWrapper(new CU::InputWrapper())
	, myDebugMenu(new Easy3D::DebugMenu())
{
}

Game::~Game()
{
	delete myInputWrapper;
}

bool Game::Init(HWND& aHwnd)
{
	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myCamera = new Easy3D::Camera();

	mySpriteColor = { 1.f, 1.f, 1.f, 1.f };
	mySpritePos = { 0.f, 0.f };
	mySpriteScale = { 1.f, 1.f };
	mySprite = new Easy3D::Sprite("Data/resources/texture/seafloor.dds", { 100.f, 100.f }, { 50.f, 50.f });

	myModel = new Easy3D::Instance(*Easy3D::Engine::GetInstance()->GetModelLoader()->LoadCube());

	myModel->SetPosition({ 0.f, 0.f, 10.f });

	myDebugMenu->StartGroup("SystemInfo");
	myDebugMenu->AddVariable("FPS", myFPS);
	myDebugMenu->AddVariable("Memory (MB)", myMemoryUsage);
	myDebugMenu->AddVariable("CPU", myCPUUsage);
	myDebugMenu->EndGroup();

	myDebugMenu->StartGroup("Sprite");
	myDebugMenu->AddVariable("Position", mySpritePos);
	myDebugMenu->AddVariable("Color", mySpriteColor, true);
	myDebugMenu->AddVariable("Scale", mySpriteScale);
	myDebugMenu->EndGroup();

	myDebugMenu->StartGroup("Cube");
	myDebugMenu->AddVariable("Position", myCubePosition);
	myDebugMenu->AddVariable("Rotation", myCubeRotation);
	myDebugMenu->EndGroup();

	GAME_LOG("Init Successful");
	return true;
}

bool Game::Destroy()
{
	return true;
}

bool Game::Update()
{
	BEGIN_TIME_BLOCK("Game::Update");
	
	UpdateSubSystems();

	if (myInputWrapper->KeyDown(DIK_ESCAPE))
	{
		return false;
	}

	Easy3D::Engine::GetInstance()->GetDebugDisplay()->Update(*myInputWrapper);
	

	myModel->SetPosition(myCubePosition);
	myModel->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundX(myCubeRotation.x * myDeltaTime));
	myModel->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundY(myCubeRotation.y * myDeltaTime));
	myModel->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundZ(myCubeRotation.z * myDeltaTime));

	Render();

	END_TIME_BLOCK("Game::Update");

	Easy3D::Engine::GetInstance()->GetDebugDisplay()->RecordFrameTime(myDeltaTime);
	return true;
}


void Game::UpdateSubSystems()
{
	Easy3D::Engine::GetInstance()->GetFileWatcher()->CheckFiles();
	myInputWrapper->Update();
	CU::TimerManager::GetInstance()->Update();
	myDeltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();

	myFPS = static_cast<int>(1.f / myDeltaTime);
	myMemoryUsage = Easy3D::SystemMonitor::GetMemoryUsageMB();
	myCPUUsage = Easy3D::SystemMonitor::GetCPUUsage();
}

void Game::Render()
{
	//Easy3D::Engine::GetInstance()->GetDebugDisplay()->Render();
	mySprite->Render(mySpritePos, mySpriteScale, mySpriteColor);
	myModel->Render(*myCamera);
	myDebugMenu->Render(*myInputWrapper);
}

void Game::Pause()
{

}

void Game::UnPause()
{

}

void Game::OnResize(int aWidth, int aHeight)
{
	aWidth;
	aHeight;
}