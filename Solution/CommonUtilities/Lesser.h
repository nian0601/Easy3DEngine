#pragma once

namespace CommonUtilities
{
	template<typename T>
	class Lesser
	{
	public:
		bool operator()(T a, T b)
		{
			return a < b;
		}
	};
}