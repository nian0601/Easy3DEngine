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

#include <numeric>
#include <algorithm>
#include <fstream>

Game::Game()
	: myDebugMenu(new Easy3D::DebugMenu())
	, myPlayer(nullptr)
	, myTextInput("")
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


	myCorrectStrings.Init(16);
	myCorrectStrings.Add("apa");
	myCorrectStrings.Add("spawnenemy");
	myCorrectStrings.Add("run");
	myCorrectStrings.Add("loadmap");
	myCorrectStrings.Add("godmode");
	myCorrectStrings.Add("a long sentance");
	LoadDictionary();

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

	if (myTextInput.size() > 0)
	{
		mySuggestion = FindClosestString(myTextInput);
	}
	else
	{
		mySuggestion = "";
	}
	ReadTextInput();
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
	//myDebugMenu->Render(*CU::InputWrapper::GetInstance());

	Easy3D::Engine::GetInstance()->PrintDebugText(myTextInput, { 300.f, -300.f });
	Easy3D::Engine::GetInstance()->PrintDebugText(mySuggestion, { 300.f, -330.f });
	myRenderer->EndFontRendering();
	
	myRenderer->ProcessScene(myScene, Easy3D::HDR);
	
	myRenderer->FinalRender();
}

void Game::ToggleCamera()
{
	myCameraEnabled = !myCameraEnabled;
	myPlayer->SendNote(ToggleInputNote(!myCameraEnabled));
}

void Game::ReadTextInput()
{
	CU::InputWrapper* input = CU::InputWrapper::GetInstance();

	bool shift = input->KeyIsPressed(DIK_LSHIFT) || input->KeyIsPressed(DIK_RSHIFT);

	myTextInput += input->KeyDown(DIK_A) ? shift ? "A" : "a" : "";
	myTextInput += input->KeyDown(DIK_B) ? shift ? "B" : "b" : "";
	myTextInput += input->KeyDown(DIK_C) ? shift ? "C" : "c" : "";
	myTextInput += input->KeyDown(DIK_D) ? shift ? "D" : "d" : "";
	myTextInput += input->KeyDown(DIK_E) ? shift ? "E" : "e" : "";
	myTextInput += input->KeyDown(DIK_F) ? shift ? "F" : "f" : "";
	myTextInput += input->KeyDown(DIK_G) ? shift ? "G" : "g" : "";
	myTextInput += input->KeyDown(DIK_H) ? shift ? "H" : "h" : "";
	myTextInput += input->KeyDown(DIK_I) ? shift ? "I" : "i" : "";
	myTextInput += input->KeyDown(DIK_J) ? shift ? "J" : "j" : "";
	myTextInput += input->KeyDown(DIK_K) ? shift ? "K" : "k" : "";
	myTextInput += input->KeyDown(DIK_L) ? shift ? "L" : "l" : "";
	myTextInput += input->KeyDown(DIK_M) ? shift ? "M" : "m" : "";
	myTextInput += input->KeyDown(DIK_N) ? shift ? "N" : "n" : "";
	myTextInput += input->KeyDown(DIK_O) ? shift ? "O" : "o" : "";
	myTextInput += input->KeyDown(DIK_P) ? shift ? "P" : "p" : "";
	myTextInput += input->KeyDown(DIK_Q) ? shift ? "Q" : "q" : "";
	myTextInput += input->KeyDown(DIK_R) ? shift ? "R" : "r" : "";
	myTextInput += input->KeyDown(DIK_S) ? shift ? "S" : "s" : "";
	myTextInput += input->KeyDown(DIK_T) ? shift ? "T" : "t" : "";
	myTextInput += input->KeyDown(DIK_U) ? shift ? "U" : "u" : "";
	myTextInput += input->KeyDown(DIK_V) ? shift ? "V" : "v" : "";
	myTextInput += input->KeyDown(DIK_W) ? shift ? "W" : "w" : "";
	myTextInput += input->KeyDown(DIK_X) ? shift ? "X" : "x" : "";
	myTextInput += input->KeyDown(DIK_Y) ? shift ? "Y" : "y" : "";
	myTextInput += input->KeyDown(DIK_Z) ? shift ? "Z" : "z" : "";

	if (input->KeyDown(DIK_BACKSPACE) && myTextInput.size() > 0)
	{
		myTextInput.erase(myTextInput.end() - 1, myTextInput.end());
	}

	if (input->KeyDown(DIK_SPACE))
	{
		myTextInput += " ";
	}

	if (input->KeyDown(DIK_RETURN) && myTextInput.size() > 0)
	{
		if (shift)
		{
			myCorrectStrings.Add(myTextInput);
		}
		else
		{
			myTextInput = mySuggestion;
		}
		
	}
}

