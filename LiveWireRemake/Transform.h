#pragma once

#include <memory>

#include "BaghdadCore/D3D11.h"

#include "IComponent.h"

namespace LiveWireRemake
{
	class Transform final : public IComponent
	{
	public:
		Transform();
		~Transform() noexcept override = default;

	public:
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR rotation;
		DirectX::XMVECTOR scale;
	};
}

