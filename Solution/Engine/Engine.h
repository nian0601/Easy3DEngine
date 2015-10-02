#pragma once
#include <Matrix.h>
#include <Vector.h>
#include <string>
#include <thread>
#include <Windows.h>


struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Prism
{
	class Camera;
	class DebugDataDisplay;
	class DirectX;
	class EffectContainer;
	class Effect;
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

		Model* DLLLoadModel(const std::string& aModelPath, Effect* aEffect);

		const CU::Vector2<int>& GetWindowSize() const;
		const CU::Matrix44<float>& GetOrthogonalMatrix() const;

		void PrintDebugText(const std::string& aText, const CU::Vector2<float>& aPosition, float aScale = 1.f);

		void EnableZBuffer();
		void DisableZBuffer();

		void ToggleWireframe();

		void EnableWireframe();
		void DisableWireframe();


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
		FBXFactory* myModelFactory;
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

inline Prism::TextureContainer* Prism::Engine::GetTextureContainer()
{
	return myTextureContainer;
}

inline Prism::EffectContainer* Prism::Engine::GetEffectContainer()
{
	return myEffectContainer;
}

inline Prism::FontContainer* Prism::Engine::GetFontContainer()
{
	return myFontContainer;
}

inline Prism::DebugDataDisplay* Prism::Engine::GetDebugDisplay()
{
	return myDebugDataDisplay;
}

inline Prism::FileWatcher* Prism::Engine::GetFileWatcher()
{
	return myFileWatcher;
}

inline Prism::ModelLoader* Prism::Engine::GetModelLoader()
{
	return myModelLoader;
}

inline const CU::Vector2<int>& Prism::Engine::GetWindowSize() const
{
	return myWindowSize;
}

inline const CU::Matrix44<float>& Prism::Engine::GetOrthogonalMatrix() const
{
	return myOrthogonalMatrix;
}

inline void Prism::Engine::SetClearColor(const CU::Vector4<float>& aClearColor)
{
	myClearColor = aClearColor;
}