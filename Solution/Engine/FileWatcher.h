#pragma once


#define WATCH_FILE(FILE, FUNCTION) (Easy3D::FileWatcher::GetInstance()->WatchFile(FILE, std::bind(&FUNCTION, this, FILE)))

#include <functional>
#include <string>

namespace Easy3D
{
	class FileWatcher
	{
	public:
		static void Create();
		static void Destroy();
		static FileWatcher* GetInstance();

		void WatchFile(const std::string& aFile, std::function<void()> aCallBack);

		void CheckFiles();

	private:
		FileWatcher();

		struct FileData
		{
			std::string myFilePath;
			std::function<void()> myCallBack;
			FILETIME myFileTime;
		};

		CU::GrowingArray<FileData> myFileDatas;

		static FileWatcher* myInstance;
	};
}