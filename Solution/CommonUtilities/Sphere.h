#pragma once
#include "Vector.h"
namespace CommonUtilities
{
	namespace Intersection
	{
		class Sphere
		{
		public:
			CommonUtilities::Vector3<float> myCenterPosition;
			float myRadius;
			float myRadiusSquared;
		};
	}
}