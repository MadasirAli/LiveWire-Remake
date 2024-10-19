#include "PlayerController.h"

#include "Entity.h"
#include "Globals.h"

using namespace LiveWireRemake;

void PlayerController::OnUpdate(std::weak_ptr<Entity>& pEntity)
{
	const float speed = 10;
	const float rotationSpeed = 10;
	const float turnPoint = 2;
	const float turnPointRange = 0.05;

	auto& globals = Globals::GetInstance();
	auto& input = globals.GetInputManager();
	auto deltaTime = globals.GetDeltaTime();

	auto& transform = pEntity.lock()->GetTransform();
	auto position = transform.position;

	if (std::fmod(position.z, turnPoint) > (turnPoint - turnPointRange))
	{
		if (input.GetKey(InputManager::Key::H))
		{
			transform.rotation.y = 90;

			transform.position.z += turnPoint;
		}
		else if (input.GetKey(InputManager::Key::G))
		{
			transform.rotation.y = -90;

			transform.position.z += turnPointRange;
		}
	}
}
