#include "Texture2D.h"

#include "ComUtility.h"

using namespace BaghdadCore;

Texture2D::Texture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D>&& pTexture)
	: Texture(std::move(ComUtility::As<ID3D11Texture2D, ID3D11Resource>(pTexture)))
{
	_ptr = std::move(pTexture);
}