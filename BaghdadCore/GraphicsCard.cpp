#include "GraphicsCard.h"

#include <memory>

using namespace BaghdadCore;

const Microsoft::WRL::ComPtr<IDXGIAdapter>& GraphicsCard::GetComPtr() const noexcept
{
	return _ptr;
}

GraphicsCard::GraphicsCard(Microsoft::WRL::ComPtr<IDXGIAdapter>&& pOther) :
	_ptr(std::move(pOther))
{}