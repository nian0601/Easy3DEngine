#pragma once

#include <GrowingArray.h>
#include "D3DPointers.h"
#include "Enums.h"
#include <Matrix.h>
#include <Vector.h>
#include <string>
#include <thread>
#include <Windows.h>

struct D3D11_BLEND_DESC;
struct D3D11_BUFFER_DESC;
struct D3D11_DEPTH_STENCIL_DESC;
struct D3D11_DEPTH_STENCIL_VIEW_DESC;
struct D3D11_RENDER_TARGET_VIEW_DESC;
struct D3D11_RASTERIZER_DESC;
struct D3D11_SUBRESOURCE_DATA;
struct D3D11_TEXTURE2D_DESC;

struct ID3D11BlendState;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceChild;
struct ID3D11DeviceContext;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11RenderTargetView;
struct ID3D11Resource;
struct ID3D11Texture2D;


namespace Easy3D
{
	class DirectX;
	class Text;

	struct SetupInfo;

	class Engine
	{
	public:
		static bool Create(HWND& aHwnd, WNDPROC aWndProc, SetupInfo& aSetupInfo);
		static void Destroy();
		static Engine* GetInstance();

		void Shutdown();
		void Render();
		void OnResize(int aWidth, int aHeigth);

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


		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContex();
		ID3D11DepthStencilView* GetDepthStencilView();
		ID3D11RenderTargetView* GetBackbuffer();

		void SetDepthBufferState(eDepthStencil aState);
		eDepthStencil GetDepthBufferState() const;

		void SetRasterizeState(eRasterizer aState);
		eRasterizer GetRasterizerState() const;

		void SetBlendState(eBlendState aState);
		eBlendState GetBlendState() const;

		void SetDebugName(ID3D11DeviceChild* aChild, const std::string& aName);
		

		void RestoreViewPort();

		void PrintDebugText(const std::string& aText, const CU::Vector2<float>& aPosition, float aScale = 1.f);

		const CU::Vector2<int>& GetWindowSize() const;
		const CU::Matrix44<float>& GetOrthogonalMatrix() const;
		void SetClearColor(const CU::Vector4<float>& aClearColor);

	private:
		Engine();
		~Engine();
		bool Init(HWND& aHwnd, WNDPROC aWndProc);
		bool WindowSetup(HWND& aHwnd, WNDPROC aWindowProc);

		DirectX* myDirectX;
		SetupInfo* mySetupInfo;

		Text* myDebugText;

		CU::Vector4<float> myClearColor;
		CU::Vector2<int> myWindowSize;
		CU::Matrix44<float> myOrthogonalMatrix;

		struct DebugText
		{
			DebugText(){}
			DebugText(const std::string& aText, const CU::Vector2<float>& aPosition)
				: myText(aText), myPosition(aPosition) {}

			std::string myText;
			CU::Vector2<float> myPosition;
		};
		CU::GrowingArray<DebugText> myDebugTexts;

		static Engine* myInstance;
	};
}

inline const CU::Vector2<int>& Easy3D::Engine::GetWindowSize() const
{
	return myWindowSize;
}

inline const CU::Matrix44<float>& Easy3D::Engine::GetOrthogonalMatrix() const
{
	return myOrthogonalMatrix;
}

inline void Easy3D::Engine::SetClearColor(const CU::Vector4<float>& aClearColor)
{
	myClearColor = aClearColor;
}