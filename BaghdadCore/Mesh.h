#pragma once

#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Device.h"
#include "DeviceContext.h"
#include "GraphicsError.h"

namespace BaghdadCore
{
	class Mesh final
	{
		friend class MeshLoader;
		friend class Renderer;

	public:
		struct vertex final
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 uv;
		};

	private:
		void Bind(const Device& device, const DeviceContext& context) const NOEXCEPT;

		Mesh(VertexBuffer&& vertexBuffer, unsigned int vertexCount,
			IndexBuffer&& indexBuffer, unsigned int indexCount);

	public:
		VertexBuffer _vertexBuffer;
		IndexBuffer _indexBuffer;

		unsigned int _vertexCount = 0u;
		unsigned int _indexCount = 0u;
	};
}

