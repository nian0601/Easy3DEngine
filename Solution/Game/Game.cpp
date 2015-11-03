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
	: myDebugMenu(new Easy3D::DebugMenu())
{
}

Game::~Game()
{
	delete myRenderer;
	delete myDebugMenu;

	delete myCollisionManager;
	delete myScene;
}

bool Game::Init(HWND& aHwnd)
{
	CU::InputWrapper::Create(aHwnd, GetModuleHandle(NULL)
		, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	myCamera = new Easy3D::Camera();
	myCamera->RotateX(90);
	myCamera->SetPosition({ 0.f, 30.f, 0.f });

	myDebugMenu->AddVariable("FPS", myFPS);
	myDebugMenu->AddVariable("FrameTime", myDeltaTime);
	

	myDebugMenu->AddVariable("No Instancing", std::bind(&Game::ToggleSetting, this, eGameSettings::NO_INSTANCING));
	myDebugMenu->AddVariable("Easy Instancing", std::bind(&Game::ToggleSetting, this, eGameSettings::EASY_INSTANCING));
	myDebugMenu->AddVariable("Hardware Instancing", std::bind(&Game::ToggleSetting, this, eGameSettings::HARDWARE_INSTANCING));
	myRenderMode = eGameSettings::NO_INSTANCING;

	myRenderer = new Easy3D::Renderer();
	myCollisionManager = new CollisionManager();
	myScene = new Easy3D::Scene();
	myScene->SetCamera(myCamera);
	myInstancedScene = new Easy3D::Scene();
	myInstancedScene->SetCamera(myCamera);

	myEntityManager = new EntityManager(myScene, myCollisionManager);

	Easy3D::Scene* currScene = myScene;
	XMLReader reader;
	reader.OpenDocument("Data/script/entities.xml");
	XMLELEMENT entityElem = reader.FindFirstChild("entity");
	while (entityElem != nullptr)
	{
		for (int i = 0; i < 500; ++i)
		{
			std::string file;
			reader.ForceReadAttribute(entityElem, "file", file);

			XMLELEMENT pos = reader.ForceFindFirstChild(entityElem, "position");
			CU::Vector3<float> position;
			reader.ForceReadAttribute(pos, "x", position.x);
			reader.ForceReadAttribute(pos, "y", position.y);
			reader.ForceReadAttribute(pos, "z", position.z);

			CU::Vector3<float> randomizer(20.f, 20.f, 20.f);
			position += CU::Math::RandomVector(-randomizer, randomizer);

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
		}

		entityElem = reader.FindNextElement(entityElem, "entity");
		currScene = myInstancedScene;
	}

	
	Easy3D::DirectionalLight* dirLight = new Easy3D::DirectionalLight();
	dirLight->SetColor({ 1.f, 0.5f, 0.5f, 1.f });
	dirLight->SetDir({ 0.f, -1.f, -1.f });
	//myScene->AddLight(dirLight);
	//myInstancedScene->AddLight(dirLight);

	myRenderProcessTarget = new Easy3D::RenderProcessTarget();
	myRenderProcessTarget->SetCamera(myCamera);

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

	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_1))
	{
		myRenderMode = eGameSettings::NO_INSTANCING;
	}
	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_2))
	{
		myRenderMode = eGameSettings::EASY_INSTANCING;
	}
	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_3))
	{
		myRenderMode = eGameSettings::HARDWARE_INSTANCING;
	}
}

void Game::Render()
{
	//myRenderer->StartFontRendering();
	//myDebugMenu->Render(*CU::InputWrapper::GetInstance());
	//myRenderer->EndFontRendering();
	//
	//if (mySettings.at(eGameSettings::SCENE_ONE_RENDER))
	//{
	//	myRenderer->ProcessScene(myScene, mySceneEffect);
	//}
	//
	////if (mySettings.at(eGameSettings::SCENE_TWO_RENDER))
	////{
	////	myRenderer->ProcessScene(mySecondScene, mySecondSceneEffect);
	////}
	//
	//myRenderer->FinalRender();

	//myDebugMenu->Render(*CU::InputWrapper::GetInstance());

	switch (myRenderMode)
	{
	case NO_INSTANCING:
		myScene->Render();
		break;
	case EASY_INSTANCING:
		myScene->Render(myRenderProcessTarget);
		myRenderProcessTarget->RenderNormal();
		break;
	case HARDWARE_INSTANCING:
		myInstancedScene->Render(myRenderProcessTarget);
		myRenderProcessTarget->RenderInstanced();
		break;
	default:
		break;
	}
	
}

void Game::ToggleSetting(eGameSettings aSetting)
{
	myRenderMode = aSetting;
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