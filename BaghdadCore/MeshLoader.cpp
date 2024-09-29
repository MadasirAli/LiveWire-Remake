#include "MeshLoader.h"

#include <fstream>

#include "Globals.h"
#include "GraphicsError.h"

using namespace BaghdadCore;

Mesh MeshLoader::Load()
{
	// TODO: LOAD THE MESH
	const auto mesh = _stlLoader.Load(_name);
	// TODO: Create Vertex And Index Buffers

	return Mesh();
}

MeshLoader& MeshLoader::Load(const std::string& name) noexcept
{
	_name = std::string(name);

	return *this;
}

MeshLoader::MeshLoader(const Device& device) :
	_device(device),
	_logger(Globals::GetLogger())
{}

std::vector<Mesh::vertex> 
MeshLoader::STLLoader::Load(const std::string& name) const
{
	// loading in memory
	std::ifstream stream{name};

	if (stream.is_open() == false)
	{
		THROW_BERROR("Failed to open file: " + name);
	}

	stream.seekg(0, std::ios::end);
	const auto size = stream.tellg();
	stream.seekg(0, std::ios::beg);

	std::unique_ptr<char[]> pFile{};
	try
	{
		pFile = std::make_unique<char[]>(size);
	}
	catch(...)
	{
		stream.close();
		THROW_BERROR("Failed to allocate memory");
	}

	stream.read(pFile.get(), size);
	stream.close();

	// parsing
	
	auto count = 80;					// header offset can be ignored
	// number of faces or triangles
	const auto length = ((unsigned long*)(pFile.get() + count))[0];
	count += sizeof(unsigned long);		// 4

	auto vertices = std::vector<Mesh::vertex>(length);

	// opening facets
	for (auto i = 0; i < length; ++i)
	{
		const auto* const pData = ((float*)(pFile.get() + count));

		// reading current face normal
		DirectX::XMVECTOR normal{};
		normal.m128_f32[0] = pData[0];
		normal.m128_f32[1] = pData[1];
		normal.m128_f32[2] = pData[2];
		normal.m128_f32[3] = 0;

		count += sizeof(float) * 3;

		// reading face vertices
		for (auto v = 0; v < 3; v++)
		{
			Mesh::vertex vertex = { 0 };
			vertex.normal = normal;

			vertex.position.m128_f32[0] = pData[0];
			vertex.position.m128_f32[1] = pData[1];
			vertex.position.m128_f32[2] = pData[2];
			vertex.position.m128_f32[3] = 0;

			count += sizeof(float) * 3;

			vertices.push_back(std::move(vertex));
		}

		// padding
		count += sizeof(wchar_t);
	}

	return vertices;
}
