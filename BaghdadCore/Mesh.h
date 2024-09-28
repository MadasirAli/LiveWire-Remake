#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace BaghdadCore
{
	class Mesh final
	{
		class MeshLoader;
		friend class MeshLoader;

		class Renderer;
		friend class Renderer;

	public:
		bool IsReadWrite() const noexcept;
		Mesh(VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, bool readWrite);

	public:
		VertexBuffer _vertexBuffer;
		IndexBuffer _indexBuffer;

		bool _readWrite;
	};
}

