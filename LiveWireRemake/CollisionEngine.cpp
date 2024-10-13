#include "CollisionEngine.h"

#include "Globals.h"
#include "CircleCollider.h"

using namespace LiveWireRemake;

void CollisionEngine::OnLateUpdate(std::weak_ptr<Entity>& pEntity)
{
	auto& globals = Globals::GetInstance();
	auto& world = globals.GetWorldManager().GetActiveWorld();

	// circle collision algorith
	world.for
}
