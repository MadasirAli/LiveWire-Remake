#pragma once

#include <functional>
#include <vector>

#include "IComponent.h"
#include "BaghdadCore\D3D11.h"

namespace LiveWireRemake
{
	class CircleCollider final : public IComponent
	{
		friend class CollisionEngine;

	public:
		class CollisionData final
		{
		public:
			std::weak_ptr<CircleCollider> colludee;
			std::weak_ptr<CircleCollider> collider;
		};

	public:
		CircleCollider() = default;
		~CircleCollider() noexcept override = default;

	private:
		void OnPreUpdate(std::weak_ptr<Entity>& pEntity) override;

		void TriggerCollision(const CollisionData& data) const;

	public:
		std::vector<std::function<void(const CollisionData&)>> onCollision;

		float radius = 1;
		bool trigger = false;

	private:
		DirectX::XMFLOAT3 _prePosition;
	};
}

