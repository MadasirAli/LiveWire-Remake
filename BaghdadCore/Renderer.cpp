#include "Renderer.h"

#include "BaghdadError.h"
#include "GraphicsError.h"

using namespace BaghdadCore;

void Renderer::DrawMesh(const Mesh& mesh, const Material& material) const
{
	const auto& context = _pDevice->GetDeviceContext();

	// TODO: BIND MESH		
	mesh.Bind(*_pDevice, context);
	// TODO: BIND MATERIAL
	material.Bind(*_pDevice, context);
	// TODO: ISSUE DRAW CALL

	D3D_CHECK_CALL(
		context.GetComPtr()->Draw(mesh._vertexCount, 0u);
	);
}

TextureBuilder& Renderer::GetTextureBuilder() const noexcept
{
	return *_pTextureBuilder;
}

MaterialBuilder& Renderer::GetMaterialBuilder() const noexcept
{
	return *_pMaterialBuilder;
}

MeshLoader& Renderer::GetMeshLoader() const noexcept
{
	return *_pMeshLoader;
}

Renderer::Renderer()
{
	auto cards = _factory.EnumGraphicCards();
	if (cards.size() < 1)
	{
		THROW_BERROR("No Graphics Capable Adapter (Graphics Card) found !");
	}

	_pCard = std::make_unique<GraphicsCard>(std::move(cards[0]));
	_pDevice = std::make_unique<Device>(*_pCard);
	_pMaterialBuilder = std::make_unique<MaterialBuilder>(*_pDevice);
	_pMeshLoader = std::make_unique<MeshLoader>(*_pDevice);
	_pTextureBuilder = std::make_unique<TextureBuilder>(*_pDevice);
}