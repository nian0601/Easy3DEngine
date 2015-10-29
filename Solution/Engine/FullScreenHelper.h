#pragma once
#include "BaseModel.h"

struct ID3DX11EffectShaderResourceVariable;

namespace Easy3D
{
	class BaseEffect;
	class Texture;

	struct SceneData;

	class FullScreenHelper : public BaseModel
	{
	public:
		FullScreenHelper();
		~FullScreenHelper();

		void Process(SceneData* aSceneData, int aEffect);

		void Combine(Texture* aSource, Texture* aTarget);
		void Combine(Texture* aSourceA, Texture* aSourceB, Texture* aTarget);

		void RenderToScreen(Texture* aToBackbufferTexture);
		void ActivateBuffers();

	private:
		struct CombineData
		{
			BaseEffect* myEffect = nullptr;
			ID3DX11EffectShaderResourceVariable* mySourceA = nullptr;
			ID3DX11EffectShaderResourceVariable* mySourceB = nullptr;
		};
		struct RenderToTextureData
		{
			BaseEffect* myEffect = nullptr;
			ID3DX11EffectShaderResourceVariable* mySource = nullptr;
		};
		struct DownSampleData
		{
			BaseEffect* myEffect;
			ID3DX11EffectShaderResourceVariable* myTexture;

			Texture* myBloomDownSample[2];
			CU::GrowingArray<Texture*> myHDRDownSamples;
			Texture* myFinalHDRDownSample;
			int myHDRDownSamplesCount;
		};
		struct HDRData
		{
			BaseEffect* myEffect;
			ID3DX11EffectShaderResourceVariable* myOriginalTexture;
			ID3DX11EffectShaderResourceVariable* myAverageColorTexture;
		};
		struct BloomData
		{
			BaseEffect* myEffect;
			ID3DX11EffectShaderResourceVariable* myTexture;
			ID3DX11EffectScalarVariable* myTexelWidthVariable;
			ID3DX11EffectScalarVariable* myTexelHeightVariable;
			Texture* myMiddleMan;
			Texture* myFinalTexture;
		};

		
		void CreateVertices();
		void CreateCombineData();
		void CreateRenderToTextureData();
		void CreateDownSampleData();
		void CreateHDRData();
		void CreateBloomData();
		float HDRLog2(float aNumber);

		void Render(BaseEffect* aEffect);
		void Render(BaseEffect* aEffect, const std::string& aTechnique);

		void DownSample(Texture* aTarget, Texture* aSource, float aWidth, float aHeight
			, bool aHDRDownSample = false);

		void HDRDownSample(Texture* aSource);
		void HDREffect(Texture* aSource, Texture* aTarget);

		void BloomEffect(Texture* aSource);
		void DoBloom(Texture* aSource, Texture* aTarget);

		CombineData myCombineData;
		RenderToTextureData myRenderToTextureData;
		DownSampleData myDownSampleData;
		HDRData myHDRData;
		BloomData myBloomData;
		Texture* myProcessedTexture;
		float myClearColor[4];
		D3D11_VIEWPORT myViewPort;
	};
}

