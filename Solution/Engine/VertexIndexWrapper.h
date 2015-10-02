#pragma once
#include <D3D11.h>

namespace Easy3D
{
	struct VertexIndexWrapper
	{
		DXGI_FORMAT myFormat;
		char* myIndexData;
		int myNumberOfIndices;
		int mySize;
	};
}