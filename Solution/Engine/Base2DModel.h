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
	class Effect2D;
	class Surface;
	class Camera;
	struct IndexBufferWrapper;
	struct VertexBufferWrapper;

	class Base2DModel : public EffectListener
	{
	public:
		Base2DModel();
		~Base2DModel();

	protected:
		void Render(const CU::Vector2<float>& aPosition, const CU::Vector2<float>& aScale = { 1.f, 1.f }
		, const CU::Vector4<float>& aColor = { 1.f, 1.f, 1.f, 1.f });

		template <typename VertexType>
		void InitVertexBuffer();
		void InitIndexBuffer();
		void InitSurface(const std::string& aResourceName, const std::string& aFileName);
		void InitBlendState();

		void SetupVertexBuffer(int aVertexCount, size_t aVertexSize, char* aVertexData);
		void SetupIndexBuffer(int aVertexCount, char* aIndexData);

		virtual void OnEffectLoad();

		Effect2D* myEffect;
		Surface* mySurface;
		
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
