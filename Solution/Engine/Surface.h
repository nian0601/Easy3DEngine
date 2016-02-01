#pragma once
#include <D3DX11.h>
#include <GrowingArray.h>

struct ID3DX11EffectShaderResourceVariable;

namespace Easy3D
{
	class BaseEffect;
	class Texture;

	class Surface
	{
	public:
		Surface();

		int GetVertexStart() const;
		void SetVertexStart(int aStart);

		int GetVertexCount() const;
		void SetVertexCount(int aCount);

		int GetIndexStart() const;
		void SetIndexStart(int aStart);

		int GetIndexCount() const;
		void SetIndexCount(int aCount);

		void SetEffect(BaseEffect* aEffect);
		void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY aTopology);

		bool SetTexture(const std::string& aResourceName, const std::string& aFileName, bool aUseSRGB);
		bool SetTexture(const std::string& aResourceName, Texture* aTexture);

		void Activate();
		void ReloadSurface();

		void SetAlbedoResource(ID3D11ShaderResourceView* aResource);

	private:
		CU::GrowingArray<Texture*> myTextures;
		CU::GrowingArray<ID3DX11EffectShaderResourceVariable*> myShaderVariables;
		CU::GrowingArray<ID3D11ShaderResourceView*> myShaderResoures;
		CU::GrowingArray<std::string> myShaderResourceNames;
		CU::GrowingArray<std::string> myFilePaths;

		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopologyType;
		int myVertexStart;
		int myVertexCount;
		int myIndexCount;
		int myIndexStart;

		BaseEffect* myEffect;
	};
}

inline int Easy3D::Surface::GetVertexStart() const
{
	return myVertexStart;
}

inline void Easy3D::Surface::SetVertexStart(const int aStart)
{
	myVertexStart = aStart;
}

inline int Easy3D::Surface::GetVertexCount() const
{
	return myVertexCount;
}

inline void Easy3D::Surface::SetVertexCount(const int aCount)
{
	myVertexCount = aCount;
}

inline int Easy3D::Surface::GetIndexStart() const
{
	return myIndexStart;
}

inline void Easy3D::Surface::SetIndexStart(const int aStart)
{
	myIndexStart = aStart;
}

inline int Easy3D::Surface::GetIndexCount() const
{
	return myIndexCount;
}

inline void Easy3D::Surface::SetIndexCount(const int aCount)
{
	myIndexCount = aCount;
}

inline void Easy3D::Surface::SetEffect(BaseEffect* aEffect)
{
	myEffect = aEffect;
}

inline void Easy3D::Surface::SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY aTopology)
{
	myPrimitiveTopologyType = aTopology;
}
