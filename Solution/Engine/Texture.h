#pragma once
#include <string>

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

namespace Easy3D
{
	class Texture
	{
	public:
		~Texture();

		void Init(float aWidth, float aHeight, unsigned int aBindFlag
			, unsigned int aFormat);
		bool LoadTexture(const std::string& aFilePath);
		const std::string& GetFileName() const;
		ID3D11ShaderResourceView* GetShaderView();
		ID3D11RenderTargetView* GetRenderTargetView();

	private:
		std::string myFileName;
		ID3D11ShaderResourceView* myTexture;
		ID3D11RenderTargetView* myRenderTargetView;
	};
}