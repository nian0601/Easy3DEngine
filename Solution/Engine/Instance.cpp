#include "stdafx.h"

#include "Camera.h"
#include "Effect3D.h"
#include "EffectContainer.h"
#include "Instance.h"
#include "Model.h"
#include "ModelProxy.h"
#include "RenderProcessTarget.h"


Easy3D::Instance::Instance(ModelProxy& aModel, const CU::Matrix44<float>& aOrientation)
	: myProxy(aModel)
	, myOrientation(aOrientation)
	, myScale({1,1,1})
{

}

Easy3D::Instance::~Instance()
{
}

void Easy3D::Instance::Render(Camera& aCamera)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));
		myProxy.GetEffect()->SetProjectionMatrix(aCamera.GetProjection());
		myProxy.GetEffect()->SetScaleVector(myScale);

		myProxy.Render(myOrientation);
	}
}

void Easy3D::Instance::Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera)
{
	if (myProxy.IsLoaded())
	{
		myProxy.GetEffect()->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));
		myProxy.GetEffect()->SetProjectionMatrix(aCamera.GetProjection());
		myProxy.GetEffect()->SetScaleVector(myScale);

		myProxy.Render(myOrientation * aParentMatrix);
	}
}

void Easy3D::Instance::AddRenderData(RenderProcessTarget* aTarget)
{
	if (myProxy.IsLoaded())
	{
		aTarget->AddModel(myProxy.GetModel(), myOrientation);
	}
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