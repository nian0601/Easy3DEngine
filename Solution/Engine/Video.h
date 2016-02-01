/*
Use this class to create and show a video
Limitations:
-Video must be in the windows media format (.wmv)
-No audio will be avalible at the moment (workaround: play a sound at the same time you play the video)
*/

#pragma once
#include <string>

struct ID3D11ShaderResourceView;

namespace Easy3D
{
	class WMVVideoInstance;

	class Video
	{
	public:
		Video();
		~Video();

		void Init(LPCWSTR aFileName);
		void Play();
		void Stop();
		void Render();
		bool IsDone() const;

		ID3D11ShaderResourceView* GetView() const;

	private:
		WMVVideoInstance* myInstance;
	};
}