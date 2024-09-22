#include "PixelShader.h"

#include <memory>

#include "ComUtility.h"

using namespace BaghdadCore;

const Microsoft::WRL::ComPtr<ID3D11PixelShader>& PixelShader::GetComPtr() const noexcept
{
	return _ptr;
}

PixelShader::PixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader>&& pShader) :
	Shader(std::move(ComUtility::As<ID3D11PixelShader, ID3D11DeviceChild>(pShader)))
{
	_ptr = std::move(pShader);
}
