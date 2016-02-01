#pragma once
#include <GrowingArray.h>
#include <unordered_map>
#include <string>

namespace Easy3D
{
	class VideoReceiver;
	class Video;
	class VideoTransmitter
	{
	public:
		static VideoTransmitter* GetInstance();
		static void Destroy();

		void AddReceiver(const std::wstring& aFeedName, VideoReceiver* aReceiver);
		void AddVideo(const std::wstring& aVideoName, const std::wstring& aFilePath);

		void StartFeed(const std::wstring& aVideoName, const std::wstring& aFeedName);
		void StartEmergencyVideo(const std::wstring& aVideoName);
		void StopEmergencyVideo();

		void Update();


	private:
		VideoTransmitter();
		~VideoTransmitter();

		std::unordered_map<std::wstring, Video*> myVideos;

		std::unordered_map<std::wstring, Video*> myFeeds;
		std::unordered_map<std::wstring, CU::GrowingArray<VideoReceiver*>> myReceivers;

		Video* myEmergencyVideo;

		static VideoTransmitter* myInstance;
	};
}