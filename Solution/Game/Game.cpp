#include "stdafx.h"

#include <Camera.h>
#include <DebugMenu.h>
#include <DirectionalLight.h>
#include <Engine.h>
#include "Game.h"
#include <InputWrapper.h>
#include <Renderer.h>
#include <Scene.h>
#include <SystemMonitor.h>
#include <TimerManager.h>

#include <Instance.h>
#include <ModelProxy.h>
#include <ModelLoader.h>
#include <Video.h>

#include <Model.h>
#include <VideoTransmitter.h>

Game::Game()
	: myDebugMenu(new Easy3D::DebugMenu())
{
}

Game::~Game()
{
	delete myRenderer;
	delete myDebugMenu;

	delete myScene;
}

bool Game::Init(HWND& aHwnd, bool aUseInputRecording)
{
	CU::InputWrapper::Create(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
		, aUseInputRecording);

	myCamera = new Easy3D::Camera();

	myCamera->SetPosition({ 0, 2, -2 });

	myDebugMenu->StartGroup("SystemInfo");
	myDebugMenu->AddVariable("FPS", myFPS);
	myDebugMenu->AddVariable("Memory (MB)", myMemoryUsage);
	myDebugMenu->AddVariable("CPU", myCPUUsage);
	myDebugMenu->AddVariable("ToggleCamera", std::bind(&Game::ToggleCamera, this));
	myDebugMenu->EndGroup();

	myRenderer = new Easy3D::Renderer();
	myScene = new Easy3D::Scene();
	myScene->SetCamera(myCamera);


	Easy3D::DirectionalLight* dirLight = new Easy3D::DirectionalLight();
	dirLight->SetColor({ 1.f, 0.5f, 0.5f, 1.f });
	dirLight->SetDir({ 0.f, -1.f, -1.f });
	myScene->AddLight(dirLight);


	myMonstersInstance = new Easy3D::Instance(*Easy3D::ModelLoader::GetInstance()->RequestModel("Data/Resource/Model/companion/companion.fbx", "Data/Shader/3D/BasicEffect.fx"), myMonstersInstanceOrientation);
	myScene->AddInstance(myMonstersInstance);
	myMonstersInstanceOrientation *= CU::Matrix44<float>::CreateRotateAroundZ(3.14f);
	myMonstersInstanceOrientation.SetPos({ 1.5f, 1.f, 2.f, 1.f });

	myMinionsInstance = new Easy3D::Instance(*Easy3D::ModelLoader::GetInstance()->RequestModel("Data/Resource/Model/companion2/companion.fbx", "Data/Shader/3D/BasicEffect.fx"), myMinionsInstanceOrientation);
	myScene->AddInstance(myMinionsInstance);
	myMinionsInstanceOrientation *= CU::Matrix44<float>::CreateRotateAroundZ(3.14f);
	myMinionsInstanceOrientation.SetPos({ -1.5f, 1.f, 2.f, 1.f });

	Easy3D::VideoTransmitter::GetInstance()->AddReceiver(L"Normal", myMonstersInstance);
	Easy3D::VideoTransmitter::GetInstance()->AddReceiver(L"Normal2", myMinionsInstance);

	Easy3D::VideoTransmitter::GetInstance()->AddVideo(L"Monsters Ink", L"Data/Resource/Video/test_video.wmv");
	Easy3D::VideoTransmitter::GetInstance()->AddVideo(L"Minions", L"Data/Resource/Video/minions.wmv");
	Easy3D::VideoTransmitter::GetInstance()->AddVideo(L"Emergency", L"Data/Resource/Video/emergency.wmv");
	Easy3D::VideoTransmitter::GetInstance()->StartFeed(L"Monsters Ink", L"Normal");
	Easy3D::VideoTransmitter::GetInstance()->StartFeed(L"Minions", L"Normal2");

	GAME_LOG("Init Successful");
	return true;
}

bool Game::Destroy()
{
	return true;
}

bool Game::Update()
{
	UpdateSubSystems();

	if (CU::InputWrapper::GetInstance()->KeyDown(DIK_ESCAPE))
	{
		return false;
	}

	Easy3D::VideoTransmitter* transmitter = Easy3D::VideoTransmitter::GetInstance();
	if (CU::InputWrapper::GetInstance()->KeyDown(DIK_1))
	{
		transmitter->StartEmergencyVideo(L"Emergency");
	}
	else if (CU::InputWrapper::GetInstance()->KeyDown(DIK_2))
	{
		transmitter->StopEmergencyVideo();
	}

	transmitter->Update();
	Render();
	return true;
}


void Game::UpdateSubSystems()
{
	CU::InputWrapper::GetInstance()->Update();
	CU::TimerManager::GetInstance()->Update();
	myDeltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();

	myFPS = static_cast<int>(1.f / myDeltaTime);
	myMemoryUsage = Easy3D::SystemMonitor::GetMemoryUsageMB();
	myCPUUsage = Easy3D::SystemMonitor::GetCPUUsage();


	if (myCameraEnabled == true)
	{
		if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_W))
		{
			myCamera->MoveForward(10.f * myDeltaTime);
		}
		if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_S))
		{
			myCamera->MoveForward(-10.f * myDeltaTime);
		}
		if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_A))
		{
			myCamera->MoveRight(-10.f * myDeltaTime);
		}
		if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_D))
		{
			myCamera->MoveRight(10.f * myDeltaTime);
		}
	}


	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_UP))
	{
		myCamera->RotateX(-45.f * myDeltaTime);
	}
	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_DOWN))
	{
		myCamera->RotateX(45.f * myDeltaTime);
	}
	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_LEFT))
	{
		myCamera->RotateY(-45.f * myDeltaTime);
	}
	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_RIGHT))
	{
		myCamera->RotateY(45.f * myDeltaTime);
	}
}

void Game::Render()
{
	myRenderer->StartFontRendering();
	myDebugMenu->Render(*CU::InputWrapper::GetInstance());
	myRenderer->EndFontRendering();
	
	myRenderer->ProcessScene(myScene, Easy3D::HDR);
	
	myRenderer->FinalRender();
}

void Game::ToggleCamera()
{
	myCameraEnabled = !myCameraEnabled;
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