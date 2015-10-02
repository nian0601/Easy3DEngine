#pragma once
#include <D3D11.h>

namespace Easy3D
{
	struct IndexBufferWrapper
	{
		unsigned int myByteOffset;
		ID3D11Buffer* myIndexBuffer;
		DXGI_FORMAT myIndexBufferFormat;
	};
}
