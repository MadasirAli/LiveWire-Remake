#include "Mesh.h"

#include <memory>

using namespace BaghdadCore;

void Mesh::Bind(const Device& device, const DeviceContext& context) const NOEXCEPT
{
	// binding vertex buffer
	const auto& pContext = context.GetComPtr();

	const UINT pStride = sizeof(vertex);
	const UINT pOffset = 0u;

	D3D_CHECK_CALL(
		pContext->IASetVertexBuffers(0u, 1u, _vertexBuffer.GetComPtr().GetAddressOf(),
			&pStride, &pOffset)
	);
}

Mesh::Mesh(VertexBuffer&& vertexBuffer, unsigned int vertexCount) :
	_vertexBuffer(std::move(vertexBuffer)),
	_vertexCount(vertexCount)
{}
