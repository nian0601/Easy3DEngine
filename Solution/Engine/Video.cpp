#include "stdafx.h"
#include "Video.h"
#include "WMVVideoInstance.h"

namespace Easy3D
{
	Video::Video()
	{
	}


	Video::~Video()
	{
		delete myInstance;
	}

	void Video::Init(LPCWSTR aFileName)
	{
		myInstance = new WMVVideoInstance();
		myInstance->Init(aFileName);
	}

	void Video::Play()
	{
		myInstance->Play();
	}

	void Video::Stop()
	{
		myInstance->Stop();
	}

	void Video::Render()
	{
		myInstance->Render();
	}

	bool Video::IsDone() const
	{
		return myInstance->IsDone();
	}

	ID3D11ShaderResourceView* Video::GetView() const
	{
		return myInstance->GetView();
	}
}