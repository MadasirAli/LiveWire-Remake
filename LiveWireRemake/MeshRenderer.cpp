#include "MeshRenderer.h"

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
