#include "stdafx.h"

#include <Camera.h>
#include <DebugMenu.h>
#include <DebugDataDisplay.h>
#include <Engine.h>
#include <FileWatcher.h>
#include <Font.h>
#include "Game.h"
#include <InputWrapper.h>
#include <Instance.h>
#include <Model.h>
#include "ModelProxy.h"
#include <TimerManager.h>
#include <Sprite.h>

Game::Game()
	: myInputWrapper(new CU::InputWrapper())
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


	ADD_FUNCTION_TO_RADIAL_MENU("Toggle FPS", Easy3D::DebugDataDisplay::ToggleFrameTime
		, Easy3D::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Graph", Easy3D::DebugDataDisplay::ToggleFunctionTimers
		, Easy3D::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Mem", Easy3D::DebugDataDisplay::ToggleMemoryUsage
		, Easy3D::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle CPU", Easy3D::DebugDataDisplay::ToggleCPUUsage
		, Easy3D::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Wireframe", Easy3D::Engine::ToggleWireframe
		, Easy3D::Engine::GetInstance());
	

	myCamera = new Easy3D::Camera();

	mySprite = new Easy3D::Sprite("Data/resources/texture/seafloor.dds", { 100.f, 100.f });
	
	Easy3D::Model* Model = new Easy3D::Model();
	Model->InitCube();
	Easy3D::ModelProxy* Proxy = new Easy3D::ModelProxy();
	Proxy->SetModel(Model);
	myCube = new Easy3D::Instance(*Proxy);
	myCube->SetPosition({ 0.f, 0.f, 10.f });

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
	Easy3D::Engine::GetInstance()->GetFileWatcher()->CheckFiles();
	myInputWrapper->Update();
	CU::TimerManager::GetInstance()->Update();
	float delta = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();

	if (myInputWrapper->KeyDown(DIK_ESCAPE))
	{
		return false;
	}


	Easy3D::Engine::GetInstance()->GetDebugDisplay()->Update(*myInputWrapper);
	

	myCube->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundY(3.14f * delta));
	myCube->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundZ(3.14f * delta));

	Render();

	END_TIME_BLOCK("Game::Update");

	Easy3D::Engine::GetInstance()->GetDebugDisplay()->RecordFrameTime(delta);
	return true;
}

void Game::Render()
{
	Easy3D::Engine::GetInstance()->GetDebugDisplay()->Render();
	mySprite->Render({ 500.f, -200.f }, { 3.f, 3.f });
	myCube->Render(*myCamera);
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