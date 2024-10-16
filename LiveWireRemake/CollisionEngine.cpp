#include "CollisionEngine.h"

#include <vector>

#include "Globals.h"
#include "CircleCollider.h"

using namespace LiveWireRemake;

void CollisionEngine::OnLateUpdate(std::weak_ptr<Entity>& pEntity)
{
	auto& globals = Globals::GetInstance();
	auto& world = globals.GetWorldManager().GetActiveWorld();

	// circle collision algorith
	auto colliders = world.GetComponentsOfType<CircleCollider>();
	std::vector<std::vector<std::weak_ptr<CircleCollider>>> collisionsList{};

	// getting all collisions and triggering event
	for (auto& pCollider : colliders)
	{
		std::vector<std::weak_ptr<CircleCollider>> pCollisions{};

		pCollisions.push_back(pCollider);

		auto pMe = pCollider.lock();

		for (auto& pOtherCollider : colliders)
		{
			auto pOther = pOtherCollider.lock();

			// self collision
			if (pMe == pOther)
				continue;

			auto pMyTransform = pMe->GetParent().lock()->GetTransform();
			auto pOtherTransform = pOther->GetParent().lock()->GetTransform();

			// getting data
			auto test_distance = pMe->radius + pOther->radius;
			auto distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract
							(DirectX::XMVectorSet(pOtherTransform.position.x, pOtherTransform.position.y, pOtherTransform.position.z, 0), 
							DirectX::XMVectorSet(pMyTransform.position.x, pMyTransform.position.y, pMyTransform.position.z, 0))));

			// checking sphere overlap
			if (distance >= test_distance)
				continue;	// no collision

			pCollisions.push_back(pOtherCollider);

			// triggering events
			CircleCollider::CollisionData myData{};
			myData.collider = pCollider;
			myData.colludee = pOtherCollider;

			pMe->TriggerCollision(myData);
		}

		collisionsList.emplace_back(std::move(pCollisions));
	}

	// handling overlapp of collisions
	for (auto& pCollisions : collisionsList)
	{
		for (auto& pCollision : pCollisions)
		{
			const auto partioners = pCollisions.size() - 1;
			if (partioners == 0)
				continue;	// no collisions with this entity

			auto pMe = pCollision.lock();

			// overlapping is intentional
			if (pMe->trigger)
				continue;

			// counter displacing overlap
			using namespace DirectX;

			
		}
	}
}