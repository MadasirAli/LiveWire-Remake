#include "SphereCollider.h"

#include "Entity.h"
#include "Globals.h"
#include "TransformCBuffer.h"

using namespace LiveWireRemake;

void SphereCollider::OnPreUpdate(std::weak_ptr<Entity>& pEntity)
{
	_prePosition = pEntity.lock()->GetTransform().position;
}

void SphereCollider::OnPreRender(std::weak_ptr<Entity>& pEntity)
{
	using namespace DirectX;

	auto& transform = pEntity.lock()->GetTransform();

	// updating transform c buffer
	TransformCBuffer data = {};
	data.WorldMatrix = XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(),
		XMVectorSet(radius, radius, radius, 1), XMVectorZero(), XMQuaternionIdentity(),
		XMVectorSet(transform.position.x, transform.position.y, transform.position.z, 1));
	data.WorldMatrix = XMMatrixTranspose(data.WorldMatrix);

	const auto ptr = (TransformCBuffer*)_pBuffer->Map(D3D11_MAP_WRITE_DISCARD);
	ptr[0] = data;

	_pBuffer->UnMap();
}

void SphereCollider::TriggerCollision(const CollisionData& data) const
{
	// firing events
	for (const auto& event : onCollision)
	{
		event(data);
	}
}

BaghdadCore::ConstantBuffer& SphereCollider::GetTransformCBuffer() noexcept
{
	return *_pBuffer;
}

SphereCollider::SphereCollider() :
	_prePosition()
{
	// creating transform cBuffer
	const TransformCBuffer pData = {};
	_pBuffer = std::make_unique<BaghdadCore::ConstantBuffer>(std::move(
		Globals::GetInstance()
		.GetRenderer()
		.GetBufferBuilder()
		.Clear()
		.Write()
		.InitialData((char*)&pData, sizeof(pData))
		.BuildCBuffer()
	));
}
