#include "MeshLoader.h"

using namespace BaghdadCore;

Mesh MeshLoader::Create()
{
	// TODO: LOAD THE MESH
	// TODO: Create Vertex And Index Buffers

	return Mesh();
}

MeshLoader& MeshLoader::AllowReadWrite() noexcept
{
	_readWrite = true;

	return *this;
}

MeshLoader& MeshLoader::Load(const std::string& name) noexcept
{
	_name = std::string(name);

	return *this;
}

MeshLoader::MeshLoader(const Device& device) :
	_device(device),
	_readWrite(false)
{}
