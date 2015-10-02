#pragma once
#include "SetupInfo.h"

struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RasterizerState;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct IDXGISwapChain;
struct SetupInfo;


namespace Easy3D
{
	class DirectX
	{
	public:
		DirectX(HWND& aHwnd, SetupInfo& aSetupInfo);

		void Present(const unsigned int aSyncInterval, const unsigned int aFlags);
		void Clear(const float aClearColor[4]);

		void OnResize(int aWidth, int aHeigth);
		void CleanD3D();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContex();

		void EnableZBuffer();
		void DisableZBuffer();

		void EnableWireframe();
		void DisableWireframe();

	private:
		void operator=(const DirectX&) = delete;

		bool D3DSetup();
		bool D3DSwapChainSetup();
		bool D3DRenderTargetSetup();
		bool D3DViewPortSetup(int aWidth, int aHeight);
		bool D3DStencilBufferSetup(int aWidth, int aHeight);
		bool D3DEnabledStencilStateSetup();
		bool D3DDisabledStencilStateSetup();
		bool D3DWireframeRasterizerStateSetup();
		bool D3DSolidRasterizerStateSetup();



		ID3D11Device* myDevice;
		ID3D11DeviceContext* myContext;
		IDXGISwapChain* mySwapChain;
		ID3D11RenderTargetView* myRenderTargetView;
		ID3D11DepthStencilView* myDepthBufferView;
		ID3D11DepthStencilState* myEnabledDepthStencilState;
		ID3D11DepthStencilState* myDisabledDepthStencilState;
		ID3D11Texture2D* myDepthBuffer;
		ID3D11RasterizerState* mySolidRasterizer;
		ID3D11RasterizerState* myWireframeRasterizer;

		HWND& myHWND;
		SetupInfo mySetupInfo;
	};
}

inline ID3D11Device* Easy3D::DirectX::GetDevice()
{
	DL_ASSERT_EXP(myDevice != nullptr, "DirectX: myDevice is nullptr, HOW?!");
	return myDevice;
}

inline ID3D11DeviceContext* Easy3D::DirectX::GetContex()
{
	DL_ASSERT_EXP(myContext != nullptr, "DirectX: myContex is nullptr, HOW?!");
	return myContext;
}