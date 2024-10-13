#pragma once

#include "IComponent.h"

namespace LiveWireRemake
{
	class CollisionEngine final : public IComponent
	{
	public:
		void OnLateUpdate(std::weak_ptr<Entity>& pEntity) override;

		~CollisionEngine() noexcept override = default;
	};
}

