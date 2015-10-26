#include "stdafx.h"

#include <D3DX11tex.h>
#include <DL_Debug.h>
#include "Texture.h"


Easy3D::Texture::~Texture()
{
	myTexture->Release();
}


void Easy3D::Texture::Init(float aWidth, float aHeight, unsigned int aBindFlag, unsigned int aFormat)
{
	D3D11_TEXTURE2D_DESC tempBufferInfo;
	tempBufferInfo.Width = static_cast<unsigned int>(aWidth);
	tempBufferInfo.Height = static_cast<unsigned int>(aHeight);
	tempBufferInfo.MipLevels = 1;
	tempBufferInfo.ArraySize = 1;
	tempBufferInfo.Format = static_cast<DXGI_FORMAT>(aFormat);
	tempBufferInfo.SampleDesc.Count = 1;
	tempBufferInfo.SampleDesc.Quality = 0;
	tempBufferInfo.Usage = D3D11_USAGE_DEFAULT;
	tempBufferInfo.BindFlags = aBindFlag;
	tempBufferInfo.CPUAccessFlags = 0;
	tempBufferInfo.MiscFlags = 0;

	ID3D11Texture2D* tempBuffer;
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateTexture2D(&tempBufferInfo, NULL, &tempBuffer);

	hr = Engine::GetInstance()->GetDevice()->CreateRenderTargetView(tempBuffer, NULL, &myRenderTargetView);
	if (FAILED(hr))
		assert(0);
	hr = Engine::GetInstance()->GetDevice()->CreateShaderResourceView(tempBuffer, NULL, &myTexture);
	if (FAILED(hr))
		assert(0);
}

bool Easy3D::Texture::LoadTexture(const std::string& aFilePath)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(Engine::GetInstance()->GetDevice(), aFilePath.c_str()
		, NULL, NULL, &myTexture, NULL);
	myFileName = aFilePath;
	if (FAILED(hr) != S_OK)
	{
		RESOURCE_LOG("Failed to load: %s, trying to use debug-texture.", aFilePath.c_str());
		hr = D3DX11CreateShaderResourceViewFromFile(Engine::GetInstance()->GetDevice()
			, "Data/resources/texture/uv_map.png", NULL, NULL, &myTexture, NULL);
		myFileName = "Data/resources/texture/uv_map.png";
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("[Texture]: Failed to load debug-texture: Data/resources/texture/uv_map.png");
		}
	}

	return true;
}

const std::string& Easy3D::Texture::GetFileName() const
{
	return myFileName;
}

ID3D11ShaderResourceView* Easy3D::Texture::GetShaderView()
{
	return myTexture;
}

ID3D11RenderTargetView* Easy3D::Texture::GetRenderTargetView()
{
	return myRenderTargetView;
}
