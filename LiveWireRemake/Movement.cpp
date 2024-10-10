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
		rotation.m128_f32[0] += rotDelta;
	}
	if (input.GetKey(xRotNKey))
	{
		rotation.m128_f32[0] -= rotDelta;
	}

	if (input.GetKey(yRotPKey))
	{
		rotation.m128_f32[1] += rotDelta;
	}
	if (input.GetKey(yRotNKey))
	{
		rotation.m128_f32[1] -= rotDelta;
	}

	if (input.GetKey(zRotPKey))
	{
		rotation.m128_f32[2] += rotDelta;
	}
	if (input.GetKey(zRotNKey))
	{
		rotation.m128_f32[2] -= rotDelta;
	}

	transform.position = position;
	transform.rotation = rotation;
}

void Movement::OnRender(std::weak_ptr<Entity>& pEntity)
{
	auto& transform = pEntity.lock()->GetTransform();

	std::stringstream ss{};
	ss << "Position: " << std::to_string(transform.position.m128_f32[0]);
	ss << "  " << std::to_string(transform.position.m128_f32[1]);
	ss << "  " << std::to_string(transform.position.m128_f32[2]) << std::endl;

	ss << "Rotation: " << std::to_string(transform.rotation.m128_f32[0]);
	ss << "  " << std::to_string(transform.rotation.m128_f32[1]);
	ss << "  " << std::to_string(transform.rotation.m128_f32[2]) << std::endl;

	ss << "Scale: " << std::to_string(transform.scale.m128_f32[0]);
	ss << "  " << std::to_string(transform.scale.m128_f32[1]);
	ss << "  " << std::to_string(transform.scale.m128_f32[2]) << std::endl;

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
		ImGui::SliderFloat("Scale: X", &(transform.scale.m128_f32[0]), 0, 1);
		ImGui::SameLine(200);
		ImGui::SliderFloat("Scale: Y", &(transform.scale.m128_f32[1]), 0, 1);
		ImGui::SameLine(400);
		ImGui::SliderFloat("Scale: Z", &(transform.scale.m128_f32[2]), 0, 1);
		ImGui::PopItemWidth();

		ImGui::End();
	}
}
