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
	auto quaternion = XMQuaternionRotationRollPitchYawFromVector(transform.rotation);

	TransformCBuffer data = {};
	data.WorldMatrix = XMMatrixTransformation(XMVectorZero(), XMVectorSet(1, 1, 1, 1), transform.scale, XMVectorZero(), quaternion, transform.position);
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
		.PrimitiveTriangle()
		.Load();
	auto material = renderer.GetMaterialBuilder()
		.VS("Basic_goroud.vert")
		.PS("Basic_goroud.pix")
		.Build();

	_pMesh = std::move(std::make_unique<Mesh>(std::move(mesh)));
	_pMaterial = std::move(std::make_unique<Material>(std::move(material)));
}