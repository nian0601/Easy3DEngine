#pragma once

struct ID3D11Buffer;

namespace Easy3D
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