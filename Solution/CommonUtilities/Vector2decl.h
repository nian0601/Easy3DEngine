#pragma once
#include "Vector3decl.h"

namespace CommonUtilities
{
	template<typename T>

	class Vector2
	{
	public:
		Vector2(T aX = 0, T aY = 0);
		//Vector2(Vector3<T> aVector);
		~Vector2();

		Vector2<T> operator-()
		{
			Vector2<T> tmp = *this;
			tmp.x = -x;
			tmp.y = -y;
			return tmp;
		}

		union
		{
			T x;
			T myX;
			T r;
			T myR;
		};

		union
		{
			T y;
			T myY;
			T g;
			T myG;
		};
	};
}

namespace CU = CommonUtilities;