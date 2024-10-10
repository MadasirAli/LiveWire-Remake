#include "MeshRenderer.h"

#include "Globals.h"

using namespace LiveWireRemake;
using namespace BaghdadCore;

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

	// creating mesh and material
	auto mesh = renderer.GetMeshLoader()
		.Clear()
		.PrimitiveTriangle()
		.Load();
	auto material = renderer.GetMaterialBuilder()
		.VS("Raw.vert")
		.PS("Raw.pix")
		.Build();

	_pMesh = std::move(std::make_unique<Mesh>(std::move(mesh)));
	_pMaterial = std::move(std::make_unique<Material>(std::move(material)));
}
