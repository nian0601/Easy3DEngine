#include "stdafx.h"

#include "DebugDataDisplay.h"
#include "DirectionalLight.h"
#include "Instance.h"
#include "ParticleEmitterInstance.h"
#include "PointLight.h"
#include "Scene.h"
#include "SpotLight.h"

Easy3D::Scene::Scene()
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

Easy3D::Scene::~Scene()
{
	myInstances.DeleteAll();
}

void Easy3D::Scene::Render()
{
	TIME_FUNCTION;;

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

	Engine::GetInstance()->SetDepthBufferState(eDepthStencilType::PARTICLES);
	Engine::GetInstance()->EnableAlphaBlending();
	Engine::GetInstance()->SetRasterizeState(eRasterizerType::NO_CULLING);
	
	for (int i = 0; i < myEmitterInstances.Size(); ++i)
	{
		myEmitterInstances[i]->Render(*myCamera);
	}

	Engine::GetInstance()->SetRasterizeState(eRasterizerType::CULL_FRONT);
	Engine::GetInstance()->DisableAlphaBlending();
	Engine::GetInstance()->SetDepthBufferState(eDepthStencilType::Z_ENABLED);

}

void Easy3D::Scene::AddInstance(Instance* aInstance)
{
	myInstances.Add(aInstance);
}

void Easy3D::Scene::AddInstance(ParticleEmitterInstance* aInstance)
{
	myEmitterInstances.Add(aInstance);
}

void Easy3D::Scene::AddLight(DirectionalLight* aLight)
{
	myDirectionalLights.Add(aLight);
}

void Easy3D::Scene::AddLight(PointLight* aLight)
{
	myPointLights.Add(aLight);
}

void Easy3D::Scene::AddLight(SpotLight* aLight)
{
	mySpotLights.Add(aLight);
}

void Easy3D::Scene::RemoveInstance(Instance* aInstance) 
{
	myInstances.RemoveCyclic(aInstance);
}

void Easy3D::Scene::SetCamera(Camera* aCamera)
{
	myCamera = aCamera;
}

Easy3D::Camera& Easy3D::Scene::GetCamera()
{
	return *myCamera;
}