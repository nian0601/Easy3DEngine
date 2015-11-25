#include "InputWrapper.h"

namespace CommonUtilities
{
	InputWrapper* InputWrapper::myInstance = nullptr;
	bool InputWrapper::Create(HWND aHwnd, HINSTANCE aHInstance, DWORD aKeyCoopFlags
		, DWORD aMouseCoopFlags, bool aUseInputRecording)
	{
		myInstance = new InputWrapper(aHwnd, aHInstance, aKeyCoopFlags, aMouseCoopFlags, aUseInputRecording);
		return true;
	}

	void InputWrapper::Destroy()
	{
		delete myInstance;
	}

	InputWrapper* InputWrapper::GetInstance()
	{
		return myInstance;
	}

	InputWrapper::InputWrapper(HWND aHwnd, HINSTANCE aHInstance, DWORD aKeyCoopFlags
		, DWORD aMouseCoopFlags, bool aUseInputRecording)
	{
		DirectInput8Create(aHInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&myDirectInput), 0);

		myDirectInput->CreateDevice(GUID_SysKeyboard, &myKeyboardDevice, nullptr);
		myDirectInput->CreateDevice(GUID_SysMouse, &myMouseDevice, nullptr);

		myKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
		myMouseDevice->SetDataFormat(&c_dfDIMouse);

		myKeyboardDevice->SetCooperativeLevel(aHwnd, aKeyCoopFlags);
		myMouseDevice->SetCooperativeLevel(aHwnd, aMouseCoopFlags);

		myKeyboardDevice->Acquire();
		myMouseDevice->Acquire();

		myWindowHandler = aHwnd;

		myIsRecordingDeltas = true;

		myIsRecording = false;
		myIsPlayingBack = false;
		myHasRecording = false;

		myStartedRecording = false;
		myStoppedRecording = false;
		myStartedPlayback = false;

		if (aUseInputRecording == true)
		{
			myIsPlayingBack = true;
			myHasRecording = true;
			StartPlayback();
		}

		Update();
	}

	InputWrapper::~InputWrapper()
	{
		if (myIsRecording == true)
		{
			RecordInput();
			StopRecording();
		}
		myKeyboardDevice->Unacquire();
		myMouseDevice->Unacquire();
	}

	void InputWrapper::Update()
	{
		myStartedRecording = false;
		myStoppedRecording = false;
		myStartedPlayback = false;

		UpdateNormalInput();

		CheckRecorderKeybinds();

		if (myIsRecording == true)
		{
			RecordInput();
		}
		else if (myIsPlayingBack == true)
		{
			PlaybackInput();
		}
	}

	void InputWrapper::UpdateNormalInput()
	{
		CapturePreviousState();
		HRESULT hr = myKeyboardDevice->GetDeviceState(sizeof(myInputData.myKeyState), reinterpret_cast<void**>(&myInputData.myKeyState));

		if (FAILED(hr))
		{
			ZeroMemory(myInputData.myKeyState, sizeof(myInputData.myKeyState));

			myKeyboardDevice->Acquire();
		}

		hr = myMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<void**>(&myInputData.myMouseState));
		if (FAILED(hr))
		{
			ZeroMemory(&myInputData.myMouseState, sizeof(myInputData.myMouseState));

			myMouseDevice->Acquire();
		}

		tagPOINT cursorPoint;
		GetCursorPos(&cursorPoint);
		ScreenToClient(myWindowHandler, &cursorPoint);
		myInputData.myMousePosition.x = static_cast<float>(cursorPoint.x);
		myInputData.myMousePosition.y = static_cast<float>(cursorPoint.y);

		if (myIsRecordingDeltas == false)
		{
			myInputData.myMouseState.lX = 0;
			myInputData.myMouseState.lY = 0;
		}

		//myMousePos.x += myMouseState.lX;
		//myMousePos.y += myMouseState.lY;
		//myMousePos.z += myMouseState.lZ;
	}

	void InputWrapper::PauseDeltaRecording()
	{
		myIsRecordingDeltas = false;
	}

	void InputWrapper::ResumeDeltaRecording()
	{
		myIsRecordingDeltas = true;
	}

	void InputWrapper::CapturePreviousState()
	{
		memcpy_s(myInputData.myPreviousKeyState, sizeof(myInputData.myPreviousKeyState)
			, myInputData.myKeyState, sizeof(myInputData.myKeyState));
		memcpy_s(&myInputData.myPreviousMouseState, sizeof(myInputData.myPreviousMouseState)
			, &myInputData.myMouseState, sizeof(myInputData.myMouseState));
	}

	void InputWrapper::CheckRecorderKeybinds()
	{
		if (KeyDown(DIK_F12) && myIsPlayingBack == true)
		{
			StopPlayback();
		}
		else
		{
			if (KeyUp(DIK_L) && myIsPlayingBack == false)
			{
				if (myIsRecording == false)
				{
					StartRecording();
				}
				else
				{
					StopRecording();
				}
			}
			else if (KeyUp(DIK_P) && myIsRecording == false)
			{
				if (myIsPlayingBack == false)
				{
					StartPlayback();
				}
				else
				{
					StopPlayback();
				}
			}
		}
	}

	void InputWrapper::StartRecording()
	{
		OutputDebugString("Started Recording\n");
		myWriteFileHandle = CreateFile("inputRecording.ird", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		myIsRecording = true;
		myHasRecording = false;

		myStartedRecording = true;
	}

	void InputWrapper::StopRecording()
	{
		OutputDebugString("Stopped Recording\n");
		CloseHandle(myWriteFileHandle);
		myIsRecording = false;
		myHasRecording = true;

		myStoppedRecording = true;
	}

	void InputWrapper::StartPlayback()
	{
		if (myHasRecording == false)
		{
			return;
		}

		OutputDebugString("Started Playback\n");
		myReadFileHandle = CreateFile("inputRecording.ird", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		myIsPlayingBack = true;

		myStartedPlayback = true;
	}

	void InputWrapper::StopPlayback()
	{
		OutputDebugString("Stopped Playback\n");
		CloseHandle(myReadFileHandle);
		myIsPlayingBack = false;
	}

	void InputWrapper::RecordInput()
	{
		DWORD bytesWritten;
		WriteFile(myWriteFileHandle, &myInputData, sizeof(myInputData), &bytesWritten, 0);
	}

	void InputWrapper::PlaybackInput()
	{
		DWORD bytesWritten;
		if (ReadFile(myReadFileHandle, &myInputData, sizeof(myInputData), &bytesWritten, 0))
		{
			if (bytesWritten == 0)
			{
				StopPlayback();
				StartPlayback();
			}
		}
	}
}