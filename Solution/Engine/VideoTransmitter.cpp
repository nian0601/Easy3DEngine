#include "stdafx.h"
#include "Video.h"
#include "VideoTransmitter.h"
#include "VideoReceiver.h"

namespace Easy3D
{
	VideoTransmitter* VideoTransmitter::myInstance = nullptr;

	VideoTransmitter* VideoTransmitter::GetInstance()
	{
		if (myInstance == nullptr)
		{
			myInstance = new VideoTransmitter();
		}

		return myInstance;
	}

	void VideoTransmitter::Destroy()
	{
		delete myInstance;
		myInstance = nullptr;
	}

	void VideoTransmitter::AddReceiver(const std::wstring& aFeedName, VideoReceiver* aReceiver)
	{
		if (myFeeds.find(aFeedName) == myFeeds.end())
		{
			myFeeds[aFeedName] = nullptr;
		}

		if (myReceivers.find(aFeedName) == myReceivers.end())
		{
			myReceivers[aFeedName].Init(8);
		}

		myReceivers[aFeedName].Add(aReceiver);
	}

	void VideoTransmitter::AddVideo(const std::wstring& aVideoName, const std::wstring& aFilePath)
	{
		if (myVideos.find(aVideoName) == myVideos.end())
		{
			myVideos[aVideoName] = new Video();
			myVideos[aVideoName]->Init(aFilePath.c_str());
		}
		else
		{
			DL_ASSERT("Tried to add more than one video with the same name");
		}
		
	}

	void VideoTransmitter::StartFeed(const std::wstring& aVideoName, const std::wstring& aFeedName)
	{
		DL_ASSERT_EXP(myFeeds.find(aFeedName) != myFeeds.end(), "Tried to start an nonexisting feed");
		DL_ASSERT_EXP(myVideos.find(aVideoName) != myVideos.end(), "Tried to start feed using nonexisting video");

		myFeeds[aFeedName] = myVideos[aVideoName];
		myFeeds[aFeedName]->Play();
	}

	void VideoTransmitter::StartEmergencyVideo(const std::wstring& aVideoName)
	{
		DL_ASSERT_EXP(myVideos.find(aVideoName) != myVideos.end(), "Tried to start feed using nonexisting video");

		myEmergencyVideo = myVideos[aVideoName];
		myEmergencyVideo->Play();
	}

	void VideoTransmitter::StopEmergencyVideo()
	{
		if (myEmergencyVideo != nullptr)
		{
			myEmergencyVideo = nullptr;
		}
	}

	void VideoTransmitter::Update()
	{
		if (myEmergencyVideo != nullptr)
		{
			if (myEmergencyVideo->IsDone())
			{
				myEmergencyVideo = nullptr;
			}
			else
			{
				myEmergencyVideo->Render();
			}
		}

		for (auto it = myFeeds.begin(); it != myFeeds.end(); ++it)
		{
			Video* video = it->second;
			if (myEmergencyVideo != nullptr)
			{
				video = myEmergencyVideo;
			}
			else
			{
				video->Render();
			}

			CU::GrowingArray<VideoReceiver*> receivers = myReceivers[it->first];
			for (int i = 0; i < receivers.Size(); ++i)
			{
				receivers[i]->SetVideoResource(video->GetView());
			}
		}
	}

	VideoTransmitter::VideoTransmitter()
		: myEmergencyVideo(nullptr)
	{
	}


	VideoTransmitter::~VideoTransmitter()
	{
		for (auto it = myVideos.begin(); it != myVideos.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}
		myVideos.clear();
	}
}