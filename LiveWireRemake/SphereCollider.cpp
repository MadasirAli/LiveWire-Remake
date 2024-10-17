#include "SphereCollider.h"

#include "Entity.h"

using namespace LiveWireRemake;

void SphereCollider::OnPreUpdate(std::weak_ptr<Entity>& pEntity)
{
	_prePosition = pEntity.lock()->GetTransform().position;
}

void SphereCollider::TriggerCollision(const CollisionData& data) const
{
	// firing events
	for (const auto& event : onCollision)
	{
		event(data);
	}
}

SphereCollider::SphereCollider() :
	_prePosition()
{}
