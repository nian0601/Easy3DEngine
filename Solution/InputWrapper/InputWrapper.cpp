#include "InputWrapper.h"

CommonUtilities::InputWrapper::InputWrapper()
{

}

void CommonUtilities::InputWrapper::Init(HWND aHwnd, HINSTANCE aHInstance, DWORD aKeyCoopFlags, DWORD aMouseCoopFlags)
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

	Update();
}



CommonUtilities::InputWrapper::~InputWrapper()
{
	myKeyboardDevice->Unacquire();
	myMouseDevice->Unacquire();
}

void CommonUtilities::InputWrapper::Update()
{
	CapturePreviousState();
	HRESULT hr = myKeyboardDevice->GetDeviceState(sizeof(myKeyState), reinterpret_cast<void**>(&myKeyState));

	if (FAILED(hr))
	{
		ZeroMemory(myKeyState, sizeof(myKeyState));

		myKeyboardDevice->Acquire();
	}

	hr = myMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<void**>(&myMouseState));
	if (FAILED(hr))
	{
		ZeroMemory(&myMouseState, sizeof(myMouseState));

		myMouseDevice->Acquire();
	}

	tagPOINT cursorPoint;
	GetCursorPos(&cursorPoint);
	ScreenToClient(myWindowHandler, &cursorPoint);
	myMousePos.x = static_cast<float>(cursorPoint.x);
	myMousePos.y = static_cast<float>(cursorPoint.y);

	if (myIsRecordingDeltas == false)
	{
		myMouseState.lX = 0;
		myMouseState.lY = 0;
	}

	//myMousePos.x += myMouseState.lX;
	//myMousePos.y += myMouseState.lY;
	//myMousePos.z += myMouseState.lZ;
}


void CommonUtilities::InputWrapper::PauseDeltaRecording()
{
	myIsRecordingDeltas = false;
}


void CommonUtilities::InputWrapper::ResumeDeltaRecording()
{
	myIsRecordingDeltas = true;
}

const CommonUtilities::Vector2<float>& CommonUtilities::InputWrapper::GetMousePosition() const
{
	return myMousePos;
}

void CommonUtilities::InputWrapper::CapturePreviousState()
{
	memcpy_s(myPreviousKeyState, sizeof(myPreviousKeyState), myKeyState, sizeof(myKeyState));
	memcpy_s(&myPreviousMouseState, sizeof(myPreviousMouseState), &myMouseState, sizeof(myMouseState));
}

bool CommonUtilities::InputWrapper::KeyIsPressed(unsigned int aKey) const
{
	UCHAR key = static_cast<UCHAR>(aKey);
	return (myKeyState[key] & 0x80) != 0;
}

bool CommonUtilities::InputWrapper::KeyDown(unsigned int aKey) const
{
	UCHAR key = static_cast<UCHAR>(aKey);
	return ((myKeyState[key] & 0x80) != 0 && (myPreviousKeyState[key] & 0x80) == 0);
}

bool CommonUtilities::InputWrapper::KeyUp(unsigned int aKey) const
{
	UCHAR key = static_cast<UCHAR>(aKey);
	return ((myKeyState[key] & 0x80) == 0 && (myPreviousKeyState[key] & 0x80) != 0);
}

bool CommonUtilities::InputWrapper::MouseIsPressed(int aButton) const
{
	return (myMouseState.rgbButtons[aButton] & 0x80) != 0;
}

bool CommonUtilities::InputWrapper::MouseDown(int aButton) const
{
	return ((myMouseState.rgbButtons[aButton] & 0x80) != 0 && (myPreviousMouseState.rgbButtons[aButton] & 0x80) == 0);
}

bool CommonUtilities::InputWrapper::MouseUp(int aButton) const
{
	return ((myMouseState.rgbButtons[aButton] & 0x80) == 0 && (myPreviousMouseState.rgbButtons[aButton] & 0x80) != 0);
}

float CommonUtilities::InputWrapper::GetMouseDX() const
{
	return static_cast<float>(myMouseState.lX);
}

float CommonUtilities::InputWrapper::GetMouseDY() const
{
	return static_cast<float>(myMouseState.lY);
}

float CommonUtilities::InputWrapper::GetMouseDZ() const
{
	return static_cast<float>(myMouseState.lZ);
}