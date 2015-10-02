#pragma once

struct ID3D11Buffer;

namespace Prism
{
	struct VertexBufferWrapper
	{
		unsigned int myByteOffset;
		int myNumberOfBuffers;
		int myStartSlot;
		unsigned int myStride;
		ID3D11Buffer* myVertexBuffer;
	};
}