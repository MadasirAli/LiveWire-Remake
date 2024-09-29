#include "Mesh.h"

#include <memory>

using namespace BaghdadCore;

void Mesh::Bind(const Device& device, const DeviceContext& context) const noexcept(_DEBUG)
{
	// TODO: BIND VERTEX BUFFER
	// TODO: BIND INDEX BUFFER
}

Mesh::Mesh(VertexBuffer&& vertexBuffer, unsigned int vertexCount) :
	_vertexBuffer(std::move(vertexBuffer)),
	_vertexCount(vertexCount)
{}
