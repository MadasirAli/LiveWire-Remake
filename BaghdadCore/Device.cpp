#include "Device.h"

#include "D3D11.h"
#include "BaghdadError.h"

using namespace BaghdadCore;

Microsoft::WRL::ComPtr<ID3D11Device> Device::GetNativePointer() const noexcept
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

	WIN32_CALL(
	D3D11CreateDevice(
		card.GetNativePointer().Get(),
		D3D_DRIVER_TYPE_HARDWARE,
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