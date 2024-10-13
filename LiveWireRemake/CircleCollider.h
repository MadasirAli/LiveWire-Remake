#pragma once

#include <functional>
#include <vector>

#include "IComponent.h"

namespace LiveWireRemake
{
	class CircleCollider final : public IComponent
	{
	public:
		class CollisionData final
		{
		public:
			std::weak_ptr<Entity> colludee;
			std::weak_ptr<Entity> collider;

			bool colludee_trigger;
			bool collider_trigger;
		};

	public:
		void TriggerCollider(const CollisionData& data) const;

		CircleCollider() = default;
		~CircleCollider() noexcept override = default;

	public:
		std::vector<std::function<void(const CollisionData&)>> onCollision;

		float radius = 1;
		bool trigger = false;
	};
}

