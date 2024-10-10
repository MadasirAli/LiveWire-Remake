#include "Movement.h"

#include "Entity.h"
#include "Globals.h"

using namespace LiveWireRemake;

void Movement::OnUpdate(std::weak_ptr<Entity>& pEntity)
{
	using namespace DirectX;

	auto& globals = Globals::GetInstance();
	auto& input = globals.GetInputManager();

	const auto deltaTime = globals.GetDeltaTime();

	auto& transform = pEntity.lock()->GetTransform();

	const auto delta = 10.0f * deltaTime;

	const auto rightKey = InputManager::Key::RightArrow;
	const auto leftKey = InputManager::Key::LeftArrow;
	const auto upKey = InputManager::Key::UpArrow;
	const auto downKey = InputManager::Key::DownArrow;
	const auto forwardKey = InputManager::Key::Space;
	const auto backwardKey = InputManager::Key::N;

	const auto xRotPKey = InputManager::Key::W;
	const auto yRotPKey = InputManager::Key::S;
	const auto zRotPKey = InputManager::Key::X;

	const auto xRotNKey = InputManager::Key::Q;
	const auto yRotNKey = InputManager::Key::A;
	const auto zRotNKey = InputManager::Key::Z;

	XMVECTOR position = transform.position;
	XMVECTOR rotation = transform.rotation;

	if (input.GetKey(rightKey))
	{
		position.m128_f32[0] += delta;
	}
	if (input.GetKey(leftKey))
	{
		position.m128_f32[0] -= delta;
	}
	if (input.GetKey(upKey))
	{
		position.m128_f32[1] += delta;
	}
	if (input.GetKey(downKey))
	{
		position.m128_f32[1] -= delta;
	}
	if (input.GetKey(forwardKey))
	{
		position.m128_f32[2] += delta;
	}
	if (input.GetKey(backwardKey))
	{
		position.m128_f32[2] -= delta;
	}

	if (input.GetKey(xRotPKey))
	{
		rotation.m128_f32[0] += delta;
	}
	if (input.GetKey(xRotNKey))
	{
		rotation.m128_f32[0] -= delta;
	}

	if (input.GetKey(yRotPKey))
	{
		rotation.m128_f32[1] += delta;
	}
	if (input.GetKey(yRotNKey))
	{
		rotation.m128_f32[1] -= delta;
	}

	if (input.GetKey(zRotPKey))
	{
		rotation.m128_f32[2] += delta;
	}
	if (input.GetKey(zRotNKey))
	{
		rotation.m128_f32[2] -= delta;
	}

	transform.position = position;
	transform.rotation = rotation;
}