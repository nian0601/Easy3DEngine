#pragma once
#include <string>
#include <D3D11.h>
#include "D3DPointers.h"

struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

namespace Easy3D
{
	class Texture
	{
	public:
		void Init(float aWidth, float aHeight, unsigned int aBindFlag
			, unsigned int aFormat);
		bool LoadTexture(const std::string& aFilePath);
		const std::string& GetFileName() const;
		ID3D11ShaderResourceView* GetShaderView();
		ID3D11RenderTargetView* GetRenderTargetView();
		ID3D11DepthStencilView* GetDepthStencilView();
		void CreateDepthStencilView(float aWidth, float aHeight);
	private:
		std::string myFileName;
		D3DPointer<ID3D11ShaderResourceView> myTexture;
		D3DPointer<ID3D11RenderTargetView> myRenderTargetView;
		D3DPointer<ID3D11DepthStencilView> myDepthStencilView;
	};
}