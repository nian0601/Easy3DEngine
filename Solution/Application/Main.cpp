#include "stdafx.h"
#include <Game.h>
#include <SetupInfo.h>
#include <string.h>
#include <istream>
#include <atlstr.h>
#include <TimerManager.h>
//#include <vld.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ReadSetup(Easy3D::SetupInfo& aSetup, const std::string& aFilePath);
void OnResize();

Game* globalGame = nullptr;
int globalClientWidth = 800;
int globalClientHeight = 600;
bool globalIsResizing = false;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int aNumberCommands)
{

	DL_Debug::Debug::Create();
	CU::TimerManager::Create();

	if (aNumberCommands > 2)
	{
		int commandCount = 0;
		LPWSTR* realCommands = CommandLineToArgvW(GetCommandLineW(), &commandCount);

		for (int i = 1; i < commandCount; ++i)
		{
			std::string command = CW2A(realCommands[i]);
			if (command == "-useEngineLog")
			{
				DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::eFilterLog::ENGINE);
			}
			else if (command == "-useGameLog")
			{
				DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::eFilterLog::GAME);
			}
			else if (command == "-useResourceLog")
			{
				DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::eFilterLog::RESOURCE);
			}
			else if (command == "-useDirectXLog")
			{
				DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::eFilterLog::DIRECTX);
			}
			else if (command == "-useFBXLog")
			{
				DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::eFilterLog::FBX);
			}
			else if (command == "-useFunctionTimerLog")
			{
				DL_Debug::Debug::GetInstance()->ActivateFilterLog(DL_Debug::eFilterLog::FUNCTION_TIMER);
			}
		}

		LocalFree(realCommands);
	}

	Easy3D::SetupInfo setup;
	ReadSetup(setup, "Data/bin/config.bin");

	HWND hwnd;

	if (Easy3D::Engine::Create(hwnd, WndProc, setup) == false)
	{
		return 1;
	}

	globalGame = new Game();
	OnResize();
	if (globalGame->Init(hwnd) == false)
	{
		return 1;
	}


	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			if (globalGame->Update() == false)
			{
				break;
			}


			Easy3D::Engine::GetInstance()->Render();
		}
	}

	globalGame->Destroy();
	delete globalGame;
	globalGame = nullptr;

	Easy3D::Engine::GetInstance()->Shutdown();
	Easy3D::Engine::Destroy();
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		if (globalGame != nullptr)
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				globalGame->Pause();
			}
			else
			{
				globalGame->UnPause();
			}
		}
		break;
	case WM_SIZE:
	{
		globalClientWidth = LOWORD(lParam);
		globalClientHeight = HIWORD(lParam);

		if (LOWORD(wParam) == SIZE_MINIMIZED)
		{
			if (globalGame != nullptr)
			{
				globalGame->Pause();
			}
		}
		else if (LOWORD(wParam) == SIZE_MAXIMIZED)
		{
			if (globalGame != nullptr)
			{
				globalGame->UnPause();
				OnResize();
			}
		}
		else if (LOWORD(wParam) == SIZE_RESTORED)
		{
			if (globalGame != nullptr && globalIsResizing == false)
			{
				globalGame->UnPause();
				OnResize();
			}
		}
		break;
	}
	case WM_ENTERSIZEMOVE:
		if (globalGame != nullptr)
		{
			globalGame->Pause();
			globalIsResizing = true;
		}
		break;
	case WM_EXITSIZEMOVE:
		if (globalGame != nullptr)
		{
			globalGame->UnPause();
			globalIsResizing = false;
			OnResize();
			
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void OnResize()
{
	globalGame->OnResize(globalClientWidth, globalClientHeight);
	Easy3D::Engine::GetInstance()->OnResize(globalClientWidth, globalClientHeight);
}

void ReadSetup(Easy3D::SetupInfo& aSetup, const std::string& aFilePath)
{
	int width = 800;
	int height = 600;
	int msaa = 4;
	int windowed = 1;

	std::ifstream file;
	file.open(aFilePath, std::ios::binary | std::ios::in);
	if (file.is_open() == true)
	{
		char buffer[4];

		file.read(buffer, 4);
		width = *(reinterpret_cast<int*>(buffer));

		file.read(buffer, 4);
		height = *(reinterpret_cast<int*>(buffer));

		file.read(buffer, 4);
		msaa = *(reinterpret_cast<int*>(buffer));

		file.read(buffer, 4);
		windowed = *(reinterpret_cast<int*>(buffer));
	}

	aSetup.myScreenWidth = width;
	aSetup.myScreenHeight = height;
	aSetup.myMSAACount = msaa;

	if (windowed == 1)
		aSetup.myWindowed = true;
	else
		aSetup.myWindowed = false;
}