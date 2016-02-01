#include "stdafx.h"

#include "Camera.h"
#include <d3dx11effect.h>
#include "Effect3D.h"
#include "LineRenderer.h"
#include "VertexBufferWrapper.h"

namespace Easy3D
{
	LineRenderer::LineRenderer()
		: myVertexBuffer(nullptr)
	{
		myEffect = EffectContainer::GetInstance()->Get3DEffect("Data/Shader/3D/LineEffect.fx");
		myPoints.Init(MAX_NR_OF_LINES * 2);
		AddLine({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f });
		CreateVertexBuffer();
		CreateInputLayout();
		myPoints.RemoveAll();
	}


	LineRenderer::~LineRenderer()
	{
		myInputLayout->Release();
		delete myVertexBuffer;
	}

	void LineRenderer::AddLine(const CU::Vector3<float>& aStartPosition, const CU::Vector3<float>& aEndPosition
		, int aPrecision, const CU::Vector4<float>& aColor)
	{
		DL_ASSERT_EXP(myPoints.Size() + aPrecision * 2 < MAX_NR_OF_LINES, "Added to many lines!");
		CU::Vector3<float> step = (aEndPosition - aStartPosition) / float(aPrecision);
		CU::Vector4<float> lineStep(step.x, step.y, step.z, 0.f);

		Point prevPoint;
		Point currPoint;
		prevPoint.myPosition = aStartPosition;
		prevPoint.myColor = aColor;
		currPoint.myColor = aColor;

		for (int i = 0; i < aPrecision; ++i)
		{
			currPoint.myPosition = prevPoint.myPosition + lineStep;

			myPoints.Add(prevPoint);
			myPoints.Add(currPoint);

			prevPoint = currPoint;
		}

		//Point p1;
		//p1.myPosition = aStartPosition;
		//p1.myColor = aColor;

		//Point p2;
		//p2.myPosition = aEndPosition;
		//p2.myColor = aColor;

		//myPoints.Add(p1);
		//myPoints.Add(p2);
	}

	void LineRenderer::Render(const Camera& aCamera)
	{
		UpdateVertexBuffer();

		Engine::GetInstance()->SetDepthBufferState(eDepthStencil::Z_DISABLED);

		myEffect->SetProjectionMatrix(aCamera.GetProjection());
		myEffect->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));
		myEffect->SetWorldMatrix(CU::Matrix44<float>());

		
		Engine::GetInstance()->GetContex()->IASetInputLayout(myInputLayout);
		Engine::GetInstance()->GetContex()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		ID3D11Buffer* buf = myVertexBuffer->myVertexBuffer.Get();
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot
			, myVertexBuffer->myNumberOfBuffers, &buf
			, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);



		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			myEffect->GetTechnique()->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
			Engine::GetInstance()->GetContex()->Draw(myPoints.Size(), 0);
		}

		myPoints.RemoveAll();

		Engine::GetInstance()->SetDepthBufferState(eDepthStencil::Z_ENABLED);
	}

	void LineRenderer::CreateVertexBuffer()
	{
		if (myVertexBuffer != nullptr && myVertexBuffer->myVertexBuffer.Get() != nullptr)
			myVertexBuffer->myVertexBuffer->Release();

		myVertexBuffer = new VertexBufferWrapper();
		myVertexBuffer->myStride = sizeof(Point);
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNumberOfBuffers = 1;


		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;



		vertexBufferDesc.ByteWidth = sizeof(Point) * MAX_NR_OF_LINES * 2;

		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = reinterpret_cast<char*>(&myPoints[0]);

		Engine::GetInstance()->CreateBuffer("LineRenderer::myVertexBuffer"
			, &vertexBufferDesc, &initData, myVertexBuffer->myVertexBuffer);
	}   

	void LineRenderer::CreateInputLayout()
	{
		D3DX11_PASS_DESC passDesc;
		myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

		D3D11_INPUT_ELEMENT_DESC particleDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(particleDesc
			, ARRAYSIZE(particleDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myInputLayout);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to CreateInputLayout", "LineRenderer::CreateInputLayout", MB_ICONWARNING);
		}
	}

	void LineRenderer::UpdateVertexBuffer()
	{
		if (myPoints.Size() > 0)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			Engine::GetInstance()->GetContex()->Map(
				myVertexBuffer->myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			if (mappedResource.pData != nullptr)
			{
				Point* data = (Point*)mappedResource.pData;
				memcpy(data, &myPoints[0], sizeof(Point) * MAX_NR_OF_LINES * 2);
			}

			Engine::GetInstance()->GetContex()->Unmap(myVertexBuffer->myVertexBuffer.Get(), 0);
		}
	}
}