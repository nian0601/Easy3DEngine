#pragma once
#include "BaseEffect.h"

namespace Easy3D
{
	class Effect2D : public BaseEffect
	{
	public:
		Effect2D();
		~Effect2D();

		bool Init(const std::string& aEffectFile) override;
		bool ReloadShader(const std::string& aFile) override;
	};

}