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

#include <ParticleEmitterData.h>
#include <ParticleEmitterInstance.h>

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
	myCamera->SetPosition({ 0.f, 0.f, -10.f });

	myArm = new Easy3D::Instance(*Easy3D::Engine::GetInstance()->GetModelLoader()->LoadCube(10.f, 10.f, 1.f));
	myArm->SetPosition({ 0.f, 0.f, 2.f });
	myDebugMenu->StartGroup("SystemInfo");
	myDebugMenu->AddVariable("FPS", myFPS);
	myDebugMenu->AddVariable("Memory (MB)", myMemoryUsage);
	myDebugMenu->AddVariable("CPU", myCPUUsage);
	myDebugMenu->EndGroup();
	myDebugMenu->AddVariable("Emitter Position", myEmitterPosition);


	myEmitterData = new Easy3D::ParticleEmitterData();
	myEmitterData->Init("Data/script/particle.xml");

	myEmitterInstance = new Easy3D::ParticleEmitterInstance(*myEmitterData);

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

	if (myInputWrapper->KeyIsPressed(DIK_W))
	{
		myCamera->MoveForward(100.f * myDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_S))
	{
		myCamera->MoveForward(-100.f * myDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_A))
	{
		myCamera->MoveRight(-100.f * myDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_D))
	{
		myCamera->MoveRight(100.f * myDeltaTime);
	}


	if (myInputWrapper->KeyIsPressed(DIK_UP))
	{
		myCamera->RotateX((3.14f * 10) * myDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_DOWN))
	{
		myCamera->RotateX((-3.14f * 10) * myDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_LEFT))
	{
		myCamera->RotateY(-(3.14f * 10) * myDeltaTime);
	}
	if (myInputWrapper->KeyIsPressed(DIK_RIGHT))
	{
		myCamera->RotateY((3.14f * 10) * myDeltaTime);
	}
	



	Easy3D::Engine::GetInstance()->GetDebugDisplay()->Update(*myInputWrapper);
	
	myEmitterInstance->SetPosition(myEmitterPosition);
	myEmitterInstance->Update(myDeltaTime);

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
	myArm->Render(*myCamera);

	Easy3D::Engine::GetInstance()->SetDepthBufferState(Easy3D::eDepthStencilType::PARTICLES);
	Easy3D::Engine::GetInstance()->EnableAlphaBlending();
	myEmitterInstance->Render(*myCamera);
	Easy3D::Engine::GetInstance()->DisableAlphaBlending();
	Easy3D::Engine::GetInstance()->SetDepthBufferState(Easy3D::eDepthStencilType::Z_ENABLED);
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