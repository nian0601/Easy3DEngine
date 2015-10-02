#pragma once
#include <GrowingArray.h>
#include <Matrix.h>
#include "Vertices.h"
#include "EffectListener.h"

struct ID3D11BlendState;
struct ID3D11InputLayout;
struct D3D11_BUFFER_DESC;
struct D3D11_SUBRESOURCE_DATA;

namespace Prism
{
	class Effect;
	class Surface;
	class Camera;
	struct IndexBufferWrapper;
	struct VertexBufferWrapper;

	class Model2D : public EffectListener
	{
	public:
		Model2D();
		~Model2D();

		void Init(const std::string& aFileName, const CU::Vector2<float> aTextureSize);
		void Render(const Camera& aCamera, const float aDrawX, const float aDrawY);

	private:

		void Update(const float aDrawX, const float aDrawY);
		void InitVertexBuffer();
		void InitIndexBuffer();
		void InitSurface(const std::string& aFileName);
		void InitBlendState();

		void SetupVertexBuffer();
		void SetupIndexBuffer();

		void OnEffectLoad();

		Effect* myEffect;
		ID3D11InputLayout* myVertexLayout;
		CU::GrowingArray<VertexPosUV> myVertices;
		CU::GrowingArray<int> myVerticeIndices;

		VertexBufferWrapper* myVertexBuffer;
		D3D11_BUFFER_DESC* myVertexBufferDesc;
		IndexBufferWrapper* myIndexBuffer;
		D3D11_BUFFER_DESC* myIndexBufferDesc;
		D3D11_SUBRESOURCE_DATA* myInitData;

		Surface* mySurface;

		CU::Matrix44<float> myIdentityMatrix;
		CU::Vector2<float> myTextureSize;
		ID3D11BlendState* myBlendState;

		float myLastDrawX;
		float myLastDrawY;
	};
}