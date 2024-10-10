#pragma once

#include "IComponent.h"

namespace LiveWireRemake
{
	class Movement final : public IComponent
	{
	private:
		void OnUpdate(std::weak_ptr<Entity>& pEntity) override;

	public:
		~Movement() noexcept override = default;
	};
}

