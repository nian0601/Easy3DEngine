#pragma once

#include <atomic>
#include <GrowingArray.h>
#include "Thread.h"

namespace Easy3D
{
	class Model;
	class ModelProxy;
	class FBXFactory;

	class ModelLoader : public Thread
	{
	public:
		static void Create();
		static void Destroy();
		static ModelLoader* GetInstance();

		void Start() override;
		void Stop() override;
		void Pause() override;
		void UnPause() override;
		void WaitUuntilFinished() const;
		volatile bool IsLoading() const;

		void Run();

		ModelProxy* RequestModel(const std::string& aModelPath, const std::string& aEffectPath);
		ModelProxy* RequestCube(float aWidth = 1.f, float aHeight = 1.f, float aDepth = 1.f
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

		ModelLoader();
		~ModelLoader();

		void WaitUntilCopyIsAllowed();
		void WaitUntilAddIsAllowed();
		void CopyNewJobs();
		Model* LoadModel(const LoadData& aData);
		Model* LoadCube(const LoadData& aData);

		CU::GrowingArray<LoadData> myNewLoadJobs;
		CU::GrowingArray<LoadData> myCurrentLoadJobs;
		volatile bool myCanAddToLoadArray;
		volatile bool myCanCopyArray;
		volatile bool myIsLoading;

		FBXFactory* myModelFactory;
		CU::GrowingArray<Model*> myNonFXBModels;
		std::unordered_map<std::string, ModelProxy*> myProxies;

		static ModelLoader* myInstance;
	};
}