#include "stdafx.h"
#include "BaseEffect.h"

namespace Easy3D
{
	BaseEffect::BaseEffect()
		: myEffect(nullptr)
		, myTechnique(nullptr)
		, myEffectListeners(4)
	{
	}

	BaseEffect::~BaseEffect()
	{
	}

	void BaseEffect::SetBlendState(ID3D11BlendState* aBlendState, float aBlendFactor[4], const unsigned int aSampleMask)
	{
		Engine::GetInstance()->GetContex()->OMSetBlendState(aBlendState, aBlendFactor, aSampleMask);
	}

}