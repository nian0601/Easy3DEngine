#pragma once

namespace CommonUtilities
{
	template<typename T>
	class Greater
	{
	public:
		bool operator()(T a, T b)
		{
			return a > b;
		}
	};
}