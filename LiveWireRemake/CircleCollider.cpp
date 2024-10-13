#include "CircleCollider.h"

using namespace LiveWireRemake;

void CircleCollider::TriggerCollider(const CollisionData& data) const
{
	if (data.collider_trigger == false && data.colludee_trigger == false)
	{
		// TODO: clamp the position


	}

	// firing events
	for (const auto& event : onCollision)
	{
		event(data);
	}
}
