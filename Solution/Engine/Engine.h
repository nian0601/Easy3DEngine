#pragma once

#include "Enums.h"
#include <Matrix.h>
#include <Vector.h>
#include <string>
#include <thread>
#include <Windows.h>


struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Easy3D
{
	class Camera;
	class DebugDataDisplay;
	class DirectX;
	class EffectContainer;
	class Effect3D;
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
		TextureContainer* GetTextureContainer();
		EffectContainer* GetEffectContainer();
		FontContainer* GetFontContainer();
		DebugDataDisplay* GetDebugDisplay();
		FileWatcher* GetFileWatcher();
		ModelLoader* GetModelLoader();

		const CU::Vector2<int>& GetWindowSize() const;
		const CU::Matrix44<float>& GetOrthogonalMatrix() const;

		void PrintDebugText(const std::string& aText, const CU::Vector2<float>& aPosition, float aScale = 1.f);

		void SetDepthBufferState(eDepthStencilType aState);

		void ToggleWireframe();

		void EnableWireframe();
		void DisableWireframe();

		void EnableAlphaBlending();
		void DisableAlphaBlending();


		void SetClearColor(const CU::Vector4<float>& aClearColor);

		bool myWireframeShouldShow;

	private:
		Engine();
		~Engine();
		bool Init(HWND& aHwnd, WNDPROC aWndProc);
		bool WindowSetup(HWND& aHwnd, WNDPROC aWindowProc);
		
		bool myWireframeIsOn;


		DirectX* myDirectX;
		SetupInfo* mySetupInfo;
		TextureContainer* myTextureContainer;
		EffectContainer* myEffectContainer;
		FontContainer* myFontContainer;
		DebugDataDisplay* myDebugDataDisplay;
		FileWatcher* myFileWatcher;

		Text* myDebugText;

		CU::Vector4<float> myClearColor;
		CU::Vector2<int> myWindowSize;
		CU::Matrix44<float> myOrthogonalMatrix;


		ModelLoader* myModelLoader;
		std::thread* myModelLoaderThread;

		static Engine* myInstance;
	};
}

inline Easy3D::TextureContainer* Easy3D::Engine::GetTextureContainer()
{
	return myTextureContainer;
}

inline Easy3D::EffectContainer* Easy3D::Engine::GetEffectContainer()
{
	return myEffectContainer;
}

inline Easy3D::FontContainer* Easy3D::Engine::GetFontContainer()
{
	return myFontContainer;
}

inline Easy3D::DebugDataDisplay* Easy3D::Engine::GetDebugDisplay()
{
	return myDebugDataDisplay;
}

inline Easy3D::FileWatcher* Easy3D::Engine::GetFileWatcher()
{
	return myFileWatcher;
}

inline Easy3D::ModelLoader* Easy3D::Engine::GetModelLoader()
{
	return myModelLoader;
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