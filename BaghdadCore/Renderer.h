#pragma once

#include <memory>

#include "DXGIFactory.h"
#include "GraphicsCard.h"
#include "Device.h"
#include "Swapchain.h"
#include "MaterialBuilder.h"
#include "MeshLoader.h"
#include "TextureBuilder.h"
#include "Mesh.h"
#include "Material.h"
#include "BufferBuilder.h"

namespace BaghdadCore
{
	class Renderer final
	{
	public:
		void DrawMesh(const Mesh& mesh, const Material& material) const noexcept(_DEBUG);
		void Blit(const Texture2D& source, const Texture2D& destination) const noexcept(_DEBUG);

		void SetRenderTexture(std::unique_ptr<Texture2D>&& pRenderTexture) noexcept;
		Texture2D& GetRenderTexture() const noexcept;

		TextureBuilder& GetTextureBuilder() const noexcept;
		MaterialBuilder& GetMaterialBuilder() const noexcept;
		MeshLoader& GetMeshLoader() const noexcept;
		BufferBuilder& GetBufferBuilder() const noexcept;

		/// <summary>
		/// Creates device on primary display adapter (graphics card).
		/// </summary>
		/// <exception cref="BaghdadError">If no Graphics capable adapter found.</exception>
		Renderer();

	private:
		DXGIFactory _factory;
		std::unique_ptr<GraphicsCard> _pCard;
		std::unique_ptr<Device> _pDevice;
		std::unique_ptr<Swapchain> _pSwapchain;

		std::unique_ptr<MaterialBuilder> _pMaterialBuilder;
		std::unique_ptr<MeshLoader> _pMeshLoader;
		std::unique_ptr<TextureBuilder> _pTextureBuilder;
		std::unique_ptr<BufferBuilder> _pBufferBuilder;

		std::unique_ptr<Texture2D> _pRenderTexture;
	};
}