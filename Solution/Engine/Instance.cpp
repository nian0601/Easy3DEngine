#include "stdafx.h"

#include "Camera.h"
#include "Effect.h"
#include "EffectContainer.h"
#include "Instance.h"
#include "Model.h"
#include "ModelProxy.h"


Prism::Instance::Instance(ModelProxy& aModel)
	: myProxy(aModel)
	, myOrientationPointer(nullptr)
	, myScale({1,1,1})
{

}

Prism::Instance::~Instance()
{
	delete &myProxy;
}

void Prism::Instance::Render(Camera& aCamera)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));
		myProxy.GetEffect()->SetProjectionMatrix(aCamera.GetProjection());
		myProxy.GetEffect()->SetScaleVector(myScale);

		if (myOrientationPointer != nullptr)
		{
			myProxy.Render(*myOrientationPointer);
		}
		else
		{
			myProxy.Render(myOrientation);
		}
	}
}

void Prism::Instance::Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));
		myProxy.GetEffect()->SetProjectionMatrix(aCamera.GetProjection());
		myProxy.GetEffect()->SetScaleVector(myScale);

		if (myOrientationPointer != nullptr)
		{
			myProxy.Render(*myOrientationPointer * aParentMatrix);
		}
		else
		{
			myProxy.Render(myOrientation * aParentMatrix);
		}
	}
}

void Prism::Instance::SetPosition(const CU::Vector3<float>& aPosition)
{
	myPosition = aPosition;
	myOrientation.SetPos(aPosition);
}

CU::Vector3<float>& Prism::Instance::GetPosition()
{
	return myPosition;
}

CU::Matrix44<float>& Prism::Instance::GetOrientation()
{
	return myOrientation;
}

void Prism::Instance::SetOrientation(const CU::Matrix44<float>& aOrientation)
{
	myOrientation = aOrientation;
}

void Prism::Instance::SetEffect(const std::string& aEffectFile)
{
	if (myProxy.IsLoaded())
	{
		myProxy.SetEffect(Engine::GetInstance()->GetEffectContainer()->GetEffect(aEffectFile));
	}
}

void Prism::Instance::SetScale(const CU::Vector3<float>& aScaleVector)
{
	myScale = aScaleVector;
	
}

void Prism::Instance::PerformRotationLocal(CU::Matrix44<float>& aRotation)
{
	CU::Vector3<float> oldPos = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
	myOrientation = myOrientation * aRotation;
	myOrientation.SetPos(oldPos);
}

void Prism::Instance::PerformRotationWorld(CU::Matrix44<float>& aRotation)
{
	myOrientation = myOrientation * aRotation;
}

void Prism::Instance::PerformTransformation(CU::Matrix44<float>& aTransformation)
{
	myOrientation = myOrientation * aTransformation;
}

void Prism::Instance::UpdateDirectionalLights(
	const CU::StaticArray<DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->UpdateDirectionalLights(someDirectionalLightData);
	}
}

void Prism::Instance::UpdatePointLights(
	const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->UpdatePointLights(somePointLightData);
	}
}

void Prism::Instance::UpdateSpotLights(
	const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->UpdateSpotLights(someSpotLightData);
	}
}

void Prism::Instance::SetOrientationPointer(CU::Matrix44<float>& aOrientation)
{
	myOrientationPointer = &aOrientation;
}