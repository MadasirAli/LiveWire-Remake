#pragma once

#include "IComponent.h"
#include "BaghdadCore/ConstantBuffer.h"

namespace LiveWireRemake
{
	class Light final : public IComponent
	{
	private:
		void OnPreRender(std::weak_ptr<Entity>& pEntity) override;

	public:
		BaghdadCore::ConstantBuffer& GetLightCBuffer() const noexcept;

		Light();
		~Light() noexcept override = default;

	public:
		DirectX::XMFLOAT3 ambientColor;
		float ambientIntensity = 1;

		DirectX::XMFLOAT3 color;
		float intensity = 1;

	private:
		std::unique_ptr<BaghdadCore::ConstantBuffer> _pBuffer;
	};
}