#undef min
#undef max
int Game::GetLevenshteinDistance(const std::string &s1, const std::string &s2)
{
	// To change the type this function manipulates and returns, change
	// the return type and the types of the two variables below.
	int s1len = s1.size();
	int s2len = s2.size();

	auto column_start = (decltype(s1len))1;

	auto column = new decltype(s1len)[s1len + 1];
	std::iota(column + column_start, column + s1len + 1, column_start);

	for (auto x = column_start; x <= s2len; x++) {
		column[0] = x;
		auto last_diagonal = x - column_start;
		for (auto y = column_start; y <= s1len; y++) {
			auto old_diagonal = column[y];
			auto possibilities = {
				column[y] + 1,
				column[y - 1] + 1,
				last_diagonal + (s1[y - 1] == s2[x - 1] ? 0 : 1)
			};
			column[y] = std::min(possibilities);
			last_diagonal = old_diagonal;
		}
	}
	auto result = column[s1len];
	delete[] column;
	return result;
}

float Game::GetLevenshteinRatio(const std::string& aString, int aLevenshtienDistance)
{
	if (aLevenshtienDistance == 0)
	{
		return 1.f;
	}

	return 1.f - float(aString.size()) / aLevenshtienDistance;
}

int Game::GetSubstringBonus(const std::string& aInput, const std::string& aCorrectString, int aScore)
{
	if (aInput.size() <= 1)
	{
		return 0;
	}

	if (aCorrectString.find(aInput) != std::string::npos)
	{
		return aScore;
	}


	std::string input(aInput);
	int middle = input.size() / 2;

	int subScoreOne = GetSubstringBonus(std::string(input.begin(), input.begin() + middle), aCorrectString, aScore / 2);
	int subScoreTwo = GetSubstringBonus(std::string(input.begin() + middle, input.end()), aCorrectString, aScore / 2);

	return std::max(subScoreOne, subScoreTwo);
}

std::string Game::FindClosestString(const std::string& aInput)
{
	int maxLevenshteinScore = 15;

	float closest = -1;
	std::string bestString;

	for (unsigned int i = 0; i < myCorrectStrings.Size(); ++i)
	{
		const std::string& correctString = myCorrectStrings[i];

		float score = 0.f;

		//If the current string contains the input-string, then we should bump up the score
		//to make it more likley to find a relevant string
		score += float(GetSubstringBonus(aInput, correctString, 5));
		/*if (correctString.find(aInput) != std::string::npos)
		{
			score += 5.f;
		}*/


		//If the LevensteinDistance is short, we should give the string a better score
		//If levDist is 0  we get full levenScore, else we get a percentage of it
		float levDist = float(GetLevenshteinDistance(aInput, correctString));
		if (levDist == 0)
		{
			score += maxLevenshteinScore;
		}
		else
		{
			float levRatio = GetLevenshteinRatio(aInput, levDist);
			levRatio = 1.f + (1.f - levRatio);
			levDist *= levRatio;
			score += (1.f / levDist) * maxLevenshteinScore;
		}


		
		if (score > closest)
		{
			closest = score;
			bestString = myCorrectStrings[i];
		}
	}

	return bestString;
}

void Game::LoadDictionary()
{
	std::ifstream infile("Data/dictionary.txt");

	std::string str;
	while (infile >> str)
	{
		myCorrectStrings.Add(str);
	}
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