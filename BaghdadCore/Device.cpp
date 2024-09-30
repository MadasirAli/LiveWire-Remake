#include "Device.h"

#include "D3D11.h"
#include "GraphicsError.h"

using namespace BaghdadCore;

const DeviceContext& Device::GetDeviceContext() const noexcept
{
	return *_pContext;
}

const Microsoft::WRL::ComPtr<ID3D11Device>& Device::GetComPtr() const noexcept
{
	return _ptr;
}

Device::Device(const GraphicsCard& card)
{
	auto flags = 0;

#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;

	D3D_CALL(
	D3D11CreateDevice(
		card.GetComPtr().Get(),
		D3D_DRIVER_TYPE_UNKNOWN,	// unknown for creating from existing adapter
		nullptr,
		flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		_ptr.ReleaseAndGetAddressOf(),
		&_featureLevel,
		pContext.ReleaseAndGetAddressOf()
	));

	_pContext = std::make_unique<DeviceContext>(std::move(pContext));
}