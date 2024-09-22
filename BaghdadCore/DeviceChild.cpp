#include "DeviceChild.h"

#include <memory>

using namespace BaghdadCore;

void DeviceChild::GetDevice(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice) const noexcept
{
	_ptr->GetDevice(pDevice.ReleaseAndGetAddressOf());
}

DeviceChild::DeviceChild(Microsoft::WRL::ComPtr<ID3D11DeviceChild>&& ptr)
{
	_ptr = std::move(ptr);
}