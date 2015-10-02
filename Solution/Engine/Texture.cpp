#include "stdafx.h"

#include <D3DX11tex.h>
#include <DL_Debug.h>
#include "Texture.h"


Prism::Texture::~Texture()
{
	myTexture->Release();
}

bool Prism::Texture::LoadTexture(const std::string& aFilePath)
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

const std::string& Prism::Texture::GetFileName() const
{
	return myFileName;
}

ID3D11ShaderResourceView* Prism::Texture::GetShaderView()
{
	return myTexture;
}