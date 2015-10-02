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
						Engine::GetInstance()->GetEffectContainer()->GetEffect(loadArray[i].myEffectPath));
					model->Init();


					int elapsed = static_cast<int>(
						CU::TimerManager::GetInstance()->StopTimer("LoadModel").GetMilliseconds());
					RESOURCE_LOG("Model \"%s\" took %d ms to load", loadArray[i].myModelPath.c_str(), elapsed);
					break;
				}
				case Easy3D::ModelLoader::eLoadType::POLYGON:
				{
					model = new Easy3D::Model();
					model->InitPolygon();

					myNonFXBModels.Add(model);
					break;
				}
				case Easy3D::ModelLoader::eLoadType::CUBE:
				{
					model = new Easy3D::Model();
					model->InitCube(loadArray[i].mySize.x, loadArray[i].mySize.y, loadArray[i].mySize.z);

					myNonFXBModels.Add(model);
					break;
				}
				case Easy3D::ModelLoader::eLoadType::LIGHT_CUBE:
				{
					model = new Easy3D::Model();
					model->InitLightCube(loadArray[i].mySize.x, loadArray[i].mySize.y,
						loadArray[i].mySize.z, loadArray[i].myColor);

					myNonFXBModels.Add(model);
					break;
				}
				case Easy3D::ModelLoader::eLoadType::GEOMETRY:
				{
					model = new Easy3D::Model();
					model->InitGeometry(loadArray[i].myMeshData);

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

		return proxy;
#else
		ModelProxy* proxy = new ModelProxy();
		
		CU::TimerManager::GetInstance()->StartTimer("LoadModel");

		Model* model = myModelFactory->LoadModel(aModelPath.c_str(),
			Engine::GetInstance()->GetEffectContainer()->GetEffect(aEffectPath));
		model->Init();

		int elapsed = static_cast<int>(
			CU::TimerManager::GetInstance()->StopTimer("LoadModel").GetMilliseconds());
		RESOURCE_LOG("Model \"%s\" took %d ms to load", aModelPath.c_str(), elapsed);

		proxy->SetModel(model);

		return proxy;
#endif
		
	}

	ModelProxy* ModelLoader::LoadPolygon()
	{

#ifdef THREADED_LOADING
		WaitUntilAddIsAllowed();

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();
		proxy->SetModel(nullptr);

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::POLYGON;


		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
#else
		ModelProxy* proxy = new ModelProxy();
		Model* model = new Easy3D::Model();
		model->InitPolygon();

		proxy->SetModel(model);

		return proxy;
#endif
		
	}

	ModelProxy* ModelLoader::LoadCube(float aWidth, float aHeight, float aDepth)
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

		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
#else
		ModelProxy* proxy = new ModelProxy();
		Model* model = new Easy3D::Model();
		model->InitCube(aWidth, aHeight, aDepth);

		proxy->SetModel(model);

		return proxy;
#endif
	}

	ModelProxy* ModelLoader::LoadLightCube(float aWidth, float aHeight, float aDepth
		, CU::Vector4f aColour)
	{
#ifdef THREADED_LOADING
		WaitUntilAddIsAllowed();

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();
		proxy->SetModel(nullptr);

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::LIGHT_CUBE;
		newData.mySize = { aWidth, aHeight, aDepth };
		newData.myColor = aColour;

		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
#else
		ModelProxy* proxy = new ModelProxy();
		Model* model = new Easy3D::Model();
		model->InitLightCube(aWidth, aHeight, aDepth, aColour);

		proxy->SetModel(model);

		return proxy;
#endif	
	}

	ModelProxy* ModelLoader::LoadGeometry(const MeshData& aMeshData)
	{
#ifdef THREADED_LOADING
		WaitUntilAddIsAllowed();

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::GEOMETRY;
		newData.myMeshData = aMeshData;

		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
#else
		ModelProxy* proxy = new ModelProxy();
		Model* model = new Easy3D::Model();
		model->InitGeometry(aMeshData);

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