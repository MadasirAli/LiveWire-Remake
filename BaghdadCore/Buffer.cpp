#include "Buffer.h"

#include <memory>

#include "ComUtility.h"

using namespace BaghdadCore;

const Microsoft::WRL::ComPtr<ID3D11Buffer>& Buffer::GetComPtr() const noexcept
{
	return _ptr;
}

Buffer::Buffer(Microsoft::WRL::ComPtr<ID3D11Buffer>&& pBuffer) :
	Resource(std::move(ComUtility::As<ID3D11Buffer, ID3D11Resource>(pBuffer)))
{
	_ptr = std::move(pBuffer);
}
