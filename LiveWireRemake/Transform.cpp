#include "Transform.h"

#include "BaghdadCore/BufferBuilder.h"

#include "Globals.h"

using namespace LiveWireRemake;
using namespace BaghdadCore;

void Transform::OnPreRender(std::weak_ptr<Entity>& pEntity)
{
	using namespace DirectX;

	// updating transform c buffer

	TransformCBuffer data = {};
	data.WorldMatrix = XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(), scale, XMVectorZero(), rotation, position);

	const auto ptr = (TransformCBuffer*)_pBuffer->Map(D3D11_MAP_WRITE_DISCARD);
	ptr[0] = data;

	_pBuffer->UnMap();
}

ConstantBuffer& Transform::GetTransformCBuffer() noexcept
{
	return *_pBuffer;
}

Transform::Transform() :
	position(),
	rotation(),
	scale()
{
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
	
	scale = DirectX::XMVectorSet(1, 1, 1, 1);
}
