#include "CameraController.h"

#include "Entity.h"
#include "Globals.h"

#include "BaghdadCore\imgui.h"

using namespace LiveWireRemake;

void CameraController::OnUpdate(std::weak_ptr<Entity>& pEntity)
{
	if (pFollow.expired())
		return;

	auto& globals = Globals::GetInstance();
	float deltaTime = globals.GetDeltaTime();

	auto& target = *pFollow.lock();
	auto targetForwardFloat = target.Forward();
	auto targetForward = XMLoadFloat3(&targetForwardFloat);

	auto& transform = pEntity.lock()->GetTransform();

	using namespace DirectX;
	
	auto back = XMVectorNegate(targetForward);
	auto newPos = XMVectorSet(target.position.x, target.position.y, target.position.z, 0);
	newPos = XMVectorAdd(newPos, XMVectorMultiply(back, XMVectorSet(distance, distance, distance, distance)));
	newPos = XMVectorAdd(newPos, XMVectorSet(sideLook, height, 0, 0));

	newPos = XMVectorLerp(XMLoadFloat3(&transform.position), newPos, followSpeed * deltaTime);

	XMStoreFloat3(&transform.position, newPos);
	
	{
		auto dir = XMVector4Normalize(XMVectorSubtract(
			XMVectorSet(target.position.x, target.position.y, target.position.z, 0),
			XMVectorSet(transform.position.x, transform.position.y, transform.position.z, 0)));

		float angleY = (std::atan2(XMVectorGetZ(dir), XMVectorGetX(dir)) * 180.0f) / (22.0f / 7.0f);
		float angleX = (std::atan2(XMVectorGetY(dir), XMVectorGetZ(dir)) * 180.0f) / (22.0f / 7.0f);

		if (angleY < 0)
			angleY += 360;

		ImGui::Text(std::to_string(angleY).c_str());

		//transform.rotation.y = std::lerp(transform.rotation.y, - (angleY - 90), lookSpeed * deltaTime);
		//transform.rotation.x = std::lerp(transform.rotation.x, - (angleX), lookSpeed * deltaTime);
		transform.rotation.y = -(angleY - 90);
		//transform.rotation.x = -angleX;
	}

	ImGui::SliderFloat("ZDistance: ", &distance, 0, 100);
	ImGui::SliderFloat("Move Spped: ", &followSpeed, 0, 100);
	ImGui::SliderFloat("Side: ", &sideLook, -30, 30);
}
