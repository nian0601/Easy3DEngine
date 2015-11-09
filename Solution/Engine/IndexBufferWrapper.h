#pragma once

#include <D3D11.h>
#include "D3DPointers.h"

namespace Easy3D
{
	struct IndexBufferWrapper
	{
		unsigned int myByteOffset;
		D3DPointer<ID3D11Buffer> myIndexBuffer;
		DXGI_FORMAT myIndexBufferFormat;
	};
}
