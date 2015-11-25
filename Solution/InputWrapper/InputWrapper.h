#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Vector.h>


namespace CommonUtilities
{
	class InputWrapper
	{
	public:
		static bool Create(HWND aHwnd, HINSTANCE aHInstance, DWORD aKeyCoopFlags
			, DWORD aMouseCoopFlags, bool aUseInputRecording);
		static void Destroy();
		static InputWrapper* GetInstance();
		

		float GetMouseDX() const;
		float GetMouseDY() const;
		float GetMouseDZ() const; //scrollwheel

		const Vector2<float>& GetMousePosition() const;

		bool MouseDown(int aButton) const; //specifik musknapp nere this frame
		bool MouseUp(int aButton) const;
		bool MouseIsPressed(int aButton) const; //musknapp nere 

		bool KeyDown(unsigned int aKey) const; //Returns true if specified key is down THIS FRAME
		bool KeyUp(unsigned int aKey) const;
		bool KeyIsPressed(unsigned int aKey) const; //Returns true if specified key is down 

		void Update();
		void PauseDeltaRecording();
		void ResumeDeltaRecording();

		bool StartedRecording();
		bool StoppedRecording();
		bool StartedPlayback();

	private:
		InputWrapper(HWND aHwnd, HINSTANCE aHInstance, DWORD aKeyCoopFlags
			, DWORD aMouseCoopFlags, bool aUseInputRecording);
		~InputWrapper();

		void CapturePreviousState();

		void StartRecording();
		void StopRecording();
		void StartPlayback();
		void StopPlayback();
		void RecordInput();
		void PlaybackInput();

		void UpdateNormalInput();
		void CheckRecorderKeybinds();

		struct InputStruct
		{
			UCHAR myKeyState[256];
			UCHAR myPreviousKeyState[256];
			CU::Vector2<float> myMousePosition;
			//float myMouseWheelPosition;
			//float myMouseWheelDelta;
			_DIMOUSESTATE myMouseState;
			_DIMOUSESTATE myPreviousMouseState;
		};

		HANDLE myWriteFileHandle;
		HANDLE myReadFileHandle;

		bool myIsRecording;
		bool myIsPlayingBack;
		bool myHasRecording;

		bool myStartedRecording;
		bool myStoppedRecording;
		bool myStartedPlayback;

		InputStruct myInputData;
		LPDIRECTINPUT8 myDirectInput;
		LPDIRECTINPUTDEVICE8 myKeyboardDevice;
		LPDIRECTINPUTDEVICE8 myMouseDevice;

		/*UCHAR myKeyState[256];
		UCHAR myPreviousKeyState[256];

		DIMOUSESTATE myMouseState;
		DIMOUSESTATE myPreviousMouseState;

		Vector2<float> myMousePos;*/



		bool myIsRecordingDeltas;
		HWND myWindowHandler;
		static InputWrapper* myInstance;
	};

	inline bool InputWrapper::KeyIsPressed(unsigned int aKey) const
	{
		UCHAR key = static_cast<UCHAR>(aKey);
		return (myInputData.myKeyState[key] & 0x80) != 0;
	}

	inline bool InputWrapper::KeyDown(unsigned int aKey) const
	{
		UCHAR key = static_cast<UCHAR>(aKey);
		return ((myInputData.myKeyState[key] & 0x80) != 0 && (myInputData.myPreviousKeyState[key] & 0x80) == 0);
	}

	inline bool InputWrapper::KeyUp(unsigned int aKey) const
	{
		UCHAR key = static_cast<UCHAR>(aKey);
		return ((myInputData.myKeyState[key] & 0x80) == 0 && (myInputData.myPreviousKeyState[key] & 0x80) != 0);
	}

	inline bool InputWrapper::MouseIsPressed(int aButton) const
	{
		return (myInputData.myMouseState.rgbButtons[aButton] & 0x80) != 0;
	}

	inline bool InputWrapper::MouseDown(int aButton) const
	{
		return ((myInputData.myMouseState.rgbButtons[aButton] & 0x80) != 0 && (myInputData.myPreviousMouseState.rgbButtons[aButton] & 0x80) == 0);
	}

	inline bool InputWrapper::MouseUp(int aButton) const
	{
		return ((myInputData.myMouseState.rgbButtons[aButton] & 0x80) == 0 && (myInputData.myPreviousMouseState.rgbButtons[aButton] & 0x80) != 0);
	}

	inline float InputWrapper::GetMouseDX() const
	{
		return static_cast<float>(myInputData.myMouseState.lX);
	}

	inline float InputWrapper::GetMouseDY() const
	{
		return static_cast<float>(myInputData.myMouseState.lY);
	}

	inline float InputWrapper::GetMouseDZ() const
	{
		return static_cast<float>(myInputData.myMouseState.lZ);
	}

	inline const Vector2<float>& InputWrapper::GetMousePosition() const
	{
		return myInputData.myMousePosition;
	}

}
namespace CU = CommonUtilities;