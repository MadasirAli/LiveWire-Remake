#pragma once

#include "IComponent.h"
#include "BaghdadCore\Texture2D.h"
#include "BaghdadCore\Material.h"
#include "BaghdadCore\Mesh.h"

namespace LiveWireRemake
{
	class SkyboxRenderer final : public IComponent
	{
	public:
		BaghdadCore::Texture2D& GetCubeMap() const noexcept;
		void SetCubeMap(BaghdadCore::Texture2D&& cubemap) noexcept;

		BaghdadCore::Material& GetMaterial() const noexcept;
		void SetMaterial(BaghdadCore::Material&& material) noexcept;

		BaghdadCore::Mesh& GetMesh() const noexcept;

		SkyboxRenderer();
		~SkyboxRenderer() noexcept override = default;

	private:
		std::unique_ptr<BaghdadCore::Texture2D> _pCubemap;
		std::unique_ptr<BaghdadCore::Mesh> _pMesh;
		std::unique_ptr<BaghdadCore::Material> _pMaterial;
	};
}

