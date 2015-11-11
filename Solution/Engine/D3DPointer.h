#pragma once

#include <DL_Assert.h>

namespace Easy3D
{
	

	template<typename T>
	class D3DPointer
	{
	public:
		explicit D3DPointer() 
			: myPtr(nullptr), myCount(nullptr) {};
		explicit D3DPointer(T* aData) 
			: myPtr(aData), myCount(new int(1)) {};
		D3DPointer(const D3DPointer<T>& aPtr) 
			: myPtr(aPtr.myPtr), myCount(aPtr.myCount) { *myCount += 1; };

		~D3DPointer();

		D3DPointer<T>& operator=(const D3DPointer<T>& aPtr);

		T* operator->(){ DL_ASSERT_EXP(myPtr != nullptr, "D3DPointer, tried to -> a nullptr");  return myPtr; };
		T& operator*() { return *myPtr; };
		T* Get() { return myPtr; };
		T* Get() const { return myPtr; };
		void Set(T* aPtr);

		void Release();
		
	private:
		T* myPtr;
		int* myCount;
	};

	template<typename T>
	D3DPointer<T>::~D3DPointer()
	{
		if (myPtr == nullptr)
		{
			return;
		}

		*myCount -= 1;
		DL_ASSERT_EXP(*myCount >= 0, "D3DPointer: myCount is less than 0.");
		if (*myCount <= 0 && myPtr != nullptr)
		{
			myPtr->Release();
			myPtr = nullptr;

			delete myCount;
			myCount = nullptr;
		}
	}

	template<typename T>
	D3DPointer<T>& D3DPointer<T>::operator=(const D3DPointer<T>& aPtr)
	{
		if (myCount != nullptr && *myCount > 1)
		{
			*myCount -= 1;
		}
		else
		{
			myPtr = nullptr;
			delete myCount;
		}

		myPtr = aPtr.myPtr;
		myCount = aPtr.myCount;
		*myCount += 1;
		return *this;
	}

	template<typename T>
	void D3DPointer<T>::Set(T* aPtr)
	{
		if (myCount != nullptr && *myCount > 1)
		{
			*myCount -= 1;

			if (*myCount <= 0)
			{
				myPtr = nullptr;
				delete myCount;
				myCount = nullptr;
			}
		}

		if (myCount == nullptr)
		{
			myCount = new int(1);
		}
		else
		{
			*myCount = 1;
		}

		myPtr = aPtr;
	}

	template<typename T>
	void D3DPointer<T>::Release()
	{
		DL_ASSERT_EXP(myPtr != nullptr, "D3DPointer: Tried to Release a nullptr");
		myPtr->Release();
		myPtr = nullptr;
		*myCount = 0;
	}
}