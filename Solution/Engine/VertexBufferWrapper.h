#pragma once

#include "D3DPointer.h"

struct ID3D11Buffer;

namespace Easy3D
{
	struct VertexBufferWrapper
	{
		unsigned int myByteOffset;
		int myNumberOfBuffers;
		int myStartSlot;
		unsigned int myStride;
		D3DPointer<ID3D11Buffer> myVertexBuffer;
	};
}