/*
This class is the internal logic for playing a video
*/

#pragma once
#include <wtypes.h>
#include <wmsdk.h>
#include <thread>
#include <mutex>

#define MAX_AUDIO_STREAMS 5
#define MAX_VIDEO_STREAMS 5

namespace Easy3D
{
	class WMVVideoInstance
	{
	public:
		WMVVideoInstance();
		~WMVVideoInstance();
		void Release();

		bool Init(LPCWSTR lpszFileName);
		void Render();

		void Play();
		void Stop();
		bool IsDone();


		ID3D11ShaderResourceView* GetView(){ return myShaderResource; }
	private:

		int NextFrame();
		void GetPixel(const unsigned int x, const unsigned int y, unsigned char& red, unsigned char& green, unsigned char& blue);
		int SaveCurrentFrameToBmp(char* filename);

		BITMAPINFOHEADER myBitmapInfoHdr;
		WMVIDEOINFOHEADER myTheVideoInfoHeader;

		WM_MEDIA_TYPE* mytheMediaType;
		DWORD myDwrdBitmapBufferLength;
		DWORD mytheOutputsCount;
		HRESULT myHResult;

		int myAudioOutputNumber;
		int myFramesRead;
		int myRow_increment;
		int myWidth;
		int myHeight;
		int myBytes_per_pixel;
		int m_iAudioStreamNumber;
		QWORD m_qwTotalTimeInSeconds;
		QWORD m_qwCurrrentPostionInSeconds;
		WORD myVideoStreamNumber;
		bool myIsStreamOpen;
		bool myOpenSuccesfully;
		bool myThreadIsDone;
		bool myCanChangeFrame;
		bool myIsRenderingFrame;
		bool myIsInitialized;
		bool myShouldStopThread;
		bool myIsStreamDone;

		int *myBuffer;
		ID3D11ShaderResourceView* myShaderResource;
		ID3D11Texture2D *myTexture;
		std::wstring myFileName;
		std::thread* myCruncherThread;
		static std::mutex myGlobalPagesMutex;
		BYTE* myBitmapBuffer;
		IWMSyncReader* mySyncReader;

		static void StartCruncher(WMVVideoInstance* aInstance);
		static void StartUpdateLoop(WMVVideoInstance* aInstance);
	};
}