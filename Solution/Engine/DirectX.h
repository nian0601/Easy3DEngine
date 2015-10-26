#pragma once

#include "Enums.h"
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
struct D3D11_VIEWPORT;
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
		ID3D11DepthStencilView* GetDepthStencilView();
		ID3D11RenderTargetView* GetBackbuffer();

		void SetDepthBufferState(eDepthStencilType aState);
		void SetRasterizeState(eRasterizerType aState);

		void EnableAlphaBlending();
		void DisableAlpaBlending();

		void RestoreViewPort();

	private:
		void operator=(const DirectX&) = delete;

		bool D3DSetup();
		bool D3DSwapChainSetup();
		bool D3DRenderTargetSetup();
		bool D3DViewPortSetup(int aWidth, int aHeight);
		bool D3DStencilBufferSetup(int aWidth, int aHeight);
		bool D3DDepthStencilStatesSetup();
		bool D3DWireframeRasterizerStateSetup();
		bool D3DSolidRasterizerStateSetup();
		bool D3DNoCullingRasterizerStateSetup();
		bool D3DSetupBlendStates();



		ID3D11Device* myDevice;
		ID3D11DeviceContext* myContext;
		IDXGISwapChain* mySwapChain;
		ID3D11RenderTargetView* myRenderTargetView;
		ID3D11DepthStencilView* myDepthBufferView;
		ID3D11DepthStencilState* myDepthStencilStates[3];
		ID3D11Texture2D* myDepthBuffer;
		ID3D11RasterizerState* mySolidRasterizer;
		ID3D11RasterizerState* myWireframeRasterizer;
		ID3D11RasterizerState* myNoCullingRasterizer;
		ID3D11BlendState* myAlphaBlendState;
		ID3D11BlendState* myNoAlphaBlendState;
		D3D11_VIEWPORT* myViewPort;


		HWND& myHWND;
		SetupInfo mySetupInfo;
	};
}