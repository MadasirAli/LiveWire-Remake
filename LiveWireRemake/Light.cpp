#include "Light.h"

#include "Globals.h"
#include "LightCBuffer.h"

using namespace LiveWireRemake;

void Light::OnPreRender(std::weak_ptr<Entity>& pEntity)
{
	using namespace DirectX;

	auto& transform = pEntity.lock()->GetTransform();

	// updating c buffer
	LightCBuffer data = {};
	data.ambient = XMFLOAT4(ambientColor.x, ambientColor.y, ambientColor.z, ambientIntensity);
	data.color = XMFLOAT4(color.x, color.y, color.z, intensity);
	data.position = XMFLOAT4(transform.position.x, transform.position.y, transform.position.z, 1);
	data.rotation = XMFLOAT4(transform.rotation.x, transform.rotation.y, transform.rotation.z, 1);

	const auto ptr = (LightCBuffer*)_pBuffer->Map(D3D11_MAP_WRITE_DISCARD);
	ptr[0] = data;

	_pBuffer->UnMap();
}

BaghdadCore::ConstantBuffer& Light::GetLightCBuffer() const noexcept
{
	return *_pBuffer;
}

Light::Light() :
	ambientColor(0.4f, 0.4f, 0.4f),
	color(1, 1, 1)
{
	// creating c buffer
	const LightCBuffer pData = {};
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
