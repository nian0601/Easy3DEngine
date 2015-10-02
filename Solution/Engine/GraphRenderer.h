#pragma once

#include <GrowingArray.h>
#include <Matrix.h>
#include "Vertices.h"

struct D3D11_BUFFER_DESC;
struct D3D11_SUBRESOURCE_DATA;
struct ID3D11InputLayout;

namespace Prism
{
	class Camera;
	class Effect;
	class Font;
	class Surface;

	struct IndexBufferWrapper;
	struct VertexBufferWrapper;

	class GraphRenderer
	{
	public:
		GraphRenderer();
		~GraphRenderer();

		void Init();

		void Render(const CU::GrowingArray<float>& aDataArray
			, const CU::Vector2<float>& aTopLeftDrawPos, const CU::Vector2<float>& aGraphSize
			, const float aMaxValue, bool aNewData = false);

	private:
		void InitVertexBuffer();
		void InitIndexBuffer();
		void InitSurface();

		void BuildBuffers(const CU::GrowingArray<float>& aDataArray, const CU::Vector2<float>& aTopLeftDrawPos
			, const CU::Vector2<float>& aGraphSize, const float aMaxValue);
		void CreateFirstTri(const CU::Vector2<float>& aBotLeft, const CU::Vector2<float>& aColumSize
			, const int aIndex, const float aHeightCoef);
		void CreateSecondTri(const CU::Vector2<float>& aBotLeft, const CU::Vector2<float>& aColumSize
			, const int aIndex, const float aHeightCoef);
		void SetupVertexBuffer();
		void SetupIndexBuffer();
		CU::Vector4<float> GetColor(const float aHeightCoef);

		Effect* myEffect;
		ID3D11InputLayout* myVertexLayout;
		CU::GrowingArray<VertexPosColor> myVertices;
		CU::GrowingArray<int> myVerticeIndices;
		CU::Matrix44<float> myIdentityMatrix;

		VertexBufferWrapper* myVertexBuffer;
		D3D11_BUFFER_DESC* myVertexBufferDesc;
		IndexBufferWrapper* myIndexBuffer;
		D3D11_BUFFER_DESC* myIndexBufferDesc;
		D3D11_SUBRESOURCE_DATA* myInitData;

		Surface* mySurface;
	};
}