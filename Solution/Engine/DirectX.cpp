#include "stdafx.h"

#include "DirectX.h"


namespace Easy3D
{
	DirectX::DirectX(HWND& aHwnd, SetupInfo& aSetupInfo)
		: myHWND(aHwnd)
		, mySetupInfo(aSetupInfo)
	{
		myViewPort = new D3D11_VIEWPORT();
		D3DSetup();
	}

	void DirectX::Present(const unsigned int aSyncInterval, const unsigned int aFlags)
	{
		mySwapChain->Present(aSyncInterval, aFlags);

	}

	void DirectX::Clear(const float aClearColor[4])
	{
		ID3D11RenderTargetView* target = myRenderTargetView.Get();
		myContext->OMSetRenderTargets(1, &target, myDepthBufferView.Get());
		myContext->ClearRenderTargetView(target, aClearColor);
		myContext->ClearDepthStencilView(myDepthBufferView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DirectX::OnResize(const int aWidth, const int aHeight)
	{
		myContext->OMSetRenderTargets(0, NULL, NULL);
		myRenderTargetView->Release();
		myContext->Flush();

		HRESULT result = mySwapChain->ResizeBuffers(1, aWidth, aHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		if (FAILED(result) != S_OK)
		{
			DIRECTX_LOG("Failed to Resize SwapChain Buffers");
			DL_MESSAGE_BOX("Failed to Resize SwapChain Buffers", "DirectX: SwapChain", MB_ICONWARNING);
		}

		D3DRenderTargetSetup();
		D3DViewPortSetup(aWidth, aHeight);
		D3DStencilBufferSetup(aWidth, aHeight);
	}

	void DirectX::CleanD3D()
	{
		mySwapChain->SetFullscreenState(FALSE, NULL);

		/*
			Need to call Release manually here because ->Release on the directX-object
			before the context gets released to make the debug-output give correct data
			*/

		mySwapChain.Release();
		myDevice.Release();

		myRenderTargetView.Release();
		myDepthBufferView.Release();
		myDepthBuffer.Release();

		myNoCullingRasterizer.Release();
		myWireframeRasterizer.Release();
		mySolidRasterizer.Release();

		myAlphaBlendState.Release();
		myNoAlphaBlendState.Release();

		myDepthStencilStates[0].Release();
		myDepthStencilStates[1].Release();
		myDepthStencilStates[2].Release();

		myContext->ClearState();
		myContext->Flush();
		myContext.Release();

#ifdef _DEBUG
		myDebugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		myDebugInterface.Release();
#endif
	}

	void DirectX::SetDebugName(ID3D11DeviceChild* aChild, const std::string& aName)
	{
		if (aChild != nullptr)
		{
			aChild->SetPrivateData(WKPDID_D3DDebugObjectName, aName.size(), aName.c_str());
		}
	}

	ID3D11Device* DirectX::GetDevice()
	{
		DL_ASSERT_EXP(myDevice.Get() != nullptr, "DirectX: myDevice is nullptr, HOW?!");
		return myDevice.Get();
	}

	ID3D11DeviceContext* DirectX::GetContex()
	{
		DL_ASSERT_EXP(myContext.Get() != nullptr, "DirectX: myContex is nullptr, HOW?!");
		return myContext.Get();
	}

	ID3D11DepthStencilView*DirectX::GetDepthStencilView()
	{
		DL_ASSERT_EXP(myDepthBufferView.Get() != nullptr, "DirectX: myDepthBufferView is nullptr, HOW?!");
		return myDepthBufferView.Get();
	}

	ID3D11RenderTargetView* DirectX::GetBackbuffer()
	{
		DL_ASSERT_EXP(myRenderTargetView.Get() != nullptr, "DirectX: myRenderTargetView is nullptr, HOW?!");
		return myRenderTargetView.Get();
	}

	void DirectX::SetDepthBufferState(eDepthStencil aState)
	{
		myDepthBufferState = aState;
		switch (aState)
		{
		case eDepthStencil::Z_ENABLED:
			myContext->OMSetDepthStencilState(myDepthStencilStates[0].Get(), 1);
			break;
		case eDepthStencil::Z_DISABLED:
			myContext->OMSetDepthStencilState(myDepthStencilStates[1].Get(), 1);
			break;
		case eDepthStencil::PARTICLES:
			myContext->OMSetDepthStencilState(myDepthStencilStates[2].Get(), 0);
			break;
		}
	}

	void DirectX::SetRasterizeState(eRasterizer aState)
	{
		myRasterizeState = aState;
		switch (aState)
		{
		case eRasterizer::CULL_FRONT:
			myContext->RSSetState(mySolidRasterizer.Get());
			break;
		case eRasterizer::WIRE_FRAME:
			myContext->RSSetState(myWireframeRasterizer.Get());
			break;
		case eRasterizer::NO_CULLING:
			myContext->RSSetState(myNoCullingRasterizer.Get());
			break;
		default:
			break;
		}
	}

	void DirectX::SetBlendState(eBlendState aState)
	{
		myBlendState = aState;

		float blendFactor[4];
		blendFactor[0] = 0.f;
		blendFactor[1] = 0.f;
		blendFactor[2] = 0.f;
		blendFactor[3] = 0.f;

		switch (myBlendState)
		{
		case Easy3D::eBlendState::ALPHA:
			myContext->OMSetBlendState(myAlphaBlendState.Get(), blendFactor, 0xFFFFFFFF);
			break;
		case Easy3D::eBlendState::NO_ALPHA:
			myContext->OMSetBlendState(myNoAlphaBlendState.Get(), blendFactor, 0xFFFFFFFF);
			break;
		default:
			break;
		}
	}

	eDepthStencil DirectX::GetDepthBufferState() const
	{
		return myDepthBufferState;
	}

	eRasterizer DirectX::GetRasterizerState() const
	{
		return myRasterizeState;
	}

	eBlendState DirectX::GetBlendState() const
	{
		return myBlendState;
	}

	bool DirectX::D3DSetup()
	{
		DL_ASSERT_EXP(D3DSwapChainSetup()
			, "Failed to Setup DirectX SwapChain");
		DL_ASSERT_EXP(D3DRenderTargetSetup()
			, "Failed to Setup RenderTarget");
		DL_ASSERT_EXP(D3DViewPortSetup(mySetupInfo.myScreenWidth, mySetupInfo.myScreenHeight)
			, "Failed to Setup DirectX ViewPort");
		DL_ASSERT_EXP(D3DStencilBufferSetup(mySetupInfo.myScreenWidth, mySetupInfo.myScreenHeight)
			, "Failed to Setup DirectX Stencil Buffer");
		DL_ASSERT_EXP(D3DDepthStencilStatesSetup()
			, "Failed to Setup D3DDepthStencilStates");
		DL_ASSERT_EXP(D3DRasterizerStateSetup()
			, "Failed to Setup RasterizerStates");
		DL_ASSERT_EXP(D3DSetupBlendStates()
			, "Failed to Setup SolidRasterizerState");


		SetRasterizeState(eRasterizer::CULL_FRONT);
		SetDepthBufferState(eDepthStencil::Z_ENABLED);
		SetBlendState(eBlendState::NO_ALPHA);

		ID3D11RenderTargetView* target = myRenderTargetView.Get();
		myContext->OMSetRenderTargets(1, &target, NULL);

		DIRECTX_LOG("DirectX Setup Successful");
		return true;
	}

	bool DirectX::D3DRenderTargetSetup()
	{
		ID3D11Texture2D* backBuffer = nullptr;
		mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

		CreateRenderTargetView("DirectX::myRenderTargetView", backBuffer, NULL, myRenderTargetView);

		backBuffer->Release();

		return TRUE;
	}

	bool DirectX::D3DSwapChainSetup()
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = mySetupInfo.myScreenWidth;
		swapChainDesc.BufferDesc.Height = mySetupInfo.myScreenHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = myHWND;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = mySetupInfo.myWindowed;

		ID3D11Device* dev = nullptr;
		ID3D11DeviceContext* cont = nullptr;
		IDXGISwapChain* chain = nullptr;
		HRESULT result = D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
#ifdef RELEASE_BUILD
			NULL,
#else
			D3D11_CREATE_DEVICE_DEBUG,
#endif
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&chain,
			&dev,
			NULL,
			&cont);

		if (FAILED(result))
		{
			return false;
		}

		myDevice.Set(dev);
		myContext.Set(cont);
		mySwapChain.Set(chain);

		SetDebugName(myContext.Get(), "DirectX::myDevice");

#ifdef _DEBUG
		ID3D11Debug* debugInterface = nullptr;
		result = myDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&debugInterface);
		if (FAILED(result))
		{
			DL_ASSERT("[DirectX]: Failed to Query DebugInterface");
			return false;
		}

		myDebugInterface.Set(debugInterface);

		ID3D11InfoQueue* infoQueue = nullptr;
		if (FAILED(myDebugInterface->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&infoQueue)))
		{
			DL_ASSERT("[DirectX]: Failed to Query InfoQueue");
			return false;
		}

