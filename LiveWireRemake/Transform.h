#pragma once

#include <memory>

#include "BaghdadCore/D3D11.h"
#include "BaghdadCore/ConstantBuffer.h"

#include "IComponent.h"
#include "TransformCBuffer.h"

namespace LiveWireRemake
{
	class Transform final : public IComponent
	{
	public:
		BaghdadCore::ConstantBuffer& GetTransformCBuffer() noexcept;

		Transform();
		~Transform() noexcept override = default;

	private:
		void OnPreRender(std::weak_ptr<Entity>& pEntity) override;

	public:
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR rotation;
		DirectX::XMVECTOR scale;

	private:
		std::unique_ptr<BaghdadCore::ConstantBuffer> _pBuffer;
	};
}

