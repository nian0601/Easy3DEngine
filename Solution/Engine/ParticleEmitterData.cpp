#include "stdafx.h"

#include <d3dx11effect.h>
#include "ParticleEffect.h"
#include "ParticleEmitterData.h"
#include "FileWatcher.h"
#include <XMLReader.h>

namespace Easy3D
{

	void ParticleEmitterData::Init(const std::string& aFile)
	{
		LoadDataFile(aFile);

		WATCH_FILE(aFile, ParticleEmitterData::LoadDataFile);
	}

	void ParticleEmitterData::LoadDataFile(const std::string& aFile)
	{
		XMLReader reader;
		reader.OpenDocument(aFile);

		XMLELEMENT root = reader.FindFirstChild("root");
		XMLELEMENT emitter = reader.FindFirstChild(root, "emitter");

		XMLELEMENT readElem = reader.ForceFindFirstChild(emitter, "myEffectName");
		std::string shaderFile;
		reader.ForceReadAttribute(readElem, "value", shaderFile);

		readElem = reader.ForceFindFirstChild(emitter, "myEmissionExtents");
		reader.ForceReadAttribute(readElem, "x", myEmissionExtents.x);
		reader.ForceReadAttribute(readElem, "y", myEmissionExtents.y);
		reader.ForceReadAttribute(readElem, "z", myEmissionExtents.z);

		readElem = reader.ForceFindFirstChild(emitter, "myEmissionLifeTime");
		reader.ForceReadAttribute(readElem, "value", myEmissionLifeTime);

		readElem = reader.ForceFindFirstChild(emitter, "myEmissionRate");
		reader.ForceReadAttribute(readElem, "value", myEmissionRate);

		readElem = reader.ForceFindFirstChild(emitter, "myEmissionRateDelta");
		reader.ForceReadAttribute(readElem, "value", myEmissionRateDelta);

		readElem = reader.ForceFindFirstChild(emitter, "myEmissionVelocityDelta");
		reader.ForceReadAttribute(readElem, "x", myEmissionVelocityDelta.x);
		reader.ForceReadAttribute(readElem, "y", myEmissionVelocityDelta.y);
		reader.ForceReadAttribute(readElem, "z", myEmissionVelocityDelta.z);

		readElem = reader.ForceFindFirstChild(emitter, "myMaxVelocity");
		reader.ForceReadAttribute(readElem, "x", myMaxVelocity.x);
		reader.ForceReadAttribute(readElem, "y", myMaxVelocity.y);
		reader.ForceReadAttribute(readElem, "z", myMaxVelocity.z);

		readElem = reader.ForceFindFirstChild(emitter, "myMinVelocity");
		reader.ForceReadAttribute(readElem, "x", myMinVelocity.x);
		reader.ForceReadAttribute(readElem, "y", myMinVelocity.y);
		reader.ForceReadAttribute(readElem, "z", myMinVelocity.z);

		readElem = reader.ForceFindFirstChild(emitter, "myGravity");
		reader.ForceReadAttribute(readElem, "value", myGravity);

		readElem = reader.ForceFindFirstChild(emitter, "myTextureName");
		std::string texFile;
		reader.ForceReadAttribute(readElem, "value", texFile);

		myEffect = Engine::GetInstance()->GetEffectContainer()->GetParticleEffect(shaderFile);
		myTexture = Engine::GetInstance()->GetTextureContainer()->GetTexture(texFile);
		


		XMLELEMENT particleElem = reader.FindFirstChild(root, "particle");

		readElem = reader.ForceFindFirstChild(particleElem, "myParticlesFollowEmitter");
		reader.ForceReadAttribute(readElem, "value", myParticleData.myParticleFollowEmitter);

		readElem = reader.ForceFindFirstChild(particleElem, "myAffectedByGravity");
		reader.ForceReadAttribute(readElem, "value", myParticleData.myGravityFlag);

		readElem = reader.ForceFindFirstChild(particleElem, "myAlphaDelta");
		reader.ForceReadAttribute(readElem, "value", myParticleData.myAlphaDelta);

		readElem = reader.ForceFindFirstChild(particleElem, "myLifeTime");
		reader.ForceReadAttribute(readElem, "value", myParticleData.myLifeTime);

		readElem = reader.ForceFindFirstChild(particleElem, "myMaxStartSize");
		reader.ForceReadAttribute(readElem, "value", myParticleData.myMaxStartSize);

		readElem = reader.ForceFindFirstChild(particleElem, "myMinStartSize");
		reader.ForceReadAttribute(readElem, "value", myParticleData.myMinStartSize);

		readElem = reader.ForceFindFirstChild(particleElem, "myMaxVelocity");
		reader.ForceReadAttribute(readElem, "x", myParticleData.myMaxVelocity.x);
		reader.ForceReadAttribute(readElem, "y", myParticleData.myMaxVelocity.y);
		reader.ForceReadAttribute(readElem, "z", myParticleData.myMaxVelocity.z);

		readElem = reader.ForceFindFirstChild(particleElem, "myMinVelocity");
		reader.ForceReadAttribute(readElem, "x", myParticleData.myMinVelocity.x);
		reader.ForceReadAttribute(readElem, "y", myParticleData.myMinVelocity.y);
		reader.ForceReadAttribute(readElem, "z", myParticleData.myMinVelocity.z);

		readElem = reader.ForceFindFirstChild(particleElem, "mySizeDelta");
		reader.ForceReadAttribute(readElem, "value", myParticleData.mySizeDelta);

		readElem = reader.ForceFindFirstChild(particleElem, "myStartAlpha");
		reader.ForceReadAttribute(readElem, "value", myParticleData.myStartAlpha);

		D3DX11_PASS_DESC passDesc;
		myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

		D3D11_INPUT_ELEMENT_DESC particleDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TIME", 0, DXGI_FORMAT_R32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(particleDesc
			, ARRAYSIZE(particleDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myParticleInputLayout);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to CreateInputLayout", "ParticleEmitterData::LoadDataFile", MB_ICONWARNING);
		}

		myMaxNrOfParticles = static_cast<int>(myParticleData.myLifeTime / myEmissionRate) + 1;
		myHasNewData = true;
	}

}
