#include "Resource.h"

#include <memory>

#include "ComUtility.h"

using namespace BaghdadCore;

const Microsoft::WRL::ComPtr<ID3D11Resource>& Resource::GetComResourcePtr() const noexcept
{
	return _ptr;
}

Resource::Resource(Microsoft::WRL::ComPtr<ID3D11Resource>&& pResource)
	: DeviceChild(std::move(ComUtility::As<ID3D11Resource, ID3D11DeviceChild>(pResource)))
{
	_ptr = std::move(pResource);
}
