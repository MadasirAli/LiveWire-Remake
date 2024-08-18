#include "DeviceContext.h"

#include <memory>

using namespace BaghdadCore;

DeviceContext::DeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext>&& pOther) :
	_ptr(std::move(pOther))
{}
