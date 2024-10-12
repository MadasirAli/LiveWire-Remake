#include "MeshLoader.h"

#include <fstream>

#include "Globals.h"
#include "GraphicsError.h"

#include "OBJ_Loader.h"

using namespace BaghdadCore;

Mesh MeshLoader::Load()
{
	using namespace Microsoft::WRL;

	std::vector<Mesh::vertex> mesh{};
	std::vector<unsigned int> indices{};

	// TODO: LOADING MESH AND INDCIEs 
	objl::Loader loader{};
	bool result = loader.LoadFile(_name);
	if (result == false)
	{
		THROW_BERROR("Model did not found or unable to load modal.");
	}

	for (const auto& v : loader.LoadedVertices)
	{
		Mesh::vertex vertex{};
		vertex.normal = DirectX::XMFLOAT3(v.Normal.X, v.Normal.Y, v.Normal.Z);
		vertex.position = DirectX::XMFLOAT3(v.Position.X, v.Position.Y, v.Position.Z);

		vertex.uv = DirectX::XMFLOAT2(v.TextureCoordinate.X, v.TextureCoordinate.Y);

		mesh.emplace_back(std::move(vertex));
	}
	indices = std::move(loader.LoadedIndices);

	_logger.WriteLine("Mesh Loader: " + _name);
	//
	
	// creating vertex buffer
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeof(Mesh::vertex) * (UINT)mesh.size();
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0u;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = (void*)mesh.data();

	ComPtr<ID3D11Buffer> pVertexBuffer{};
	D3D_CALL(
	_device.GetComPtr()->CreateBuffer(
		&desc, &data, pVertexBuffer.ReleaseAndGetAddressOf()));

	_logger.WriteLine("Buffer Created." + 
		std::string("\nWidth: ") + std::to_string(desc.ByteWidth));

	// creating index buffer
	desc.ByteWidth = sizeof(unsigned int) * (UINT)indices.size();
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

	data.pSysMem = (void*)indices.data();

	ComPtr<ID3D11Buffer> pIndexBuffer{};
	D3D_CALL(
		_device.GetComPtr()->CreateBuffer(
			&desc, &data, pIndexBuffer.ReleaseAndGetAddressOf()));

	_logger.WriteLine("Buffer Created." +
		std::string("\nWidth: ") + std::to_string(desc.ByteWidth));

	return Mesh(
		std::move(VertexBuffer(
			std::move(Buffer(
				std::move(pVertexBuffer), Resource::View())))), (unsigned int)mesh.size(),
		std::move(IndexBuffer(
			std::move(Buffer(
				std::move(pIndexBuffer), Resource::View())))), (unsigned int)indices.size());
}

MeshLoader& MeshLoader::Clear() noexcept
{
	_name = "";

	return *this;
}

MeshLoader& MeshLoader::FromFile(const std::string& name) noexcept
{
	_name = std::string(name);

	return *this;
}

MeshLoader::MeshLoader(const Device& device) :
	_device(device),
	_logger(Globals::GetLogger())
{}