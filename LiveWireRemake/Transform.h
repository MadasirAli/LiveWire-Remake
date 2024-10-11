#pragma once

#include <memory>

#include "BaghdadCore/D3D11.h"

#include "IComponent.h"

namespace LiveWireRemake
{
	class Transform final : public IComponent
	{
	public:
		DirectX::XMFLOAT3 Forward() const noexcept;
		DirectX::XMFLOAT3 Right() const noexcept;
		DirectX::XMFLOAT3 Up() const noexcept;

		DirectX::XMVECTOR Quaternion() const noexcept;

		Transform();
		~Transform() noexcept override = default;

	public:
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 rotation;
		DirectX::XMFLOAT3 scale;
	};
}

