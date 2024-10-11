#include "MeshRenderer.h"

#include "Globals.h"
#include "TransformCBuffer.h"

using namespace LiveWireRemake;
using namespace BaghdadCore;

void MeshRenderer::OnPreRender(std::weak_ptr<Entity>& pEntity)
{
	using namespace DirectX;

	auto& transform = pEntity.lock()->GetTransform();

	// updating transform c buffer
	TransformCBuffer data = {};
	data.WorldMatrix = XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(),
		XMVectorSet(transform.scale.x, transform.scale.y, transform.scale.z, 1), XMVectorZero(), transform.Quaternion(),
		XMVectorSet(transform.position.x, transform.position.y, transform.position.z, 1));
	data.WorldMatrix = XMMatrixTranspose(data.WorldMatrix);

	const auto ptr = (TransformCBuffer*)_pBuffer->Map(D3D11_MAP_WRITE_DISCARD);
	ptr[0] = data;

	_pBuffer->UnMap();
}

ConstantBuffer& MeshRenderer::GetTransformCBuffer() noexcept
{
	return *_pBuffer;
}

void MeshRenderer::SetMaterial(Material&& material) noexcept
{
    _pMaterial = std::move(std::make_unique<Material>(std::move(material)));
}

void MeshRenderer::SetMesh(Mesh&& mesh) noexcept
{
    _pMesh = std::move(std::make_unique<Mesh>(std::move(mesh)));
}

Material& MeshRenderer::GetMaterial() noexcept
{
    return *_pMaterial;
}

Mesh& MeshRenderer::GetMesh() noexcept
{
    return *_pMesh;
}

MeshRenderer::MeshRenderer()
{
	auto& renderer = LiveWireRemake::Globals::GetInstance().GetRenderer();

	// creating transform cBuffer
	const TransformCBuffer pData = {};
	_pBuffer = std::make_unique<BaghdadCore::ConstantBuffer>(std::move(
		Globals::GetInstance()
		.GetRenderer()
		.GetBufferBuilder()
		.Clear()
		.Write()
		.InitialData((char*)&pData, sizeof(pData))
		.BuildCBuffer()
	));

	// creating mesh and material
	auto mesh = renderer.GetMeshLoader()
		.Clear()
		.FromFile("Resources\\Models\\cube.obj")
		.Load();
	auto material = renderer.GetMaterialBuilder()
		.Clear()
		.VS("Basic_goroud.vert")
		.PS("Basic_goroud.pix")
		.Build();

	// binding default texture
	auto defaultTexture = renderer.GetTextureBuilder()
		.Clear()
		.FromFile("Resources\\Textures\\landscape.bmp")
		.ViewFlag(BaghdadCore::Resource::View::Type::SRV)
		.Build();

	material.SetPSTexture("albedo", defaultTexture);

	_pMesh = std::move(std::make_unique<Mesh>(std::move(mesh)));
	_pMaterial = std::move(std::make_unique<Material>(std::move(material)));
}