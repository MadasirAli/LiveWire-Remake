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
		const auto collisionCount = pCollisions.size() - 1;
		if (collisionCount == 0)
			continue;	// no collisions with this entity

		auto pMe = pCollisions[0].lock();

		// overlapping is intentional
		if (pMe->trigger)
			continue;

		// counter displacing overlap
		using namespace DirectX;

		auto pMyTransform = pMe->GetParent().lock()->GetTransform();

		unsigned int overlapAccumulation = 0u;
		unsigned int overlapPartioners = 0u;
		// getting overlap arthimetic accumulation
		for (auto i = 1; i < collisionCount; ++i)
		{
			auto pOther = pCollisions[i].lock();
			auto pOtherTransform = pOther->GetParent().lock()->GetTransform();

			// getting frame displacement
			auto frameDisplacement = DirectX::XMVectorSubtract
				(DirectX::XMVectorSet(pOther->_prePosition.x, pOther->_prePosition.y, pOther->_prePosition.z, 0),
				DirectX::XMVectorSet(pOtherTransform.position.x, pOtherTransform.position.y, pOtherTransform.position.z, 0));

			// if this collider does not contributed actions in overlap.
			if (XMVectorGetX(XMVectorEqual(XMVector3LengthEst(frameDisplacement), XMVectorZero())) > 0u)
			{
				continue;
			}

			// getting overlap
			auto bothDisplacement = DirectX::XMVectorSubtract
				(DirectX::XMVectorSet(pOtherTransform.position.x, pOtherTransform.position.y, pOtherTransform.position.z, 0),
				DirectX::XMVectorSet(pMyTransform.position.x, pMyTransform.position.y, pMyTransform.position.z, 0));
			auto bothDistance = XMVector3Length(bothDisplacement);
			auto bothDir = XMVectorDivide(bothDisplacement, bothDistance);

			auto myEdge = XMVectorAdd(
				XMVectorSet(pMyTransform.position.x, pMyTransform.position.y, pMyTransform.position.z, 0),
				XMVectorMultiply(bothDir, XMVectorSet(pMe->radius, pMe->radius, pMe->radius, pMe->radius)));

			auto otherEdge = XMVectorAdd(
				XMVectorSet(pOtherTransform.position.x, pOtherTransform.position.y, pOtherTransform.position.z, 0),
				XMVectorMultiply(XMVectorNegate(bothDir), XMVectorSet(pMe->radius, pMe->radius, pMe->radius, pMe->radius)));

			// getting edge overlap displacement
			auto overlap = XMVectorSubtract(otherEdge, myEdge);

			overlapAccumulation += XMVectorGetX(XMVector3Length(overlap));
			++overlapPartioners;
		}

		// calculating anti overlap
		// getting frame displacement
		auto frameDisplacement = DirectX::XMVectorSubtract
			(DirectX::XMVectorSet(pMe->_prePosition.x, pMe->_prePosition.y, pMe->_prePosition.z, 0),
			DirectX::XMVectorSet(pMyTransform.position.x, pMyTransform.position.y, pMyTransform.position.z, 0));

		auto frameDistance = XMVector3Length(frameDisplacement);
		auto frameDir = XMVectorDivide(frameDisplacement, frameDistance);

		// getting contributed magnitude
		const auto contribution = overlapAccumulation / (float)overlapPartioners;

		auto antiOverlap = XMVectorMultiply(-frameDir, XMVectorSet(contribution, contribution, contribution, contribution));
		
		// applying anti overlap
		auto newPosition = XMVectorAdd(
			XMVectorSet(pMyTransform.position.x, pMyTransform.position.y, pMyTransform.position.z, 0),
			antiOverlap);

		pMyTransform.position = XMFLOAT3(XMVectorGetX(newPosition), XMVectorGetY(newPosition), XMVectorGetZ(newPosition));
	}
}