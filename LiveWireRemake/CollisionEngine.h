#pragma once

#include "IComponent.h"

namespace LiveWireRemake
{
	class CollisionEngine final : public IComponent
	{
	public:
		void OnPreRender(std::weak_ptr<Entity>& pEntity) override;

		~CollisionEngine() noexcept override = default;
	};
}

