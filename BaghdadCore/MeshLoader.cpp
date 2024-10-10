#include "MeshLoader.h"

#include <fstream>

#include "Globals.h"
#include "GraphicsError.h"

using namespace BaghdadCore;

Mesh MeshLoader::Load()
{
	using namespace Microsoft::WRL;

	std::vector<Mesh::vertex> mesh{};
	if (_primitiveQuad)
	{
		auto vertex = Mesh::vertex{};

		vertex.position = DirectX::XMFLOAT3(-1, 1, 0);
		vertex.normal = DirectX::XMFLOAT3(0, 0, -1);
		mesh.push_back(vertex);

		vertex.position = DirectX::XMFLOAT3(1, 1, 0);
		mesh.push_back(vertex);

		vertex.position = DirectX::XMFLOAT3(1, -1, 0);
		mesh.push_back(vertex);

		vertex.position = DirectX::XMFLOAT3(-1, 1, 0);
		mesh.push_back(vertex);

		vertex.position = DirectX::XMFLOAT3(1, -1, 0);
		mesh.push_back(vertex);

		vertex.position = DirectX::XMFLOAT3(-1, -1, 0);
		mesh.push_back(vertex);
	}
	else if (_primitiveTriangle)
	{
		auto vertex = Mesh::vertex{};

		vertex.position = DirectX::XMFLOAT3(-1, -1, 0);
		vertex.normal = DirectX::XMFLOAT3(0, 0, -1);
		mesh.push_back(vertex);

		vertex.position = DirectX::XMFLOAT3(0, 1, 0);
		mesh.push_back(vertex);

		vertex.position = DirectX::XMFLOAT3(1, -1, 0);
		mesh.push_back(vertex);
	}
	else
	{
		mesh = _stlLoader.Load(_name);
	}
	
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeof(Mesh::vertex) * (UINT)mesh.size();
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0u;
	desc.StructureByteStride = sizeof(Mesh::vertex);

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = (void*)mesh.data();

	ComPtr<ID3D11Buffer> pBuffer{};
	D3D_CALL(
	_device.GetComPtr()->CreateBuffer(
		&desc, &data, pBuffer.ReleaseAndGetAddressOf()));

	_logger.WriteLine("Buffer Created." + 
		std::string("\nWidth: ") + std::to_string(desc.ByteWidth));

	return Mesh(
		std::move(VertexBuffer(
			std::move(Buffer(
				std::move(pBuffer), Resource::View())))), (unsigned int)mesh.size());
}

MeshLoader& MeshLoader::Clear() noexcept
{
	_name = "";
	_primitiveQuad = false;
	_primitiveTriangle = false;

	return *this;
}

MeshLoader& MeshLoader::FromFile(const std::string& name) noexcept
{
	_name = std::string(name);

	return *this;
}

MeshLoader& MeshLoader::PrimitiveQuad() noexcept
{
	_primitiveQuad = true;

	return *this;
}

MeshLoader& MeshLoader::PrimitiveTriangle() noexcept
{
	_primitiveTriangle = true;

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
	std::ifstream stream{name, std::ios::binary};

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

	auto vertices = std::vector<Mesh::vertex>();
	vertices.reserve(length);

	// opening facets
	for (auto i = 0u; i < length; ++i)
	{
		const auto* const pData = ((float*)(pFile.get() + count));

		// reading current face normal
		DirectX::XMFLOAT3 normal = 
			DirectX::XMFLOAT3(pData[0], pData[1], pData[2]);

		count += sizeof(float) * 3;

		// reading face vertices
		for (auto v = 0; v < 3; v++)
		{
			Mesh::vertex vertex{};
			vertex.normal = normal;
			vertex.position = DirectX::XMFLOAT3(
				pData[0], pData[1], pData[2]);

			count += sizeof(float) * 3;

			vertices.emplace_back(std::move(vertex));
		}

		// padding
		count += sizeof(wchar_t);
	}

	return vertices;
}
