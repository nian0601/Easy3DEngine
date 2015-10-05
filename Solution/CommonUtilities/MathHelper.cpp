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

		Vector2<float> RandomVector(const Vector2<float>& aMin, const Vector2<float>& aMax)
		{
			return Vector2<float>(RandomRange<float>(aMin.x, aMax.x)
				, RandomRange<float>(aMin.y, aMax.y));
		}

		Vector3<float> RandomVector(const Vector3<float>& aMin, const Vector3<float>& aMax)
		{
			return Vector3<float>(RandomRange<float>(aMin.x, aMax.x)
				, RandomRange<float>(aMin.y, aMax.y)
				, RandomRange<float>(aMin.z, aMax.z));
		}

		Vector4<float> RandomVector(const Vector4<float>& aMin, const Vector4<float>& aMax)
		{
			return Vector4<float>(RandomRange<float>(aMin.x, aMax.x)
				, RandomRange<float>(aMin.y, aMax.y)
				, RandomRange<float>(aMin.z, aMax.z)
				, RandomRange<float>(aMin.w, aMax.w));
		}
	}
}