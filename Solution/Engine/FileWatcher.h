#pragma once


#define WATCH_FILE(FILE, FUNCTION) (Prism::Engine::GetInstance()->GetFileWatcher()->WatchFile(FILE, std::bind(&FUNCTION, this, FILE)))

#include <functional>
#include <string>

namespace Prism
{
	class FileWatcher
	{
	public:
		FileWatcher();

		void WatchFile(const std::string& aFile, std::function<void()> aCallBack);

		void CheckFiles();

	private:
		struct FileData
		{
			std::string myFilePath;
			std::function<void()> myCallBack;
			FILETIME myFileTime;
		};

		CU::GrowingArray<FileData> myFileDatas;
	};
}