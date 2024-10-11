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

	D3D_CHECK_CALL(
		pContext->IASetIndexBuffer(_indexBuffer.GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0u)
	);
}

Mesh::Mesh(VertexBuffer&& vertexBuffer, unsigned int vertexCount,
	IndexBuffer&& indexBuffer, unsigned int indexCount) :
	_vertexBuffer(std::move(vertexBuffer)),
	_vertexCount(vertexCount),
	_indexBuffer(std::move(indexBuffer)),
	_indexCount(indexCount)
{}
