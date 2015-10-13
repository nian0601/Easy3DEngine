#include "stdafx.h"
#include "DirectX.h"
#include <D3D11.h>



Easy3D::DirectX::DirectX(HWND& aHwnd, SetupInfo& aSetupInfo)
	: myHWND(aHwnd)
	, mySetupInfo(aSetupInfo)
{
	D3DSetup();
}

void Easy3D::DirectX::Present(const unsigned int aSyncInterval, const unsigned int aFlags)
{
	mySwapChain->Present(aSyncInterval, aFlags);
	
}

void Easy3D::DirectX::Clear(const float aClearColor[4])
{
	myContext->OMSetRenderTargets(1, &myRenderTargetView, myDepthBufferView);
	myContext->ClearRenderTargetView(myRenderTargetView, aClearColor);
	myContext->ClearDepthStencilView(myDepthBufferView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Easy3D::DirectX::OnResize(const int aWidth, const int aHeight)
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

void Easy3D::DirectX::CleanD3D()
{
	mySwapChain->SetFullscreenState(FALSE, NULL);

	mySwapChain->Release();
	mySwapChain = nullptr;

	myRenderTargetView->Release();
	myRenderTargetView = nullptr;

	myDevice->Release();
	myDevice = nullptr;

	myContext->Release();
	myContext = nullptr;
}

void Easy3D::DirectX::SetDepthBufferState(eDepthStencilType aState)
{
	switch (aState)
	{
	case Easy3D::eDepthStencilType::Z_ENABLED:
		myContext->OMSetDepthStencilState(myDepthStencilStates[0], 1);
		break;
	case Easy3D::eDepthStencilType::Z_DISABLED:
		myContext->OMSetDepthStencilState(myDepthStencilStates[1], 1);
		break;
	case Easy3D::eDepthStencilType::PARTICLES:
		myContext->OMSetDepthStencilState(myDepthStencilStates[2], 0);
		break;
	}
}

bool Easy3D::DirectX::D3DSetup()
{
	if (D3DSwapChainSetup() == false)
	{
		DIRECTX_LOG("Failed to Setup DirectX SwapChain");
		return false;
	}

	if (D3DRenderTargetSetup() == false)
	{
		DIRECTX_LOG("Failed to Setup RenderTarget");
		return false;
	}

	if (D3DViewPortSetup(mySetupInfo.myScreenWidth, mySetupInfo.myScreenHeight) == false)
	{
		DIRECTX_LOG("Failed to Setup DirectX ViewPort");
		return false;
	}

	if (D3DStencilBufferSetup(mySetupInfo.myScreenWidth, mySetupInfo.myScreenHeight) == false)
	{
		DIRECTX_LOG("Failed to Setup DirectX Stencil Buffer");
		return false;
	}

	if (D3DDepthStencilStatesSetup() == false)
	{
		DIRECTX_LOG("Failed to Setup D3DDepthStencilStates");
		return false;
	}

	if (D3DWireframeRasterizerStateSetup() == false)
	{
		DIRECTX_LOG("Failed to Setup WireframeRasterizerState");
		return false;
	}

	if (D3DSolidRasterizerStateSetup() == false)
	{
		DIRECTX_LOG("Failed to Setup SolidRasterizerState");
		return false;
	}

	if (D3DSetupBlendStates() == false)
	{
		DIRECTX_LOG("Failed to Setup SolidRasterizerState");
		return false;
	}

	DisableWireframe();
	SetDepthBufferState(eDepthStencilType::Z_ENABLED);
	
	DIRECTX_LOG("DirectX Setup Successful");
	return true;
}

bool Easy3D::DirectX::D3DRenderTargetSetup()
{
	ID3D11Texture2D* backBuffer = nullptr;
	mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	myDevice->CreateRenderTargetView(backBuffer, NULL, &myRenderTargetView);
	backBuffer->Release();

	myContext->OMSetRenderTargets(1, &myRenderTargetView, NULL);

	return TRUE;
}

bool Easy3D::DirectX::D3DSwapChainSetup()
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

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mySwapChain,
		&myDevice,
		NULL,
		&myContext);

	if (FAILED(result))
	{
		return false;
	}

	return TRUE;
}

