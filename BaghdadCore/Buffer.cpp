#include "Buffer.h"

#include <memory>

#include "ComUtility.h"

using namespace BaghdadCore;

const Microsoft::WRL::ComPtr<ID3D11Buffer>& Buffer::GetComPtr() const noexcept
{
	return _ptr;
}

Buffer::Buffer(Microsoft::WRL::ComPtr<ID3D11Buffer>&& pBuffer, View&& view) :
	Resource(std::move(ComUtility::As<ID3D11Buffer, ID3D11Resource>(pBuffer)), std::move(view))
{
	_ptr = std::move(pBuffer);
}
