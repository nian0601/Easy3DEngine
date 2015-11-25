#include "stdafx.h"

#include <Camera.h>
#include "CollisionManager.h"
#include <DebugMenu.h>
#include <DirectionalLight.h>
#include "Entity.h"
#include "EntityManager.h"
#include <EmitterContainer.h>
#include <Engine.h>
#include <FileWatcher.h>
#include "Game.h"
#include "GraphicsComponent.h"
#include <InputWrapper.h>
#include <LineRenderer.h>
#include <ParticleEmitterInstance.h>
#include <Renderer.h>
#include <Scene.h>
#include <SystemMonitor.h>
#include <TimerManager.h>
#include "ToggleInputNote.h"
#include <XMLReader.h>


#include <D3DPointers.h>

Game::Game()
	: myDebugMenu(new Easy3D::DebugMenu())
	, myPlayer(nullptr)
{
}

Game::~Game()
{
	delete myRenderer;
	delete myDebugMenu;

	delete myCollisionManager;
	delete myScene;
}

bool Game::Init(HWND& aHwnd, bool aUseInputRecording)
{
	CU::InputWrapper::Create(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
		, aUseInputRecording);

	myCamera = new Easy3D::Camera();
	/*myCamera->RotateX(90);
	myCamera->SetPosition({ 0.f, 30.f, 0.f });*/

	myCamera->SetPosition({ 0, 2, -2 });

	myDebugMenu->StartGroup("SystemInfo");
	myDebugMenu->AddVariable("FPS", myFPS);
	myDebugMenu->AddVariable("Memory (MB)", myMemoryUsage);
	myDebugMenu->AddVariable("CPU", myCPUUsage);
	myDebugMenu->AddVariable("ToggleCamera", std::bind(&Game::ToggleCamera, this));
	myDebugMenu->EndGroup();


	myRenderer = new Easy3D::Renderer();
	myCollisionManager = new CollisionManager();
	myScene = new Easy3D::Scene();
	myScene->SetCamera(myCamera);

	myEntityManager = new EntityManager(myScene, myCollisionManager);

	XMLReader reader;
	reader.OpenDocument("Data/script/entities.xml");
	XMLELEMENT entityElem = reader.FindFirstChild("entity");
	while (entityElem != nullptr)
	{
		std::string file;
		reader.ForceReadAttribute(entityElem, "file", file);
	
		XMLELEMENT pos = reader.ForceFindFirstChild(entityElem, "position");
		CU::Vector3<float> position;
		reader.ForceReadAttribute(pos, "x", position.x);
		reader.ForceReadAttribute(pos, "y", position.y);
		reader.ForceReadAttribute(pos, "z", position.z);
	
		XMLELEMENT rot = reader.ForceFindFirstChild(entityElem, "rotation");
		CU::Vector3<float> rotation;
		reader.ForceReadAttribute(rot, "x", rotation.x);
		reader.ForceReadAttribute(rot, "y", rotation.y);
		reader.ForceReadAttribute(rot, "z", rotation.z);

		XMLELEMENT type = reader.ForceFindFirstChild(entityElem, "entityType");
		std::string entityType;
		reader.ForceReadAttribute(type, "type", entityType);

		Entity* newEntity = nullptr;
		if (entityType == "player")
		{
			DL_ASSERT_EXP(myPlayer == nullptr, "Tried to add several Players");
			newEntity = myEntityManager->CreateEntity(file, eEntityType::PLAYER);
			myPlayer = newEntity;
		}
		else if (entityType == "material")
		{
			newEntity = myEntityManager->CreateEntity(file, eEntityType::PICKABLE);
		}
	
		newEntity->Rotate(CU::Matrix44<float>::CreateRotateAroundX(rotation.x));
		newEntity->Rotate(CU::Matrix44<float>::CreateRotateAroundY(rotation.y));
		newEntity->Rotate(CU::Matrix44<float>::CreateRotateAroundZ(rotation.z));
		newEntity->SetPosition(position);
	

		entityElem = reader.FindNextElement(entityElem, "entity");
	}

	Easy3D::DirectionalLight* dirLight = new Easy3D::DirectionalLight();
	dirLight->SetColor({ 1.f, 0.5f, 0.5f, 1.f });
	dirLight->SetDir({ 0.f, -1.f, -1.f });
	myScene->AddLight(dirLight);

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

	myCollisionManager->CleanUp();
	
	myEntityManager->Update(myDeltaTime);

	myCollisionManager->CheckCollisions();


	Render();
	return true;
}


void Game::UpdateSubSystems()
{
	Easy3D::FileWatcher::GetInstance()->CheckFiles();
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
			myCamera->MoveForward(100.f * myDeltaTime);
		}
		if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_S))
		{
			myCamera->MoveForward(-100.f * myDeltaTime);
		}
		if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_A))
		{
			myCamera->MoveRight(-100.f * myDeltaTime);
		}
		if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_D))
		{
			myCamera->MoveRight(100.f * myDeltaTime);
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
	myPlayer->SendNote(ToggleInputNote(!myCameraEnabled));
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