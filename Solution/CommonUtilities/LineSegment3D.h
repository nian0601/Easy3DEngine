#pragma once
#include "Vector.h"
namespace CommonUtilities
{
	namespace Intersection
	{
		class LineSegment3D
		{
		public:
			CommonUtilities::Vector3<float> myStartPos;
			CommonUtilities::Vector3<float> myEndPos;
		};
	}
}