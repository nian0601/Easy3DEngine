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
#include <MathHelper.h>
#include <ParticleEmitterInstance.h>
#include <Renderer.h>
#include <RenderProcessTarget.h>
#include <Scene.h>
#include <SystemMonitor.h>
#include <TimerManager.h>
#include <XMLReader.h>


Game::Game()
{
}

Game::~Game()
{
	delete myRenderer;

	delete myCollisionManager;
	delete myScene;
}

bool Game::Init(HWND& aHwnd)
{
	CU::InputWrapper::Create(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myCamera = new Easy3D::Camera();
	myCamera->SetPosition({ 2.f, 3.f, -5.f });

	myRenderer = new Easy3D::Renderer();
	myCollisionManager = new CollisionManager();
	myScene = new Easy3D::Scene();
	myScene->SetCamera(myCamera);

	myEntityManager = new EntityManager(myScene, myCollisionManager);

	Easy3D::Scene* currScene = myScene;
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

		Entity* newEntity = myEntityManager->CreateEntity(file);

		newEntity->Rotate(CU::Matrix44<float>::CreateRotateAroundX(rotation.x));
		newEntity->Rotate(CU::Matrix44<float>::CreateRotateAroundY(rotation.y));
		newEntity->Rotate(CU::Matrix44<float>::CreateRotateAroundZ(rotation.z));
		newEntity->SetPosition(position);

		GraphicsComponent* gfx = static_cast<GraphicsComponent*>(newEntity->GetComponent(eComponent::GRAPHIC));
		if (gfx != nullptr)
		{
			if (gfx->GetInstance() != nullptr)
			{
				currScene->AddInstance(gfx->GetInstance());
			}
		}

		entityElem = reader.FindNextElement(entityElem, "entity");
	}

	
	Easy3D::DirectionalLight* dirLight = new Easy3D::DirectionalLight();
	dirLight->SetColor({ 1.f, 0.5f, 0.5f, 1.f });
	dirLight->SetDir({ 0.f, -1.f, -1.f });

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

	myScene->Update(myDeltaTime);
	Render();
	return true;
}


void Game::UpdateSubSystems()
{
	//Easy3D::Engine::GetInstance()->GetFileWatcher()->CheckFiles();
	CU::InputWrapper::GetInstance()->Update();
	CU::TimerManager::GetInstance()->Update();
	myDeltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();

	myFPS = static_cast<int>(1.f / myDeltaTime);
	myMemoryUsage = Easy3D::SystemMonitor::GetMemoryUsageMB();
	myCPUUsage = Easy3D::SystemMonitor::GetCPUUsage();


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
	myScene->Render();
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