#include "stdafx.h"

#include "Camera.h"
#include <d3d11.h>
#include <d3dx11effect.h>
#include <MathHelper.h>
#include "ParticleEmitterData.h"
#include "ParticleEmitterInstance.h"
#include "ParticleEffect.h"
#include "VertexBufferWrapper.h"

namespace Easy3D
{
	ParticleEmitterInstance::ParticleEmitterInstance(ParticleEmitterData& aEmitterData)
		: myEmitterData(aEmitterData)
		, myGravity(0.f)
		, myProcessTime(0.f)
	{
		myVertexBufferWrapper = nullptr;
		myParticles.Init(myEmitterData.myMaxNrOfParticles);
		myParticles.Add(ParticleInstance());
		CreateVertexBuffer();
	}
	
	
	ParticleEmitterInstance::~ParticleEmitterInstance()
	{
		delete myVertexBufferWrapper;
	}

	void ParticleEmitterInstance::Update(float aDelta)
	{
		if (myEmitterData.myHasNewData == true)
		{
			CreateVertexBuffer();
		}
		EmitterUpdate(aDelta);
	}

	void ParticleEmitterInstance::Render(const Camera& aCamera)
	{
		UpdateVertexBuffer();

		myEmitterData.myEffect->SetProjectionMatrix(aCamera.GetProjection());
		myEmitterData.myEffect->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));

		if (myEmitterData.myParticleData.myParticleFollowEmitter == true)
		{
			myEmitterData.myEffect->SetWorldMatrix(myOrientation);
		}
		else
		{
			myEmitterData.myEffect->SetWorldMatrix(CU::Matrix44<float>());
		}

		myEmitterData.myEffect->SetAlphaDelta(myEmitterData.myParticleData.myAlphaDelta);
		myEmitterData.myEffect->SetSizeDelta(myEmitterData.myParticleData.mySizeDelta);
		myEmitterData.myEffect->SetStartAlpha(myEmitterData.myParticleData.myStartAlpha);
		myEmitterData.myEffect->SetTexture(myEmitterData.myTexture);

		Engine::GetInstance()->GetContex()->IASetInputLayout(myEmitterData.myParticleInputLayout);
		Engine::GetInstance()->GetContex()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBufferWrapper->myStartSlot
			, myVertexBufferWrapper->myNumberOfBuffers, &myVertexBufferWrapper->myVertexBuffer
			, &myVertexBufferWrapper->myStride, &myVertexBufferWrapper->myByteOffset);



		D3DX11_TECHNIQUE_DESC techDesc;
		myEmitterData.myEffect->GetTechnique()->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			myEmitterData.myEffect->GetTechnique()->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
			Engine::GetInstance()->GetContex()->Draw(myParticles.Size(), 0);
		}
	}

	void ParticleEmitterInstance::PerformRotation(const CU::Matrix44<float>& aRotation)
	{
		myOrientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
		myOrientation = aRotation * myOrientation;
		myOrientation.SetPos(myPosition);
	}

	void ParticleEmitterInstance::CreateVertexBuffer()
	{
		myEmitterData.myHasNewData = false;
		if (myVertexBufferWrapper != nullptr)
		{
			myVertexBufferWrapper->myVertexBuffer->Release();
		}

		myVertexBufferWrapper = new VertexBufferWrapper();
		myVertexBufferWrapper->myStride = sizeof(ParticleInstance);
		myVertexBufferWrapper->myByteOffset = 0;
		myVertexBufferWrapper->myStartSlot = 0;
		myVertexBufferWrapper->myNumberOfBuffers = 1;


		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		if (myVertexBufferWrapper->myVertexBuffer != nullptr)
			myVertexBufferWrapper->myVertexBuffer->Release();

		vertexBufferDesc.ByteWidth = sizeof(ParticleInstance) * myEmitterData.myMaxNrOfParticles;

		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = reinterpret_cast<char*>(&myParticles[0]);


		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &initData
			, &myVertexBufferWrapper->myVertexBuffer);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("ParticleEmitterInstance::CreateVertexBuffer: Failed to CreateVertexBuffer");
		}
	}

	void ParticleEmitterInstance::EmitterUpdate(float aDelta)
	{
		myProcessTime += aDelta;

		if (myProcessTime >= myEmitterData.myEmissionRate)
		{
			EmittParticle(aDelta);
			myProcessTime = 0.f;
		}

		UpdateParticles(aDelta);
	}

	void ParticleEmitterInstance::EmittParticle(float)
	{
		if (myParticles.Size() == myEmitterData.myMaxNrOfParticles)
		{
			return;
		}

		ParticleInstance newParticle;
		newParticle.myAlpha = myEmitterData.myParticleData.myStartAlpha;
		newParticle.myTime = myEmitterData.myParticleData.myLifeTime;

		CU::Vector3<float> minExt = myOrientation.GetPos() - myEmitterData.myEmissionExtents / 2.f;
		CU::Vector3<float> maxExt = myOrientation.GetPos() + myEmitterData.myEmissionExtents / 2.f;

		newParticle.myPosition = CU::Math::RandomVector(minExt, maxExt);

		newParticle.mySize = CU::Math::RandomRange<float>(myEmitterData.myParticleData.myMinStartSize
			, myEmitterData.myParticleData.myMaxStartSize);

		newParticle.myVelocity = CU::Math::RandomVector(myEmitterData.myParticleData.myMinVelocity
			, myEmitterData.myParticleData.myMaxVelocity);

		myParticles.Add(newParticle);
	}

	void ParticleEmitterInstance::UpdateParticles(float aDelta)
	{
		for (int i = myParticles.Size() - 1; i >= 0; --i)
		{
			myParticles[i].myTime -= aDelta;
			if (myParticles[i].myTime <= 0.f)
			{
				myParticles.RemoveCyclicAtIndex(i);
				continue;
			}

			myParticles[i].myPosition += myParticles[i].myVelocity * aDelta;
			if (myEmitterData.myParticleData.myGravityFlag == true)
			{
				myParticles[i].myVelocity += myEmitterData.myGravity * aDelta;

			}

			myParticles[i].myAlpha += myEmitterData.myParticleData.myAlphaDelta * aDelta;
			myParticles[i].mySize += myEmitterData.myParticleData.mySizeDelta * aDelta;
		}
	}

	void ParticleEmitterInstance::UpdateVertexBuffer()
	{
		if (myParticles.Size() > 0)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			Engine::GetInstance()->GetContex()->Map(
				myVertexBufferWrapper->myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			if (mappedResource.pData != nullptr)
			{
				ParticleInstance* data = (ParticleInstance*)mappedResource.pData;
				memcpy(data, &myParticles[0], sizeof(ParticleInstance) * myEmitterData.myMaxNrOfParticles);
			}

			Engine::GetInstance()->GetContex()->Unmap(myVertexBufferWrapper->myVertexBuffer, 0);
		}

	}
}