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

	// loading vertices and indices
	if(_primitiveCube)
	{
		Mesh::vertex vertex{};
		// Vertex positions, normals, and UV coordinates for a cube

		// Vertex 0
		vertex.position = DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f); // Front Bottom Right
		vertex.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);     // Normal pointing to the front
		vertex.uv = DirectX::XMFLOAT2(1.0f, 0.0f);                // UV
		mesh.push_back(vertex);

		// Vertex 1
		vertex.position = DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f); // Front Bottom Left
		vertex.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);     // Normal pointing to the front
		vertex.uv = DirectX::XMFLOAT2(0.0f, 0.0f);                // UV
		mesh.push_back(vertex);

		// Vertex 2
		vertex.position = DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f);   // Front Top Right
		vertex.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);     // Normal pointing to the front
		vertex.uv = DirectX::XMFLOAT2(1.0f, 1.0f);                // UV
		mesh.push_back(vertex);

		// Vertex 3
		vertex.position = DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f);  // Front Top Left
		vertex.normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);     // Normal pointing to the front
		vertex.uv = DirectX::XMFLOAT2(0.0f, 1.0f);                // UV
		mesh.push_back(vertex);

		// Vertex 4
		vertex.position = DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f);   // Back Bottom Right
		vertex.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);      // Normal pointing to the back
		vertex.uv = DirectX::XMFLOAT2(1.0f, 0.0f);                // UV
		mesh.push_back(vertex);

		// Vertex 5
		vertex.position = DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f);  // Back Bottom Left
		vertex.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);      // Normal pointing to the back
		vertex.uv = DirectX::XMFLOAT2(0.0f, 0.0f);                // UV
		mesh.push_back(vertex);

		// Vertex 6
		vertex.position = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);    // Back Top Right
		vertex.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);      // Normal pointing to the back
		vertex.uv = DirectX::XMFLOAT2(1.0f, 1.0f);                // UV
		mesh.push_back(vertex);

		// Vertex 7
		vertex.position = DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f);   // Back Top Left
		vertex.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);      // Normal pointing to the back
		vertex.uv = DirectX::XMFLOAT2(0.0f, 1.0f);                // UV
		mesh.push_back(vertex);

		// adding indices

// Indices for the cube with corrected winding order
		indices.push_back(0); // Triangle 1
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(2); // Triangle 2
		indices.push_back(1);
		indices.push_back(3);

		indices.push_back(4); // Triangle 3
		indices.push_back(6);
		indices.push_back(5);

		indices.push_back(5); // Triangle 4
		indices.push_back(6);
		indices.push_back(7);

		indices.push_back(0); // Triangle 5
		indices.push_back(4);
		indices.push_back(1);

		indices.push_back(1); // Triangle 6
		indices.push_back(4);
		indices.push_back(5);

		indices.push_back(2); // Triangle 7
		indices.push_back(3);
		indices.push_back(6);

		indices.push_back(6); // Triangle 8
		indices.push_back(3);
		indices.push_back(7);

		indices.push_back(0); // Triangle 9
		indices.push_back(2);
		indices.push_back(4);

		indices.push_back(4); // Triangle 10
		indices.push_back(2);
		indices.push_back(6);

		indices.push_back(1); // Triangle 11
		indices.push_back(5);
		indices.push_back(3);

		indices.push_back(3); // Triangle 12
		indices.push_back(5);
		indices.push_back(7);

	}
	else
	{
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
	}
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

MeshLoader& MeshLoader::PrimitiveCube() noexcept
{
	_primitiveCube = true;

	return *this;
}

MeshLoader& MeshLoader::Clear() noexcept
{
	_name = "";
	_primitiveCube = false;

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