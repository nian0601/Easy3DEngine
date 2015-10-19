#include "stdafx.h"

#include <Camera.h>
#include "CollisionManager.h"
#include <DebugDataDisplay.h>
#include <DebugMenu.h>
#include <DirectionalLight.h>
#include "Entity.h"
#include <Engine.h>
#include <FileWatcher.h>
#include "Game.h"
#include "GraphicsComponent.h"
#include <InputWrapper.h>
#include <Renderer.h>
#include <Scene.h>
#include <SystemMonitor.h>
#include <TimerManager.h>
#include <XMLReader.h>


Game::Game()
	: myDebugMenu(new Easy3D::DebugMenu())
{
}

Game::~Game()
{
}

bool Game::Init(HWND& aHwnd)
{
	CU::InputWrapper::Create(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myCamera = new Easy3D::Camera();
	myCamera->SetPosition({ 0.f, 25.f, -10.f });
	myCamera->RotateX(45);

	myDebugMenu->StartGroup("SystemInfo");
	myDebugMenu->AddVariable("FPS", myFPS);
	myDebugMenu->AddVariable("Memory (MB)", myMemoryUsage);
	myDebugMenu->AddVariable("CPU", myCPUUsage);
	myDebugMenu->EndGroup();

	myRenderer = new Easy3D::Renderer();

	myEntities.Init(4);
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

		Entity* newEntity = new Entity();
		newEntity->LoadFromScript(file);

		newEntity->Rotate(CU::Matrix44<float>::CreateRotateAroundX(rotation.x));
		newEntity->Rotate(CU::Matrix44<float>::CreateRotateAroundY(rotation.y));
		newEntity->Rotate(CU::Matrix44<float>::CreateRotateAroundZ(rotation.z));
		newEntity->SetPosition(position);

		myEntities.Add(newEntity);
		entityElem = reader.FindNextElement(entityElem, "entity");
	}

	myCollisionManager = new CollisionManager();
	myScene = new Easy3D::Scene();
	myScene->SetCamera(myCamera);
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		CollisionComponent* comp = reinterpret_cast<CollisionComponent*>(myEntities[i]->GetComponent(eComponent::COLLISION));
		if (comp != nullptr)
		{
			myCollisionManager->Add(comp);
		}

		GraphicsComponent* gfx = reinterpret_cast<GraphicsComponent*>(myEntities[i]->GetComponent(eComponent::GRAPHIC));
		if (gfx != nullptr)
		{
			if (gfx->GetInstance() != nullptr)
			{
				myScene->AddInstance(gfx->GetInstance());
			}
		}
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
	BEGIN_TIME_BLOCK("Game::Update");
	
	UpdateSubSystems();

	if (CU::InputWrapper::GetInstance()->KeyDown(DIK_ESCAPE))
	{
		return false;
	}

	myCollisionManager->CleanUp();

	

	for (int i = myEntities.Size() - 1; i >= 0; --i)
	{
		if (myEntities[i]->IsAlive() == false)
		{
			myEntities.DeleteCyclicAtIndex(i);
			continue;
		}

		myEntities[i]->Update(myDeltaTime);
	}

	myCollisionManager->CheckCollisions();


	Easy3D::Engine::GetInstance()->GetDebugDisplay()->Update(*CU::InputWrapper::GetInstance());

	Render();

	END_TIME_BLOCK("Game::Update");

	Easy3D::Engine::GetInstance()->GetDebugDisplay()->RecordFrameTime(myDeltaTime);
	return true;
}


void Game::UpdateSubSystems()
{
	Easy3D::Engine::GetInstance()->GetFileWatcher()->CheckFiles();
	CU::InputWrapper::GetInstance()->Update();
	CU::TimerManager::GetInstance()->Update();
	myDeltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();

	myFPS = static_cast<int>(1.f / myDeltaTime);
	myMemoryUsage = Easy3D::SystemMonitor::GetMemoryUsageMB();
	myCPUUsage = Easy3D::SystemMonitor::GetCPUUsage();
}

void Game::Render()
{
	myDebugMenu->Render(*CU::InputWrapper::GetInstance());

	myRenderer->ProcessScene(myScene, Easy3D::ePostProcess::BLOOM);
	myRenderer->FinalRender();
	//myScene->Render();
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