bool Easy3D::DirectX::D3DViewPortSetup(int aWidth, int aHeight)
{
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));

	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = static_cast<FLOAT>(aWidth);
	viewPort.Height = static_cast<FLOAT>(aHeight);
	viewPort.MinDepth = 0.f;
	viewPort.MaxDepth = 1.f;

	myContext->RSSetViewports(1, &viewPort);

	return true;
}

bool Easy3D::DirectX::D3DStencilBufferSetup(int aWidth, int aHeight)
{
	HRESULT hr = S_OK;

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

	hr = myDevice->CreateTexture2D(&depthBufferInfo, NULL, &myDepthBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));

	stencilDesc.Format = depthBufferInfo.Format;
	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	stencilDesc.Texture2D.MipSlice = 0;

	hr = myDevice->CreateDepthStencilView(myDepthBuffer, &stencilDesc, &myDepthBufferView);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool Easy3D::DirectX::D3DDepthStencilStatesSetup()
{
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC  stencilDesc;

#pragma region EnabledState
	hr = S_OK;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));

	stencilDesc.DepthEnable = true;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
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

	hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[0]);
	if (FAILED(hr))
	{
		return false;
	}
#pragma endregion


#pragma region DisabledState
	hr = S_OK;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));

	stencilDesc.DepthEnable = false;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
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

	hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[1]);
	if (FAILED(hr))
	{
		return false;
	}
#pragma endregion


#pragma region ParticlesState
	hr = S_OK;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));

	stencilDesc.DepthEnable = true;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
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


	hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[2]);
	if (FAILED(hr))
	{
		return false;
	}
#pragma endregion

	return true;
}

bool Easy3D::DirectX::D3DWireframeRasterizerStateSetup()
{

	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_FRONT;
	desc.FrontCounterClockwise = true;
	desc.DepthBias = false;
	desc.DepthBiasClamp = 0;
	desc.SlopeScaledDepthBias = 0;
	desc.DepthClipEnable = false;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = false;
	desc.AntialiasedLineEnable = false;
	
	myDevice->CreateRasterizerState(&desc, &myWireframeRasterizer);

	return true;
}

bool Easy3D::DirectX::D3DSolidRasterizerStateSetup()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_FRONT;
	desc.FrontCounterClockwise = true;
	desc.DepthBias = false;
	desc.DepthBiasClamp = 0;
	desc.SlopeScaledDepthBias = 0;
	desc.DepthClipEnable = false;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = false;
	desc.AntialiasedLineEnable = false;

	myDevice->CreateRasterizerState(&desc, &mySolidRasterizer);

	return true;
}

bool Easy3D::DirectX::D3DSetupBlendStates()
{
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	HRESULT hr = myDevice->CreateBlendState(&blendDesc, &myAlphaBlendState);
	if (FAILED(hr) != S_OK)
	{
		DL_ASSERT("BaseModel::InitBlendState: Failed to CreateAlphaBlendState");
	}

	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = myDevice->CreateBlendState(&blendDesc, &myNoAlphaBlendState);
	if (FAILED(hr) != S_OK)
	{
		DL_ASSERT("BaseModel::InitBlendState: Failed to CreateNoAlphaBlendState");
	}

	return true;
}

void Easy3D::DirectX::EnableWireframe()
{
	myContext->RSSetState(myWireframeRasterizer);
}

void Easy3D::DirectX::DisableWireframe()
{
	myContext->RSSetState(mySolidRasterizer);
}

void Easy3D::DirectX::EnableAlphaBlending()
{
	float blendFactor[4];
	blendFactor[0] = 0.f;
	blendFactor[1] = 0.f;
	blendFactor[2] = 0.f;
	blendFactor[3] = 0.f;

	myContext->OMSetBlendState(myAlphaBlendState, blendFactor, 0xFFFFFFFF);
}

void Easy3D::DirectX::DisableAlpaBlending()
{
	float blendFactor[4];
	blendFactor[0] = 0.f;
	blendFactor[1] = 0.f;
	blendFactor[2] = 0.f;
	blendFactor[3] = 0.f;

	myContext->OMSetBlendState(myNoAlphaBlendState, blendFactor, 0xFFFFFFFF);
}
