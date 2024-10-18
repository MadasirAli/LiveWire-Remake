#include "PlayerController.h"

#include "Entity.h"
#include "Globals.h"

using namespace LiveWireRemake;

void PlayerController::OnUpdate(std::weak_ptr<Entity>& pEntity)
{
	const float speed = 10;
	const float rotationSpeed = 10;

	auto& globals = Globals::GetInstance();
	auto& input = globals.GetInputManager();
	auto deltaTime = globals.GetDeltaTime();


}
