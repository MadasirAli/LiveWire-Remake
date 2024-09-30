#include "Texture2D.h"

#include "ComUtility.h"

using namespace BaghdadCore;

const Microsoft::WRL::ComPtr<ID3D11Texture2D>& Texture2D::GetComPtr() const noexcept
{
	return _ptr;
}

Texture2D::Texture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D>&& pTexture, View&& view)
	: Texture(std::move(ComUtility::As<ID3D11Texture2D, ID3D11Resource>(pTexture)), std::move(view))
{
	_ptr = std::move(pTexture);
}