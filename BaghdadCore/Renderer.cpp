#include "Renderer.h"

#include "BaghdadError.h"
#include "GraphicsError.h"

using namespace BaghdadCore;

void Renderer::DrawMesh(const Mesh& mesh, const Material& material) const noexcept(!_DEBUG)
{
	const auto& context = _pDevice->GetDeviceContext();

	/// binding mesh	
	mesh.Bind(*_pDevice, context);
	// binding material
	material.Bind(*_pDevice, context);

	// issuing draw call
	D3D_CHECK_CALL(
		context.GetComPtr()->Draw(mesh._vertexCount, 0u);
	);
}

void Renderer::Blit(const Texture2D& source, const Texture2D& destination) const noexcept(!_DEBUG)
{
	D3D_CHECK_CALL(
	_pDevice->GetDeviceContext()
		.GetComPtr()->CopyResource(
			source.GetComResourcePtr().Get(), destination.GetComResourcePtr().Get());
	);
}

void Renderer::SetRenderTexture(std::unique_ptr<Texture2D>&& pRenderTexture) noexcept
{
	_pRenderTexture = std::move(pRenderTexture);
}

Texture2D& Renderer::GetRenderTexture() const noexcept
{
	return *_pRenderTexture;
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

BufferBuilder& Renderer::GetBufferBuilder() const noexcept
{
	return *_pBufferBuilder;
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
	_pBufferBuilder = std::make_unique<BufferBuilder>(*_pDevice);
}