#pragma once
#include "BaseEffect.h"

struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVectorVariable;

namespace Easy3D
{
	class Effect2D : public BaseEffect
	{
	public:
		bool Init(const std::string& aEffectFile) override;
		bool ReloadShader(const std::string& aFile) override;

		void UpdatePosAndScale(const CU::Vector2<float>& aPos
			, const CU::Vector2<float>& aScale = { 1.f, 1.f });

		void SetPosAndScale();
		void SetColor(const CU::Vector4<float>& aColor);
		void SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix);

	private:
		ID3DX11EffectMatrixVariable* myProjectionMatrixVariable;
		ID3DX11EffectVectorVariable* myPosAndScaleVariable;
		ID3DX11EffectVectorVariable* myColorVariable;

		CU::Vector4<float> myPosAndScale;
	};

}