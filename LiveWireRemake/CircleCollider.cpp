#include "CircleCollider.h"

#include "Entity.h"

using namespace LiveWireRemake;

void CircleCollider::OnPreUpdate(std::weak_ptr<Entity>& pEntity)
{
	_prePosition = pEntity.lock()->GetTransform().position;
}

void CircleCollider::TriggerCollision(const CollisionData& data) const
{
	// firing events
	for (const auto& event : onCollision)
	{
		event(data);
	}
}
