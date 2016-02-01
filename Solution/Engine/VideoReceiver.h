#pragma once

struct ID3D11ShaderResourceView;
namespace Easy3D
{
	class VideoReceiver
	{
	public:
		virtual void SetVideoResource(ID3D11ShaderResourceView* aResource) = 0;
		virtual void OnVideoStop() = 0;

	protected:
		VideoReceiver(){};
		virtual ~VideoReceiver(){};
	};
}