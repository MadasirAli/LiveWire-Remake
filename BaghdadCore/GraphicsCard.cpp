#include "GraphicsCard.h"

#include <memory>

using namespace BaghdadCore;

Microsoft::WRL::ComPtr<IDXGIAdapter> GraphicsCard::GetNativePointer() const noexcept
{
	return _ptr;
}

GraphicsCard::GraphicsCard(Microsoft::WRL::ComPtr<IDXGIAdapter>&& pOther) :
	_ptr(std::move(pOther))
{}