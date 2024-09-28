#include "Mesh.h"

#include <memory>

using namespace BaghdadCore;

bool Mesh::IsReadWrite() const noexcept
{
	return false;
}

Mesh::Mesh(VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer,
	unsigned int vertexCount, unsigned int indexCount) :
	_vertexBuffer(std::move(vertexBuffer)),
	_indexBuffer(std::move(indexBuffer)),
	_vertexCount(vertexCount),
	_indexCount(indexCount)
{}

void Mesh::Bind(const Device& device, const DeviceContext& context) const noexcept(_DEBUG)
{
	// TODO: BIND VERTEX BUFFER
	// TODO: BIND INDEX BUFFER
}
