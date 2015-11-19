#include "stdafx.h"

#include "Engine.h"
#include "EffectContainer.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include "FBXFactory.h"
#include <TimerManager.h>
#include <thread>

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
		: Thread()
		, myNewLoadJobs(4)
		, myCurrentLoadJobs(4)
		, myNonFXBModels(4)
		, myCanAddToLoadArray(true)
		, myCanCopyArray(true)
		, myIsLoading(false)
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

	void ModelLoader::Start()
	{
		DL_ASSERT_EXP(myIsRunning == false, "Tried to start a thread thats allready running");
		DL_ASSERT_EXP(mySTDThread == nullptr, "Tried to start a thread before deleting the old");

		myIsRunning = true;
		myIsLoading = true;
		mySTDThread = new std::thread(&ModelLoader::Run, this);
	}

	void ModelLoader::Stop()
	{
		DL_ASSERT_EXP(mySTDThread != nullptr, "Tried to stop a nonexisting thread");
		DL_ASSERT_EXP(myIsRunning == true, "Tried to stop a thread that wasnt running");

		myIsRunning = false;
		mySTDThread->join();
		delete mySTDThread;
	}

	void ModelLoader::Pause()
	{
		DL_ASSERT_EXP(myIsPaused == true, "ModelLoader needs to unpaused to pause it");
		myIsPaused = true;
		while (myIsLoading == true)
		{
			//Wait
		}
	}

	void ModelLoader::UnPause()
	{
		DL_ASSERT_EXP(myIsPaused == true, "ModelLoader needs to paused to unpause it");
		myIsPaused = false;
		while (myIsLoading == false)
		{
			//Wait
		}
	}

	void ModelLoader::WaitUuntilFinished() const
	{
		while (myIsLoading == true)
		{
			//Wait
		}
	}

	volatile bool ModelLoader::IsLoading() const
	{
		return myIsLoading;
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

			if (myNewLoadJobs.Size() == 0)
			{
				myCanAddToLoadArray = true;
				std::this_thread::yield();
				continue;
			}

			CopyNewJobs();
			myNewLoadJobs.RemoveAll();
			myIsLoading = true;
			myCanAddToLoadArray = true;
			

			for (int i = myCurrentLoadJobs.Size()-1; i >= 0; --i)
			{
				if (myIsPaused == true)
				{
					myIsLoading = false;
					break;
				}

				if (myIsRunning == false)
				{
					return;
				}

				switch (myCurrentLoadJobs[i].myLoadType)
				{
				case Easy3D::ModelLoader::eLoadType::MODEL:
				{
					myCurrentLoadJobs[i].myProxy->SetModel(LoadModel(myCurrentLoadJobs[i]));
					break;
				}
				case Easy3D::ModelLoader::eLoadType::CUBE:
				{
					myCurrentLoadJobs[i].myProxy->SetModel(LoadCube(myCurrentLoadJobs[i]));
					break;
				}
				default:
					DL_ASSERT("ModelLoader tried to load something that dint have a specified LoadType!!!");
					break;
				}

				myCurrentLoadJobs.RemoveCyclicAtIndex(i);
			}
		}
#endif
	}

	ModelProxy* ModelLoader::RequestModel(const std::string& aModelPath, const std::string& aEffectPath)
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

		myNewLoadJobs.Add(newData);

		myCanCopyArray = true;

		myProxies[aModelPath] = proxy;
		return proxy;
#else
		if (myProxies.find(aModelPath) != myProxies.end())
		{
			return myProxies[aModelPath];
		}

		ModelProxy* proxy = new ModelProxy();
		
		LoadData data;
		data.myModelPath = aModelPath;
		data.myEffectPath = aEffectPath;

		Model* model = LoadModel(data);
		proxy->SetModel(model);

		myProxies[aModelPath] = proxy;
		return proxy;
#endif
		
	}

	ModelProxy* ModelLoader::RequestCube(float aWidth, float aHeight, float aDepth
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

		myNewLoadJobs.Add(newData);

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

	void ModelLoader::CopyNewJobs()
	{
		for (int i = 0; i < myNewLoadJobs.Size(); ++i)
		{
			myCurrentLoadJobs.Add(myNewLoadJobs[i]);
		}
	}

	Model* ModelLoader::LoadModel(const LoadData& aData)
	{
		Model* model = myModelFactory->LoadModel(aData.myModelPath.c_str(),
			EffectContainer::GetInstance()->Get3DEffect(aData.myEffectPath));

		return model;
	}

	Model* ModelLoader::LoadCube(const LoadData& aData)
	{
		Model* model = new Easy3D::Model();
		model->InitCube(aData.mySize.x, aData.mySize.y,
			aData.mySize.z, aData.myColor);

		myNonFXBModels.Add(model);
		return model;
	}

}