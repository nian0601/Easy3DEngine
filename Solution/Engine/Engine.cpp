#include "stdafx.h"

#include "DirectX.h"
#include "EffectContainer.h"
#include "EmitterContainer.h"
#include "Engine.h"
#include "FBXFactory.h"
#include "FileWatcher.h"
#include "FontContainer.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include <TimerManager.h>
#include "Text.h"
#include "TextureContainer.h"
#include <Vector.h>
#include "VTuneApi.h"

namespace Easy3D
{
	Engine* Engine::myInstance = nullptr;

	bool Engine::Create(HWND& aHwnd, WNDPROC aWndProc, SetupInfo& aSetupInfo)
	{
		myInstance = new Engine();
		myInstance->mySetupInfo = &aSetupInfo;
		return myInstance->Init(aHwnd, aWndProc);
	}

	void Engine::Destroy()
	{
		delete myInstance;
		myInstance = nullptr;
	}

	Engine* Engine::GetInstance()
	{
		return myInstance;
	}


	void Engine::Shutdown()
	{
		ModelLoader::GetInstance()->Shutdown();
		myModelLoaderThread->join();

		myDirectX->CleanD3D();
		delete myDirectX;
		myDirectX = nullptr;
	}

	void Engine::Render()
	{
		VTUNE_EVENT_BEGIN(VTUNE::RENDER);

		myDirectX->Present(0, 0);
		float clearColor[4] = { myClearColor.myR, myClearColor.myG, myClearColor.myB, myClearColor.myA };
		myDirectX->Clear(clearColor);

		VTUNE_EVENT_END();
	}

	void Engine::OnResize(int aWidth, int aHeigth)
	{
		myWindowSize.x = aWidth;
		myWindowSize.y = aHeigth;
		myDirectX->OnResize(aWidth, aHeigth);

		myOrthogonalMatrix = CU::Matrix44<float>::CreateOrthogonalMatrixLH(static_cast<float>(myWindowSize.x)
			, static_cast<float>(myWindowSize.y), 0.1f, 1000.f);
	}


	void Engine::CreateRenderTargetView(const std::string& aDebugName, ID3D11Resource* aResource
		, const D3D11_RENDER_TARGET_VIEW_DESC* aDesc, D3DPointer<ID3D11RenderTargetView>& aOutPointer)
	{
		myDirectX->CreateRenderTargetView(aDebugName, aResource, aDesc, aOutPointer);
	}

	void Engine::CreateTexture2D(const std::string& aDebugName, const D3D11_TEXTURE2D_DESC* aDesc
		, const D3D11_SUBRESOURCE_DATA* aInitData, D3DPointer<ID3D11Texture2D>& aOutPointer)
	{
		myDirectX->CreateTexture2D(aDebugName, aDesc, aInitData, aOutPointer);
	}

	void Engine::CreateDepthStencilView(const std::string& aDebugName, const D3DPointer<ID3D11Texture2D>& aTexture
		, const D3D11_DEPTH_STENCIL_VIEW_DESC* aDesc, D3DPointer<ID3D11DepthStencilView>& aOutPointer)
	{
		myDirectX->CreateDepthStencilView(aDebugName, aTexture, aDesc, aOutPointer);
	}

	ID3D11Device* Engine::GetDevice()
	{
		return myDirectX->GetDevice();
	}

	ID3D11DeviceContext* Engine::GetContex()
	{
		return myDirectX->GetContex();
	}

	ID3D11DepthStencilView* Engine::GetDepthStencilView()
	{
		return myDirectX->GetDepthStencilView();
	}

	ID3D11RenderTargetView* Engine::GetBackbuffer()
	{
		return myDirectX->GetBackbuffer();
	}


	void Engine::SetDepthBufferState(eDepthStencil aState)
	{
		myDirectX->SetDepthBufferState(aState);
	}

	eDepthStencil Engine::GetDepthBufferState() const
	{
		return myDirectX->GetDepthBufferState();
	}

	void Engine::SetRasterizeState(eRasterizer aState)
	{
		myDirectX->SetRasterizeState(aState);
	}

	eRasterizer Engine::GetRasterizerState() const
	{
		return myDirectX->GetRasterizerState();
	}

	void Engine::SetBlendState(eBlendState aState)
	{
		myDirectX->SetBlendState(aState);
	}

	eBlendState Engine::GetBlendState() const
	{
		return myDirectX->GetBlendState();
	}


	void Engine::SetDebugName(ID3D11DeviceChild* aChild, const std::string& aName)
	{
		myDirectX->SetDebugName(aChild, aName);
	}

	void Engine::RestoreViewPort()
	{
		myDirectX->RestoreViewPort();
	}

	void Engine::PrintDebugText(const std::string& aText, const CU::Vector2<float>& aPosition, float aScale)
	{
		myDebugText->Render(aText.c_str(), aPosition.x, aPosition.y, aScale);
	}


	Engine::Engine() 
		: myClearColor({ 0.5f, 0.5f, 0.5f, 1.f })
	{
		TextureContainer::Create();
		EffectContainer::Create();
		FontContainer::Create();
		EmitterContainer::Create();
		FileWatcher::Create();
		ModelLoader::Create();
	}

	Engine::~Engine()
	{
		TextureContainer::Destroy();
		EffectContainer::Destroy();
		FontContainer::Destroy();
		EmitterContainer::Destroy();
		FileWatcher::Destroy();
		ModelLoader::Destroy();
	}

	bool Engine::Init(HWND& aHwnd, WNDPROC aWndProc)
	{
		myWindowSize.x = mySetupInfo->myScreenWidth;
		myWindowSize.y = mySetupInfo->myScreenHeight;

		if (WindowSetup(aHwnd, aWndProc) == false)
		{
			ENGINE_LOG("Failed to Create Window");
			return false;
		}

		myDirectX = new DirectX(aHwnd, *mySetupInfo);
		if (myDirectX == nullptr)
		{
			ENGINE_LOG("Failed to Setup DirectX");
			return false;
		}

		ShowWindow(aHwnd, 10);
		UpdateWindow(aHwnd);

		myDebugText = new Text();
		myDebugText->Init("Data/resources/font/font.dds");

		myOrthogonalMatrix = CU::Matrix44<float>::CreateOrthogonalMatrixLH(static_cast<float>(myWindowSize.x)
			, static_cast<float>(myWindowSize.y), 0.1f, 1000.f);

		myModelLoaderThread = new std::thread(&ModelLoader::Run, ModelLoader::GetInstance());

		ENGINE_LOG("Engine Init Successful");
		return true;
	}

	bool Engine::WindowSetup(HWND& aHwnd, WNDPROC aWindowProc)
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = aWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hIcon = LoadIcon(GetModuleHandle(NULL), NULL);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = "DirectX Window";
		wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

		if (RegisterClassEx(&wcex) == FALSE)
		{
			ENGINE_LOG("Failed to register WindowClass");
			return FALSE;
		}

		RECT rc = { 0, 0, mySetupInfo->myScreenWidth, mySetupInfo->myScreenHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		aHwnd = CreateWindow(
			"DirectX Window",
			"DirectX Window",
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL, 
			NULL,
			GetModuleHandle(NULL),
			NULL);

		if (!aHwnd)
		{
			ENGINE_LOG("Failed to CreateWindow");
			return FALSE;
		}

		ENGINE_LOG("Window Setup Successful");
		return TRUE;
	}
}