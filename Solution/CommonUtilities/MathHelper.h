#ifndef MATH_HELPER_HEADER
#define MATH_HELPER_HEADER
#include <cstdlib>

namespace CommonUtilities
{
	namespace Math
	{
		int CapValue(int aMinValue, int aMaxValue, int aValueToCap);
		float CapValue(const float aMinValue, const float aMaxValue, float aValueToCap);

		float Saturate(const float aValue);

		template <typename T>
		inline T RandomRange(const T aMinValue, const T aMaxValue)
		{
			float alpha = static_cast<float>(rand() % 10000) / 10000.f;
			return Lerp<T>(aMinValue, aMaxValue, alpha);
		}

		template <typename T>
		inline T Lerp(const T aStartValue, const T aEndValue, float aAlpha)
		{
			aAlpha = Saturate(aAlpha);
			return static_cast<T>(aStartValue + (aEndValue - aStartValue) * aAlpha);
		}
	}
}
namespace CU = CommonUtilities;

#endif