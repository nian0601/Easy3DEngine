#include "stdafx.h"

#include <Camera.h>
#include <DebugMenu.h>
#include <DebugDataDisplay.h>
#include <Engine.h>
#include <FileWatcher.h>
#include <Font.h>
#include "Game.h"
#include <InputWrapper.h>
#include <TimerManager.h>


Game::Game()
	: myLockMouse(true)
{
	myInputWrapper = new CU::InputWrapper();
}

Game::~Game()
{
	delete myInputWrapper;
}

bool Game::Init(HWND& aHwnd)
{
	myInputWrapper->Init(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);


	ADD_FUNCTION_TO_RADIAL_MENU("Toggle FPS", Easy3D::DebugDataDisplay::ToggleFrameTime, Easy3D::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Graph", Easy3D::DebugDataDisplay::ToggleFunctionTimers, Easy3D::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Mem", Easy3D::DebugDataDisplay::ToggleMemoryUsage, Easy3D::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle CPU", Easy3D::DebugDataDisplay::ToggleCPUUsage, Easy3D::Engine::GetInstance()->GetDebugDisplay());
	ADD_FUNCTION_TO_RADIAL_MENU("Toggle Wireframe", Easy3D::Engine::ToggleWireframe, Easy3D::Engine::GetInstance());

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
	myInputWrapper->Update();
	CU::TimerManager::GetInstance()->Update();
	float delta = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();

	if (myInputWrapper->KeyDown(DIK_ESCAPE))
	{
		return false;
	}

	if (myInputWrapper->KeyUp(DIK_O) == true)
	{
		myLockMouse = !myLockMouse;
		ShowCursor(!myLockMouse);
	}

	if (myLockMouse == true)
	{
		SetCursorPos(myWindowSize.x / 2, myWindowSize.y / 2);
	}

	Easy3D::Engine::GetInstance()->GetDebugDisplay()->Update(*myInputWrapper);
	Easy3D::Engine::GetInstance()->GetDebugDisplay()->RecordFrameTime(delta);

	Render();
	END_TIME_BLOCK("Game::Update");
	return true;
}

void Game::Render()
{
	Easy3D::Engine::GetInstance()->GetDebugDisplay()->Render();
}

void Game::Pause()
{

}

void Game::UnPause()
{

}

void Game::OnResize(int aWidth, int aHeight)
{
	myWindowSize.x = aWidth;
	myWindowSize.y = aHeight;
}