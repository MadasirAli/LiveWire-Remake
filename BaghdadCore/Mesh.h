#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Device.h"
#include "DeviceContext.h"

namespace BaghdadCore
{
	class Mesh final
	{
		friend class MeshLoader;

		friend class Renderer;

	public:
		bool IsReadWrite() const noexcept;

	private:
		void Bind(const Device& device, const DeviceContext& context) const noexcept(_DEBUG);

		Mesh(VertexBuffer&& vertexBuffer,
			IndexBuffer&& indexBuffer,
			unsigned int vertexCount,
			unsigned int indexCount);

	public:
		VertexBuffer _vertexBuffer;
		IndexBuffer _indexBuffer;

		unsigned int _vertexCount = 0u;
		unsigned int _indexCount = 0u;
	};
}

