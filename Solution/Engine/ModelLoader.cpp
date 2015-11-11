#include "stdafx.h"

#include "Engine.h"
#include "EffectContainer.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include "FBXFactory.h"
#include <TimerManager.h>


#define THREADED_LOADING

namespace Easy3D
{
	ModelLoader* ModelLoader::myInstance = nullptr;

	void ModelLoader::Create()
	{
		myInstance = new ModelLoader();
	}

	void ModelLoader::Destroy()
	{
		delete myInstance;
	}

	ModelLoader* ModelLoader::GetInstance()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "ModelLoader: myInstance is nullptr, forgot to create?");
		return myInstance;
	}

	ModelLoader::ModelLoader()
		: myModelsToLoad(4)
		, myNonFXBModels(4)
		, myIsRunning(true)
		, myCanAddToLoadArray(true)
		, myCanCopyArray(true)
		, myModelFactory(new FBXFactory())
	{
	}

	ModelLoader::~ModelLoader()
	{
		myNonFXBModels.DeleteAll();
		delete myModelFactory;

		for (auto it = myProxies.begin(); it != myProxies.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}
	}

	void ModelLoader::Run()
	{
#ifndef THREADED_LOADING
		return;
#else
		while (myIsRunning == true)
		{
			WaitUntilCopyIsAllowed();
			myCanAddToLoadArray = false;

			if (myModelsToLoad.Size() == 0)
			{
				myCanAddToLoadArray = true;
				std::this_thread::yield();
				continue;
			}

			CU::GrowingArray<LoadData> loadArray;
			loadArray = myModelsToLoad;
			myModelsToLoad.RemoveAll();
			myCanAddToLoadArray = true;
			

			for (int i = 0; i < loadArray.Size(); ++i)
			{
				eLoadType loadType = loadArray[i].myLoadType;

				Model* model = nullptr;
				switch (loadType)
				{
				case Easy3D::ModelLoader::eLoadType::MODEL:
				{
					CU::TimerManager::GetInstance()->StartTimer("LoadModel");

					model = myModelFactory->LoadModel(loadArray[i].myModelPath.c_str(),
						EffectContainer::GetInstance()->Get3DEffect(loadArray[i].myEffectPath));
					model->Init();


					int elapsed = static_cast<int>(
						CU::TimerManager::GetInstance()->StopTimer("LoadModel").GetMilliseconds());
					RESOURCE_LOG("Model \"%s\" took %d ms to load", loadArray[i].myModelPath.c_str(), elapsed);
					break;
				}
				case Easy3D::ModelLoader::eLoadType::CUBE:
				{
					model = new Easy3D::Model();
					model->InitCube(loadArray[i].mySize.x, loadArray[i].mySize.y,
						loadArray[i].mySize.z, loadArray[i].myColor);

					myNonFXBModels.Add(model);
					break;
				}
				default:
					DL_ASSERT("ModelLoader tried to load something that dint have a specified LoadType!!!");
					break;
				}

				if (model == nullptr)
				{
					DL_MESSAGE_BOX("Failed to Load model", "ModelLoader->Error", MB_ICONWARNING);
				}

				loadArray[i].myProxy->SetModel(model);
			}
		}
#endif
	}

	void ModelLoader::Shutdown()
	{
		myIsRunning = false;
	}

	ModelProxy* ModelLoader::LoadModel(const std::string& aModelPath, const std::string& aEffectPath)
	{
#ifdef THREADED_LOADING
		WaitUntilAddIsAllowed();


		if(myProxies.find(aModelPath) != myProxies.end())
		{
			return myProxies[aModelPath];
		}

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();
		proxy->SetModel(nullptr);

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::MODEL;
		newData.myModelPath = aModelPath;
		newData.myEffectPath = aEffectPath;

		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		myProxies[aModelPath] = proxy;
		return proxy;
#else

		if (myProxies.find(aModelPath) != myProxies.end())
		{
			return myProxies[aModelPath];
		}

		ModelProxy* proxy = new ModelProxy();
		
		CU::TimerManager::GetInstance()->StartTimer("LoadModel");

		Model* model = myModelFactory->LoadModel(aModelPath.c_str(),
			EffectContainer::GetInstance()->GetEffect(aEffectPath));
		model->Init();

		int elapsed = static_cast<int>(
			CU::TimerManager::GetInstance()->StopTimer("LoadModel").GetMilliseconds());
		RESOURCE_LOG("Model \"%s\" took %d ms to load", aModelPath.c_str(), elapsed);

		proxy->SetModel(model);

		myProxies[aModelPath] = proxy;
		return proxy;
#endif
		
	}

	ModelProxy* ModelLoader::LoadCube(float aWidth, float aHeight, float aDepth
		, CU::Vector4f aColor)
	{
#ifdef THREADED_LOADING
		WaitUntilAddIsAllowed();

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();
		proxy->SetModel(nullptr);

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::CUBE;
		newData.mySize = { aWidth, aHeight, aDepth };
		newData.myColor = aColor;

		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
#else
		ModelProxy* proxy = new ModelProxy();
		Model* model = new Easy3D::Model();
		model->InitLightCube(aWidth, aHeight, aDepth, aColor);

		proxy->SetModel(model);

		return proxy;
#endif	
	}

	void ModelLoader::WaitUntilCopyIsAllowed()
	{
		while (myCanCopyArray == false)
			; //Should be an empty whileloop!
	}

	void ModelLoader::WaitUntilAddIsAllowed()
	{
		while (myCanAddToLoadArray == false)
			; //Should be an empty whileloop!
	}

}