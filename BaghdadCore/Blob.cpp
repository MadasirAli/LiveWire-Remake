#include "Blob.h"

#include <memory>

using namespace BaghdadCore;

const Microsoft::WRL::ComPtr<ID3DBlob>& Blob::GetComPtr() const noexcept
{
	return _ptr;
}

BaghdadCore::Blob::Blob(Microsoft::WRL::ComPtr<ID3DBlob>&& pBlob)
{
	_ptr = std::move(pBlob);
}
