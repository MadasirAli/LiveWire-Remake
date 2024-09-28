#pragma once

#include <memory>

#include "DXGIFactory.h"
#include "GraphicsCard.h"
#include "Device.h"
#include "Swapchain.h"
#include "MaterialBuilder.h"
#include "MeshLoader.h"
#include "Mesh.h"
#include "Material.h"

namespace BaghdadCore
{
	class Renderer final
	{
	public:
		void DrawMesh(const Mesh& mesh, const Material& material) const noexcept(_DEBUG);

		/// <summary>
		/// Creates device on primary display adapter (graphics card).
		/// </summary>
		/// <exception cref="BaghdadError">If no Graphics capable adapter found.</exception>
		Renderer();

		Renderer(const Renderer& renderer) = delete;

		Renderer& operator= (const Renderer& renderer) = delete;

	private:
		DXGIFactory _factory;
		std::unique_ptr<GraphicsCard> _pCard;
		std::unique_ptr<Device> _pDevice;
		std::unique_ptr<Swapchain> _pSwapchain;

		std::unique_ptr<MaterialBuilder> _pMaterialBuilder;
		std::unique_ptr<MeshLoader> _pMeshLoader;
	};
}