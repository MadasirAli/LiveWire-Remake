#include "Transform.h"

#include "Globals.h"

using namespace LiveWireRemake;
using namespace BaghdadCore;

DirectX::XMFLOAT3 Transform::Forward() const noexcept
{
	auto dir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), Quaternion());

	return DirectX::XMFLOAT3(DirectX::XMVectorGetX(dir), DirectX::XMVectorGetY(dir), DirectX::XMVectorGetZ(dir));
}

DirectX::XMFLOAT3 Transform::Right() const noexcept
{
	auto dir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(1, 0, 0, 0), Quaternion());

	return DirectX::XMFLOAT3(DirectX::XMVectorGetX(dir), DirectX::XMVectorGetY(dir), DirectX::XMVectorGetZ(dir));
}

DirectX::XMFLOAT3 Transform::Up() const noexcept
{
	auto dir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 1, 0, 0), Quaternion());

	return DirectX::XMFLOAT3(DirectX::XMVectorGetX(dir), DirectX::XMVectorGetY(dir), DirectX::XMVectorGetZ(dir));
}

DirectX::XMVECTOR Transform::Quaternion() const noexcept
{
	using namespace DirectX;

	// updating transform c buffer
	auto quaternion = XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(rotation.x),
		XMConvertToRadians(rotation.y),
		XMConvertToRadians(rotation.z));

	return quaternion;
}

Transform::Transform() :
	position(0,0,0),
	rotation(0, 0, 0),
	scale(1, 1, 1)
{}
