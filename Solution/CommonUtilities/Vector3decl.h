#pragma once


namespace CommonUtilities
{

	template<typename T>
	class Vector3
	{
	public:
		Vector3(T aX = 0, T aY = 0, T aZ = 0);
		~Vector3();


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

		union
		{
			T z;
			T myZ;
			T b;
			T myB;
		};
	};
}