#include "DeviceContext.h"

#include <memory>

using namespace BaghdadCore;

const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& DeviceContext::GetComPtr() const noexcept
{
	return _ptr;
}

DeviceContext::DeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext>&& pOther) :
	_ptr(std::move(pOther))
{}
