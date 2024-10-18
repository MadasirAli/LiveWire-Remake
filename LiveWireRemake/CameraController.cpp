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
	auto newPos = XMVectorMultiply(back, XMVectorSet(distance, distance, distance, distance));
	newPos = XMVectorSetX(newPos, sideLook);
	newPos = XMVectorSetY(newPos, height);
	newPos = XMVectorAdd(newPos, XMVectorSet(target.position.x, target.position.y, target.position.z, 0));

	auto newRot = target.Quaternion();

	newPos = XMVectorLerp(XMLoadFloat3(&transform.position), newPos, followSpeed * deltaTime);
	newRot = XMQuaternionSlerp(transform.Quaternion(), newRot, lookSpeed * deltaTime);

	XMStoreFloat3(&transform.position, newPos);

	float x;
	float y;
	float z;
	auto f = XMVectorSet(1, 0, 0, 0);
	auto u = XMVectorSet(0, 1, 0, 0);
	auto s = XMVectorSet(0, 0, 1, 0);
	XMQuaternionToAxisAngle(&f, &x, newRot);
	XMQuaternionToAxisAngle(&u, &y, newRot);
	XMQuaternionToAxisAngle(&s, &z, newRot);

	//transform.rotation = XMFLOAT3(XMConvertToDegrees(x), XMConvertToDegrees(y), XMConvertToDegrees(z));

	ImGui::SliderFloat("ZDistance: ", &distance, 0, 100);
	ImGui::SliderFloat("Move Spped: ", &followSpeed, 0, 100);
}
