#include "stdafx.h"
#include "MathHelper.h"
namespace CommonUtilities
{
	namespace Math
	{
		int CapValue(int aMinValue, int aMaxValue, int aValueToCap)
		{
			if (aValueToCap > aMaxValue)
			{
				aValueToCap = aMaxValue;
			}
			else if (aValueToCap < aMinValue)
			{
				aValueToCap = aMinValue;
			}

			return aValueToCap;
		}

		float CapValue(const float aMinValue, const float aMaxValue, float aValueToCap)
		{
			if (aValueToCap > aMaxValue)
			{
				aValueToCap = aMaxValue;
			}
			else if (aValueToCap < aMinValue)
			{
				aValueToCap = aMinValue;
			}

			return aValueToCap;
		}

		float Saturate(const float aValue)
		{
			return CapValue(0.f, 1.f, aValue);
		}
	}
}