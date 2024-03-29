#pragma once
#include <cassert>
#define SA_TEMPLATE template<typename Type, int Size>
#define SA_TYPE StaticArray<Type, Size>

namespace CommonUtilities
{
	SA_TEMPLATE
	class StaticArray
	{
	public:
		StaticArray();
		StaticArray(const StaticArray& aStaticArray);

		~StaticArray();

		StaticArray& operator=(const StaticArray& aStaticArray);

		inline const Type& operator[](const int& aIndex) const;
		inline Type& operator[](const int& aIndex);


		// Utility functions
		inline void Insert(int aIndex, Type& aObject);
		inline void DeleteAll();
	private:
		Type myData[Size];
	};
}

namespace CU = CommonUtilities;

namespace CommonUtilities
{
	SA_TEMPLATE
	SA_TYPE::StaticArray()
	{

	}

	SA_TEMPLATE
	SA_TYPE::~StaticArray()
	{

	}

	SA_TEMPLATE
	SA_TYPE::StaticArray(const StaticArray& aStaticArray)
	{
		operator=(aStaticArray);
	}

	SA_TEMPLATE
	SA_TYPE& SA_TYPE::operator=(const StaticArray& aStaticArray)
	{
		for (int i = 0; i < Size; ++i)
		{
			myData[i] = aStaticArray.myData[i];
		}
		return *this;
	}

	SA_TEMPLATE
	inline const Type& SA_TYPE::operator[](const int& aIndex) const
	{
		assert(aIndex >= 0 && "Index has to be 0 or more.");
		assert(aIndex < Size && "a index out of bounds!");
		return myData[aIndex];
	}

	SA_TEMPLATE
	inline Type& SA_TYPE::operator[](const int& aIndex)
	{
		assert(aIndex >= 0 && "Index has to be 0 or more.");
		assert(aIndex < Size && "a index out of bounds!");
		return myData[aIndex];
	}

	SA_TEMPLATE
	inline void SA_TYPE::Insert(int aIndex, Type& aObject)
	{
		assert(aIndex >= 0 && "Index has to be 0 or more.");
		assert(aIndex < Size && "a index out of bounds!");
		for (int i = Size - 2; i >= aIndex; --i)
		{
			myData[i + 1] = myData[i];
		}
		myData[aIndex] = aObject;
	}

	SA_TEMPLATE
	inline void SA_TYPE::DeleteAll()
	{
		for (int i = 0; i < Size; ++i)
		{
			delete myData[i];
			myData[i] = nullptr;
		}
	}
}

