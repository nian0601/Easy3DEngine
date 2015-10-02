#pragma once
#include <GrowingArray.h>
#include <Matrix.h>
#include "Vertices.h"

struct ID3D11BlendState;
struct ID3D11InputLayout;
struct D3D11_BUFFER_DESC;
struct D3D11_SUBRESOURCE_DATA;

namespace Easy3D
{
	class Effect;
	class Font;
	class Surface;
	class Camera;

	
	struct IndexBufferWrapper;
	struct VertexBufferWrapper;
	class Text
	{
	public:
		Text();
		~Text();

		void Init(Font* aFont);

		void Render(const char* aString, const float aDrawX, const float aDrawY
			, const CU::Vector4<float>& aColor = CU::Vector4<float>(1.f, 1.f, 1.f, 1.f), const float aScale = 1.f);


		float GetTextWidth() const;
		CU::Vector2<float> GetTextSize(const char* aString) const;

	private:
		void InitSentence();
		void InitVertexBuffer();
		void InitIndexBuffer();
		void InitSurface();
		void InitBlendState();

		void SetupVertexBuffer();
		void SetupIndexBuffer();


		void CreateFirstTri(const CU::Vector3<float>& aDrawPos, const float aScale, 
				const int aIndex, const CU::Vector2<float>& aTopLeftUV, const CU::Vector2<float>& aBotRightUV,
				const CU::Vector4<float>& aColor);
		void CreateSecondTri(const CU::Vector3<float>& aDrawPos, const float aScale, 
				const int aIndex, const CU::Vector2<float>& aTopLeftUV, const CU::Vector2<float>& aBotRightUV, 
				const CU::Vector4<float>& aColor);

		void UpdateSentence(const char* aString, const float aDrawX, const float aDrawY,
			const CU::Vector4<float>& aColor, const float aScale);

		Font* myFont;
		Effect* myEffect;
		ID3D11InputLayout* myVertexLayout;
		CU::GrowingArray<VertexPosColorUV> myVertices;
		CU::GrowingArray<int> myVerticeIndices;

		VertexBufferWrapper* myVertexBuffer;
		D3D11_BUFFER_DESC* myVertexBufferDesc;
		IndexBufferWrapper* myIndexBuffer;
		D3D11_BUFFER_DESC* myIndexBufferDesc;
		D3D11_SUBRESOURCE_DATA* myInitData;

		Surface* mySurface;

		CU::Matrix44<float>  myIdentityMatrix;
		ID3D11BlendState* myBlendState;

		bool myHasText;
		CU::Vector2<float> myCharSize;
		float myTextWidth;

		std::string myLastText;
		float myLastDrawX;
		float myLastDrawY;
		float myLastScale;

		float myCharSpacing;
	};
}