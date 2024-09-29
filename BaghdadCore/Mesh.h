#pragma once

#include <vector>

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
		struct vertex final
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 uv;
		};

	private:
		void Bind(const Device& device, const DeviceContext& context) const noexcept(!_DEBUG);

		Mesh(VertexBuffer&& vertexBuffer,
			unsigned int vertexCount);

	public:
		VertexBuffer _vertexBuffer;

		unsigned int _vertexCount = 0u;
	};
}

