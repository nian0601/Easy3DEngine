#pragma once

#include <D3D11.h>
#include "D3DPointers.h"
#include "EffectListener.h"
#include "VertexBufferWrapper.h"
#include "Vertices.h"

namespace Easy3D
{
	class BaseEffect;
	class Surface;
	struct IndexBufferWrapper;
	struct VertexBufferWrapper;

	class BaseModel : public EffectListener
	{
	public:
		BaseModel();
		virtual ~BaseModel();

		virtual void SetAlbedoResource(ID3D11ShaderResourceView* aResource);

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

		D3DPointer<ID3D11InputLayout> myVertexLayout;
		D3DPointer<ID3D11BlendState> myBlendState;
		D3D11_BUFFER_DESC myVertexBufferDesc;
		D3D11_BUFFER_DESC myIndexBufferDesc;
		D3D11_SUBRESOURCE_DATA myInitData;

		VertexBufferWrapper* myVertexBuffer;
		IndexBufferWrapper* myIndexBuffer;

		CU::Vector2<float> myPosition;
		CU::Vector2<float> myScale;
	};
}
