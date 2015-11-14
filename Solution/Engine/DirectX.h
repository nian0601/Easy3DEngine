#pragma once

#include <D3D11.h>
#include "D3DPointer.h"
#include "Enums.h"
#include "SetupInfo.h"

struct D3D11_BLEND_DESC;
struct D3D11_RASTERIZER_DESC;
struct D3D11_RENDER_TARGET_VIEW_DESC;
struct D3D11_VIEWPORT;

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
struct ID3D11Resource;
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

		void SetDebugName(ID3D11DeviceChild* aChild, const std::string& aName);

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContex();
		ID3D11DepthStencilView* GetDepthStencilView();
		ID3D11RenderTargetView* GetBackbuffer();

		void SetDepthBufferState(eDepthStencil aState);
		void SetRasterizeState(eRasterizer aState);
		void SetBlendState(eBlendState aState);
		eDepthStencil GetDepthBufferState() const;
		eRasterizer GetRasterizerState() const;
		eBlendState GetBlendState() const;

		void RestoreViewPort();


		void CreateRenderTargetView(const std::string& aDebugName, ID3D11Resource* aResource
			, const D3D11_RENDER_TARGET_VIEW_DESC* aDesc, D3DPointer<ID3D11RenderTargetView>& aOutPointer);
		void CreateBuffer(const std::string& aDebugName, const D3D11_BUFFER_DESC* aDesc
			, const D3D11_SUBRESOURCE_DATA* aInitData, D3DPointer<ID3D11Buffer>& aOutPointer);
		void CreateTexture2D(const std::string& aDebugName, const D3D11_TEXTURE2D_DESC* aDesc
			, const D3D11_SUBRESOURCE_DATA* aInitData, D3DPointer<ID3D11Texture2D>& aOutPointer);
		void CreateDepthStencilView(const std::string& aDebugName, const D3DPointer<ID3D11Texture2D>& aTexture
			, const D3D11_DEPTH_STENCIL_VIEW_DESC* aDesc, D3DPointer<ID3D11DepthStencilView>& aOutPointer);
		void CreateDepthStencilState(const std::string& aDebugName, const D3D11_DEPTH_STENCIL_DESC* aDesc
			, D3DPointer<ID3D11DepthStencilState>& aOutPointer);
		void CreateRasterizerState(const std::string& aDebugName, const D3D11_RASTERIZER_DESC* aDesc
			, D3DPointer<ID3D11RasterizerState>& aOutPointer);
		void CreateBlendState(const std::string& aDebugName, const D3D11_BLEND_DESC* aDesc
			, D3DPointer<ID3D11BlendState >& aOutPointer);

	private:
		void operator=(const DirectX&) = delete;

		bool D3DSetup();
		bool D3DSwapChainSetup();
		bool D3DRenderTargetSetup();
		bool D3DViewPortSetup(int aWidth, int aHeight);
		bool D3DStencilBufferSetup(int aWidth, int aHeight);
		bool D3DDepthStencilStatesSetup();
		bool D3DRasterizerStateSetup();
		bool D3DSetupBlendStates();

		template <typename T>
		void AssignPointer(T* aDirectXObject, D3DPointer<T>& aD3DPointer, const std::string& aDebugName);


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

		eDepthStencil myDepthBufferState;
		eRasterizer myRasterizeState;
		eBlendState myBlendState;

		HWND& myHWND;
		SetupInfo mySetupInfo;
	};

	template <typename T>
	inline void DirectX::AssignPointer(T* aDirectXObject, D3DPointer<T>& aD3DPointer
		, const std::string& aDebugName)
	{
		aD3DPointer.Set(aDirectXObject);
		SetDebugName(aD3DPointer.Get(), aDebugName);
	}
}