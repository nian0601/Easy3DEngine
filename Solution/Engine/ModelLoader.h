#pragma once

#include <GrowingArray.h>
#include <atomic>

namespace Easy3D
{
	class Model;
	class ModelProxy;
	class FBXFactory;

	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader();

		void Run();
		void Shutdown();

		ModelProxy* LoadModel(const std::string& aModelPath, const std::string& aEffectPath);
		ModelProxy* LoadCube(float aWidth = 1.f, float aHeight = 1.f, float aDepth = 1.f
			, CU::Vector4f aColor = { 1.f, 1.f, 1.f, 1.f });

	private:
		enum class eLoadType
		{
			MODEL,
			CUBE,
		};
		struct LoadData
		{
			ModelProxy* myProxy;
			eLoadType myLoadType;
			std::string myModelPath = "";
			std::string myEffectPath = "";
			CU::Vector3<float> mySize;
			CU::Vector4<float> myColor;
		};

		void WaitUntilCopyIsAllowed();
		void WaitUntilAddIsAllowed();

		CU::GrowingArray<LoadData> myModelsToLoad;
		volatile bool myCanAddToLoadArray;
		volatile bool myCanCopyArray;
		volatile bool myIsRunning;

		FBXFactory* myModelFactory;
		CU::GrowingArray<Model*> myNonFXBModels;
		std::unordered_map<std::string, ModelProxy*> myProxies;
	};
}