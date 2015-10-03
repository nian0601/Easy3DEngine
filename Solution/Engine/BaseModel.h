#pragma once
#include "EffectListener.h"
#include "VertexBufferWrapper.h"

struct ID3D11BlendState;
struct ID3D11InputLayout;
struct D3D11_BUFFER_DESC;
struct D3D11_INPUT_ELEMENT_DESC;
struct D3D11_SUBRESOURCE_DATA;

namespace Easy3D
{
	class BaseEffect;
	class Effect3D;
	class Effect2D;
	class Surface;
	class Camera;
	struct IndexBufferWrapper;
	struct VertexBufferWrapper;

	class BaseModel : public EffectListener
	{
	public:
		BaseModel();
		~BaseModel();

	protected:
		void Render();

		void InitInputLayout(D3D11_INPUT_ELEMENT_DESC* aVertexDescArray, int aArraySize);
		void InitVertexBuffer(int aVertexSize, int aBufferUsage, int aCPUUsage);
		void InitIndexBuffer();
		void InitSurface(const std::string& aResourceName, const std::string& aFileName);
		void InitBlendState();

		void SetupVertexBuffer(int aVertexCount, int aVertexSize, char* aVertexData);
		void SetupIndexBuffer(int aVertexCount, char* aIndexData);

		virtual void OnEffectLoad();

		BaseEffect* myEffect;
		CU::GrowingArray<Surface*> mySurfaces;

		ID3D11InputLayout* myVertexLayout;
		ID3D11BlendState* myBlendState;
		D3D11_BUFFER_DESC* myVertexBufferDesc;
		D3D11_BUFFER_DESC* myIndexBufferDesc;
		D3D11_SUBRESOURCE_DATA* myInitData;

		VertexBufferWrapper* myVertexBuffer;
		IndexBufferWrapper* myIndexBuffer;

		CU::Vector2<float> myPosition;
		CU::Vector2<float> myScale;
	};
}
