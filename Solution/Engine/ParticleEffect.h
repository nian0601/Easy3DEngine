#pragma once
#include "BaseEffect.h"

struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectShaderResourceVariable;

namespace Easy3D
{
	class Texture;
	class ParticleEffect : public BaseEffect
	{
	public:
		bool Init(const std::string& aEffectFile) override;
		bool ReloadShader(const std::string& aFile) override;

		void SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix);
		void SetViewMatrix(const CU::Matrix44<float>& aViewMatrix);
		void SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix);
		void SetAlphaDelta(float aDelta);
		void SetSizeDelta(float aDelta);
		void SetStartAlpha(float aStartAlpha);
		void SetTexture(Texture* aTexture);

	private:
		ID3DX11EffectMatrixVariable* myProjectionMatrixVariable;
		ID3DX11EffectMatrixVariable* myViewMatrixVariable;
		ID3DX11EffectMatrixVariable* myWorldMatrixVariable;

		ID3DX11EffectScalarVariable* myAlphaDeltaVariable;
		ID3DX11EffectScalarVariable* mySizeDeltaVariable;
		ID3DX11EffectScalarVariable* myStartAlphaVariable;

		ID3DX11EffectShaderResourceVariable* myTextureVariable;
	};
}

