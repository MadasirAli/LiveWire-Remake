#pragma once

#include <memory>

#include "IComponent.h"
#include "BaghdadCore/Material.h"
#include "BaghdadCore/Mesh.h"
#include "BaghdadCore/ConstantBuffer.h"

namespace LiveWireRemake
{
	class MeshRenderer final : public IComponent
	{
	public:
		BaghdadCore::ConstantBuffer& GetTransformCBuffer() noexcept;
		void SetMaterial(BaghdadCore::Material&& material) noexcept;
		void SetMesh(BaghdadCore::Mesh&& mesh) noexcept;

		BaghdadCore::Material& GetMaterial() noexcept;
		BaghdadCore::Mesh& GetMesh() noexcept;

		MeshRenderer();
		~MeshRenderer() noexcept override = default;

	private:
		void OnPreRender(std::weak_ptr<Entity>& pEntity) override;

	public:
		bool useLights = true;
		bool goroudShading = true;

	private:
		std::unique_ptr<BaghdadCore::Material> _pMaterial = nullptr;
		std::unique_ptr<BaghdadCore::Mesh> _pMesh = nullptr;
		std::unique_ptr<BaghdadCore::ConstantBuffer> _pBuffer;
	};
}

