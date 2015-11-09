#pragma once

#include <D3D11.h>
#include "D3DPointer.h"
#include "Enums.h"
#include "SetupInfo.h"

struct ID3D11Debug;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11Device;
struct ID3D11DeviceChild;
struct ID3D11DeviceContext;
struct ID3D11InfoQueue;
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

		void SetDebugName(ID3D11DeviceChild* aChild, const std::string& aName);

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


		D3DPointer<ID3D11Device> myDevice;
		D3DPointer<ID3D11DeviceContext> myContext;
		D3DPointer<ID3D11Debug> myDebugInterface;
		D3DPointer<ID3D11InfoQueue> myInfoQueue;
		D3DPointer<IDXGISwapChain> mySwapChain;
		D3DPointer<ID3D11RenderTargetView> myRenderTargetView;
		D3DPointer<ID3D11DepthStencilView> myDepthBufferView;
		D3DPointer<ID3D11DepthStencilState> myDepthStencilStates[3];
		D3DPointer<ID3D11Texture2D> myDepthBuffer;
		D3DPointer<ID3D11RasterizerState> mySolidRasterizer;
		D3DPointer<ID3D11RasterizerState> myWireframeRasterizer;
		D3DPointer<ID3D11RasterizerState> myNoCullingRasterizer;
		D3DPointer<ID3D11BlendState> myAlphaBlendState;
		D3DPointer<ID3D11BlendState> myNoAlphaBlendState;
		D3D11_VIEWPORT* myViewPort;


		HWND& myHWND;
		SetupInfo mySetupInfo;
	};
}