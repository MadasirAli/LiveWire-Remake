#include "Mesh.h"

#include <memory>

using namespace BaghdadCore;

bool Mesh::IsReadWrite() const noexcept
{
	return false;
}

Mesh::Mesh(VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer, bool readWrite) :
	_vertexBuffer(std::move(vertexBuffer)),
	_indexBuffer(std::move(indexBuffer))
{
	_readWrite = readWrite;
}
