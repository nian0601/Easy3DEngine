#include "stdafx.h"

#include "DirectionalLight.h"
#include "Instance.h"
#include "ParticleEmitterInstance.h"
#include "PointLight.h"
#include "Scene.h"
#include "SpotLight.h"

namespace Easy3D
{
	Scene::Scene()
		: myInstances(4)
		, myEmitterInstances(4)
		, myDirectionalLights(4)
		, myPointLights(4)
		, mySpotLights(4)
	{
		memset(&myDirectionalLightData[0], 0, sizeof(DirectionalLightData) * NUMBER_OF_DIRECTIONAL_LIGHTS);
		memset(&myPointLightData[0], 0, sizeof(PointLightData) * NUMBER_OF_POINT_LIGHTS);
		memset(&mySpotLightData[0], 0, sizeof(SpotLightData) * NUMBER_OF_SPOT_LIGHTS);
	}

	Scene::~Scene()
	{
		myInstances.DeleteAll();
	}

	void Scene::Render()
	{
		for (int i = 0; i < myDirectionalLights.Size(); ++i)
		{
			myDirectionalLights[i]->Update();

			myDirectionalLightData[i].myDirection = myDirectionalLights[i]->GetCurrentDir();
			myDirectionalLightData[i].myColor = myDirectionalLights[i]->GetColor();
		}

		for (int i = 0; i < myPointLights.Size(); ++i)
		{
			myPointLights[i]->Update();

			myPointLightData[i].myColor = myPointLights[i]->GetColor();
			myPointLightData[i].myPosition = myPointLights[i]->GetPosition();
			myPointLightData[i].myRange = myPointLights[i]->GetRange();
		}

		for (int i = 0; i < mySpotLights.Size(); ++i)
		{
			mySpotLights[i]->Update();

			mySpotLightData[i].myPosition = mySpotLights[i]->GetPosition();
			mySpotLightData[i].myDirection = mySpotLights[i]->GetDir();
			mySpotLightData[i].myColor = mySpotLights[i]->GetColor();
			mySpotLightData[i].myRange = mySpotLights[i]->GetRange();
			mySpotLightData[i].myCone = mySpotLights[i]->GetCone();
		}

		for (int i = 0; i < myInstances.Size(); ++i)
		{
			myInstances[i]->UpdateDirectionalLights(myDirectionalLightData);
			myInstances[i]->UpdatePointLights(myPointLightData);
			myInstances[i]->UpdateSpotLights(mySpotLightData);
			myInstances[i]->Render(*myCamera);
		}

		Engine::GetInstance()->SetDepthBufferState(eDepthStencil::PARTICLES);
		Engine::GetInstance()->SetBlendState(eBlendState::ALPHA);
		Engine::GetInstance()->SetRasterizeState(eRasterizer::NO_CULLING);

		for (int i = 0; i < myEmitterInstances.Size(); ++i)
		{
			myEmitterInstances[i]->Render(*myCamera);
		}

		Engine::GetInstance()->SetRasterizeState(eRasterizer::CULL_FRONT);
		Engine::GetInstance()->SetBlendState(eBlendState::NO_ALPHA);
		Engine::GetInstance()->SetDepthBufferState(eDepthStencil::Z_ENABLED);

	}

	void Scene::AddInstance(Instance* aInstance)
	{
		myInstances.Add(aInstance);
	}

	void Scene::AddInstance(ParticleEmitterInstance* aInstance)
	{
		myEmitterInstances.Add(aInstance);
	}

	void Scene::AddLight(DirectionalLight* aLight)
	{
		myDirectionalLights.Add(aLight);
	}

	void Scene::AddLight(PointLight* aLight)
	{
		myPointLights.Add(aLight);
	}

	void Scene::AddLight(SpotLight* aLight)
	{
		mySpotLights.Add(aLight);
	}

	void Scene::RemoveInstance(Instance* aInstance)
	{
		myInstances.RemoveCyclic(aInstance);
	}

	void Scene::SetCamera(Camera* aCamera)
	{
		myCamera = aCamera;
	}

	Camera& Scene::GetCamera()
	{
		return *myCamera;
	}
}