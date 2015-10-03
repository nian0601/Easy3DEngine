#pragma once
#include "EffectListener.h"
#include "VertexBufferWrapper.h"

struct ID3D11BlendState;
struct ID3D11InputLayout;
struct D3D11_BUFFER_DESC;
struct D3D11_SUBRESOURCE_DATA;

namespace Easy3D
{
	class Effect3D;
	class Surface;
	class Camera;
	struct IndexBufferWrapper;
	struct VertexBufferWrapper;

	class Base2DModel : EffectListener
	{
	public:
		Base2DModel();
		~Base2DModel();

	protected:
		template <typename VertexType>
		void InitVertexBuffer();
		void InitIndexBuffer();
		void InitSurface(const std::string& aFileName);
		void InitBlendState();

		void SetupVertexBuffer(int aVertexCount, size_t aVertexSize, char* aVertexData);
		void SetupIndexBuffer(int aVertexCount, char* aIndexData);

		void OnEffectLoad();

		Effect3D* myEffect;
		Surface* mySurface;
		
		ID3D11InputLayout* myVertexLayout;
		ID3D11BlendState* myBlendState;
		D3D11_BUFFER_DESC* myVertexBufferDesc;
		D3D11_BUFFER_DESC* myIndexBufferDesc;
		D3D11_SUBRESOURCE_DATA* myInitData;

		VertexBufferWrapper* myVertexBuffer;
		IndexBufferWrapper* myIndexBuffer;

		CU::Matrix44<float> myOrienation;
	};

	template <typename VertexType>
	void Easy3D::Base2DModel::InitVertexBuffer()
	{
		myVertexBuffer = new VertexBufferWrapper();
		myVertexBuffer->myStride = sizeof(VertexType);
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNumberOfBuffers = 1;


		ZeroMemory(myVertexBufferDesc, sizeof(myVertexBufferDesc));
		myVertexBufferDesc->Usage = D3D11_USAGE_DYNAMIC;
		myVertexBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
		myVertexBufferDesc->CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		myVertexBufferDesc->MiscFlags = 0;
		myVertexBufferDesc->StructureByteStride = 0;
	}
}
