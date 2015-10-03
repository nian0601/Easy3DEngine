#pragma once

struct ID3D11BlendState;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectVectorVariable;

namespace Easy3D
{
	class BaseEffect
	{
	public:
		BaseEffect();
		virtual ~BaseEffect();

		virtual bool Init(const std::string& aEffectFile) = 0;

		void SetBlendState(ID3D11BlendState* aBlendState, float aBlendFactor[4], const unsigned int aSampleMask = 0xFFFFFFFF);
		void AddListener(EffectListener* aListener);
		virtual bool ReloadShader(const std::string& aFile) = 0;

		ID3DX11Effect* GetEffect();
		ID3DX11EffectTechnique* GetTechnique();
		const std::string& GetFileName() const;


		

	protected:
		ID3DX11Effect* myEffect;
		ID3DX11EffectTechnique* myTechnique;

		std::string myFileName;

		CU::GrowingArray<EffectListener*> myEffectListeners;
	};

	inline void BaseEffect::AddListener(EffectListener* aListener)
	{
		myEffectListeners.Add(aListener);
	}

	inline ID3DX11Effect* BaseEffect::GetEffect()
	{
		return myEffect;
	}

	inline ID3DX11EffectTechnique* BaseEffect::GetTechnique()
	{
		return myTechnique;
	}

	inline const std::string& BaseEffect::GetFileName() const
	{
		return myFileName;
	}

}