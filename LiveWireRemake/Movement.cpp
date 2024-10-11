#include "Movement.h"

#include <sstream>

#include "Entity.h"
#include "Globals.h"
#include "BaghdadCore/imgui.h"

using namespace LiveWireRemake;

void Movement::OnUpdate(std::weak_ptr<Entity>& pEntity)
{
	using namespace DirectX;

	auto& globals = Globals::GetInstance();
	auto& input = globals.GetInputManager();

	const auto deltaTime = globals.GetDeltaTime();

	auto& transform = pEntity.lock()->GetTransform();

	const auto delta = _speed * deltaTime;
	const auto rotDelta = _rotateSpeed * deltaTime;

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

	XMFLOAT3 position = transform.position;
	XMFLOAT3 rotation = transform.rotation;

	if (input.GetKey(rightKey))
	{
		auto dir = transform.Right();
		position = XMFLOAT3(position.x + dir.x * delta, position.y + dir.y * delta, position.z + dir.z * delta);
	}
	if (input.GetKey(leftKey))
	{
		auto dir = transform.Right();
		position = XMFLOAT3(position.x - dir.x * delta, position.y - dir.y * delta, position.z - dir.z * delta);
	}
	if (input.GetKey(upKey))
	{
		auto dir = transform.Up();
		position = XMFLOAT3(position.x + dir.x * delta, position.y + dir.y * delta, position.z + dir.z * delta);
	}
	if (input.GetKey(downKey))
	{
		auto dir = transform.Up();
		position = XMFLOAT3(position.x - dir.x * delta, position.y - dir.y * delta, position.z - dir.z * delta);
	}
	if (input.GetKey(forwardKey))
	{
		auto dir = transform.Forward();
		position = XMFLOAT3(position.x + dir.x * delta, position.y + dir.y * delta, position.z + dir.z * delta);
	}
	if (input.GetKey(backwardKey))
	{
		auto dir = transform.Forward();
		position = XMFLOAT3(position.x - dir.x * delta, position.y - dir.y * delta, position.z - dir.z * delta);
	}

	if (input.GetKey(xRotPKey))
	{
		rotation.x += rotDelta;
	}
	if (input.GetKey(xRotNKey))
	{
		rotation.x -= rotDelta;
	}

	if (input.GetKey(yRotPKey))
	{
		rotation.y += rotDelta;
	}
	if (input.GetKey(yRotNKey))
	{
		rotation.y -= rotDelta;
	}

	if (input.GetKey(zRotPKey))
	{
		rotation.z += rotDelta;
	}
	if (input.GetKey(zRotNKey))
	{
		rotation.z -= rotDelta;
	}

	transform.position = position;
	transform.rotation = rotation;
}

void Movement::OnRender(std::weak_ptr<Entity>& pEntity)
{
	auto& transform = pEntity.lock()->GetTransform();

	std::stringstream ss{};
	ss << "Position: " << std::to_string(transform.position.x);
	ss << "  " << std::to_string(transform.position.y);
	ss << "  " << std::to_string(transform.position.z) << std::endl;

	ss << "Rotation: " << std::to_string(transform.rotation.x);
	ss << "  " << std::to_string(transform.rotation.y);
	ss << "  " << std::to_string(transform.rotation.z) << std::endl;

	ss << "Scale: " << std::to_string(transform.scale.x);
	ss << "  " << std::to_string(transform.scale.y);
	ss << "  " << std::to_string(transform.scale.z) << std::endl;

	// imgui 
	{
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_Alpha, 0.3f);
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowRounding, 15);
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_TabBorderSize, 0);

		ImGui::Begin("Debug:", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowPos(ImVec2(15, 15));
		ImGui::SetWindowSize(ImVec2(630, 130));

		ImGui::PopStyleVar(3);

		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text(ss.str().c_str());
		ImGui::PopStyleColor();

		ImGui::SliderFloat("Speed", &_speed, 1, 1000);
		ImGui::SliderFloat("Rotation Speed", &_rotateSpeed, 1, 1000);

		ImGui::PushItemWidth(50);
		ImGui::SliderFloat("Scale: X", &(transform.scale.x), 0, 1);
		ImGui::SameLine(200);
		ImGui::SliderFloat("Scale: Y", &(transform.scale.y), 0, 1);
		ImGui::SameLine(400);
		ImGui::SliderFloat("Scale: Z", &(transform.scale.z), 0, 1);
		ImGui::PopItemWidth();

		ImGui::End();
	}
}