		myInfoQueue.Set(infoQueue);
		myInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		myInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
		myInfoQueue->Release();
#endif

		return TRUE;
	}

	bool DirectX::D3DViewPortSetup(int aWidth, int aHeight)
	{
		ZeroMemory(myViewPort, sizeof(D3D11_VIEWPORT));

		myViewPort->TopLeftX = 0;
		myViewPort->TopLeftY = 0;
		myViewPort->Width = static_cast<FLOAT>(aWidth);
		myViewPort->Height = static_cast<FLOAT>(aHeight);
		myViewPort->MinDepth = 0.f;
		myViewPort->MaxDepth = 1.f;

		myContext->RSSetViewports(1, myViewPort);

		return true;
	}

	bool DirectX::D3DStencilBufferSetup(int aWidth, int aHeight)
	{
		D3D11_TEXTURE2D_DESC depthBufferInfo;
		ZeroMemory(&depthBufferInfo, sizeof(depthBufferInfo));

		depthBufferInfo.Width = aWidth;
		depthBufferInfo.Height = aHeight;
		depthBufferInfo.MipLevels = 1;
		depthBufferInfo.ArraySize = 1;
		depthBufferInfo.Format = DXGI_FORMAT_D32_FLOAT;
		depthBufferInfo.SampleDesc.Count = 1;
		depthBufferInfo.Usage = D3D11_USAGE_DEFAULT;
		depthBufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		CreateTexture2D("DirectX::myDepthBuffer", &depthBufferInfo, NULL, myDepthBuffer);

		D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));

		stencilDesc.Format = depthBufferInfo.Format;
		stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		stencilDesc.Texture2D.MipSlice = 0;

		CreateDepthStencilView("DirectX::myDepthBufferView", myDepthBuffer, &stencilDesc
			, myDepthBufferView);

		return true;
	}

	bool DirectX::D3DDepthStencilStatesSetup()
	{
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		CreateDepthStencilState("DirectX::myDepthStencilStates[0]", &stencilDesc
			, myDepthStencilStates[0]);


		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		CreateDepthStencilState("DirectX::myDepthStencilStates[1]", &stencilDesc
			, myDepthStencilStates[1]);


		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		CreateDepthStencilState("DirectX::myDepthStencilStates[2]", &stencilDesc
			, myDepthStencilStates[2]);

		return true;
	}

	bool DirectX::D3DRasterizerStateSetup()
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = true;
		desc.DepthBias = false;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = false;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = false;
		desc.AntialiasedLineEnable = false;


		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_FRONT;
		CreateRasterizerState("DirectX::myWireframeRasterizer", &desc, myWireframeRasterizer);


		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_FRONT;
		CreateRasterizerState("DirectX::mySolidRasterizer", &desc, mySolidRasterizer);

	
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		CreateRasterizerState("DirectX::myNoCullingRasterizer", &desc, myNoCullingRasterizer);

		return true;
	}

	bool DirectX::D3DSetupBlendStates()
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;


		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		CreateBlendState("DirectX::myAlphaBlendState", &blendDesc, myAlphaBlendState);

		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		CreateBlendState("DirectX::myNoAlphaBlendState", &blendDesc, myNoAlphaBlendState);

		return true;
	}

	void DirectX::RestoreViewPort()
	{
		myContext->RSSetViewports(1, myViewPort);
	}

	void DirectX::CreateRenderTargetView(const std::string& aDebugName, ID3D11Resource* aResource
		, const D3D11_RENDER_TARGET_VIEW_DESC* aDesc, D3DPointer<ID3D11RenderTargetView>& aOutPointer)
	{
		ID3D11RenderTargetView* target = nullptr;
		HRESULT hr = myDevice->CreateRenderTargetView(aResource, aDesc, &target);

		DL_ASSERT_EXP(hr == S_OK, "Failed to CreateRenderTargetView");

		AssignPointer(target, aOutPointer, aDebugName);
	}

	void DirectX::CreateBuffer(const std::string& aDebugName, const D3D11_BUFFER_DESC* aDesc
		, const D3D11_SUBRESOURCE_DATA* aInitData, D3DPointer<ID3D11Buffer>& aOutPointer)
	{
		ID3D11Buffer* buf = nullptr;
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(aDesc, aInitData, &buf);

		DL_ASSERT_EXP(hr == S_OK, "Failed to CreateBuffer");

		AssignPointer(buf, aOutPointer, aDebugName);
	}

	void DirectX::CreateTexture2D(const std::string& aDebugName, const D3D11_TEXTURE2D_DESC* aDesc
		, const D3D11_SUBRESOURCE_DATA* aInitData, D3DPointer<ID3D11Texture2D>& aOutPointer)
	{
		ID3D11Texture2D* tex = nullptr;
		HRESULT hr = myDevice->CreateTexture2D(aDesc, aInitData, &tex);
		
		DL_ASSERT_EXP(hr == S_OK, "Failed to CreateTexture2D");

		AssignPointer(tex, aOutPointer, aDebugName);
	}

	void DirectX::CreateDepthStencilView(const std::string& aDebugName, const D3DPointer<ID3D11Texture2D>& aTexture
		, const D3D11_DEPTH_STENCIL_VIEW_DESC* aDesc, D3DPointer<ID3D11DepthStencilView>& aOutPointer)
	{
		ID3D11DepthStencilView* depth = nullptr;
		HRESULT hr = myDevice->CreateDepthStencilView(aTexture.Get(), aDesc, &depth);
		
		DL_ASSERT_EXP(hr == S_OK, "Failed to CreateDepthStencilView");

		AssignPointer(depth, aOutPointer, aDebugName);
	}

	void DirectX::CreateDepthStencilState(const std::string& aDebugName, const D3D11_DEPTH_STENCIL_DESC* aDesc
		, D3DPointer<ID3D11DepthStencilState>& aOutPointer)
	{
		ID3D11DepthStencilState* depth = nullptr;
		HRESULT hr = myDevice->CreateDepthStencilState(aDesc, &depth);
		
		DL_ASSERT_EXP(hr == S_OK, "Failed to CreateDepthStencilState");

		AssignPointer(depth, aOutPointer, aDebugName);
	}

	void DirectX::CreateRasterizerState(const std::string& aDebugName, const D3D11_RASTERIZER_DESC* aDesc
		, D3DPointer<ID3D11RasterizerState>& aOutPointer)
	{
		ID3D11RasterizerState* rast = nullptr;
		HRESULT hr = myDevice->CreateRasterizerState(aDesc, &rast);

		DL_ASSERT_EXP(hr == S_OK, "Failed to CreateRasterizerState");

		AssignPointer(rast, aOutPointer, aDebugName);
	}

	void DirectX::CreateBlendState(const std::string& aDebugName, const D3D11_BLEND_DESC* aDesc
		, D3DPointer<ID3D11BlendState >& aOutPointer)
	{
		ID3D11BlendState* blend = nullptr;
		HRESULT hr = myDevice->CreateBlendState(aDesc, &blend);
		
		DL_ASSERT_EXP(hr == S_OK, "Failed to CreateBlendState");

		AssignPointer(blend, aOutPointer, aDebugName);
	}
}