#pragma once

#include "Enums.h"
#include <Matrix.h>
#include <Vector.h>
#include <string>
#include <thread>
#include <Windows.h>


struct ID3D11Device;
struct ID3D11DeviceChild;
struct ID3D11DeviceContext;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;

namespace Easy3D
{
	class Camera;
	class DirectX;
	class EffectContainer;
	class Effect3D;
	class EmitterContainer;
	class FBXFactory;
	class FileWatcher;
	class FontContainer;
	class Model;
	class ModelLoader;
	class ModelProxy;
	class TextureContainer;
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

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContex();
		ID3D11DepthStencilView* GetDepthStencilView();
		ID3D11RenderTargetView* GetBackbuffer();

		void SetDebugName(ID3D11DeviceChild* aChild, const std::string& aName);

		const CU::Vector2<int>& GetWindowSize() const;
		const CU::Matrix44<float>& GetOrthogonalMatrix() const;

		void PrintDebugText(const std::string& aText, const CU::Vector2<float>& aPosition, float aScale = 1.f);

		void SetDepthBufferState(eDepthStencil aState);
		void SetRasterizeState(eRasterizer aState);
		void SetBlendState(eBlendState aState);
		eDepthStencil GetDepthBufferState() const;
		eRasterizer GetRasterizerState() const;
		eBlendState GetBlendState() const;

		void RestoreViewPort();

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

		std::thread* myModelLoaderThread;

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