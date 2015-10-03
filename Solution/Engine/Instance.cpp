#include "stdafx.h"

#include "Camera.h"
#include "Effect3D.h"
#include "EffectContainer.h"
#include "Instance.h"
#include "Model.h"
#include "ModelProxy.h"


Easy3D::Instance::Instance(ModelProxy& aModel)
	: myProxy(aModel)
	, myOrientationPointer(nullptr)
	, myScale({1,1,1})
{

}

Easy3D::Instance::~Instance()
{
	delete &myProxy;
}

void Easy3D::Instance::Render(Camera& aCamera)
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

void Easy3D::Instance::Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera)
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

void Easy3D::Instance::SetPosition(const CU::Vector3<float>& aPosition)
{
	myPosition = aPosition;
	myOrientation.SetPos(aPosition);
}

CU::Vector3<float>& Easy3D::Instance::GetPosition()
{
	return myPosition;
}

CU::Matrix44<float>& Easy3D::Instance::GetOrientation()
{
	return myOrientation;
}

void Easy3D::Instance::SetOrientation(const CU::Matrix44<float>& aOrientation)
{
	myOrientation = aOrientation;
}

void Easy3D::Instance::SetEffect(const std::string& aEffectFile)
{
	if (myProxy.IsLoaded())
	{
		myProxy.SetEffect(Engine::GetInstance()->GetEffectContainer()->Get3DEffect(aEffectFile));
	}
}

void Easy3D::Instance::SetScale(const CU::Vector3<float>& aScaleVector)
{
	myScale = aScaleVector;
	
}

void Easy3D::Instance::PerformRotationLocal(CU::Matrix44<float>& aRotation)
{
	CU::Vector3<float> oldPos = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
	myOrientation = myOrientation * aRotation;
	myOrientation.SetPos(oldPos);
}

void Easy3D::Instance::PerformRotationWorld(CU::Matrix44<float>& aRotation)
{
	myOrientation = myOrientation * aRotation;
}

void Easy3D::Instance::PerformTransformation(CU::Matrix44<float>& aTransformation)
{
	myOrientation = myOrientation * aTransformation;
}

void Easy3D::Instance::UpdateDirectionalLights(
	const CU::StaticArray<DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->UpdateDirectionalLights(someDirectionalLightData);
	}
}

void Easy3D::Instance::UpdatePointLights(
	const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->UpdatePointLights(somePointLightData);
	}
}

void Easy3D::Instance::UpdateSpotLights(
	const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->UpdateSpotLights(someSpotLightData);
	}
}

void Easy3D::Instance::SetOrientationPointer(CU::Matrix44<float>& aOrientation)
{
	myOrientationPointer = &aOrientation;
}