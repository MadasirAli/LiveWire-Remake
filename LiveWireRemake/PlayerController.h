#pragma once

#include "IComponent.h"

namespace LiveWireRemake
{
	class PlayerController final : public IComponent
	{


	public:
		void OnUpdate(std::weak_ptr<Entity>& pEntity) override;

		~PlayerController() noexcept override = default;
	};
}

