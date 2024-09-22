#include "VertexShader.h"

#include <memory>

#include "ComUtility.h"

using namespace BaghdadCore;

const Microsoft::WRL::ComPtr<ID3D11VertexShader>& VertexShader::GetComPtr() const noexcept
{
	return _ptr;
}

VertexShader::VertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader>&& pShader) :
	Shader(std::move(ComUtility::As<ID3D11VertexShader, ID3D11DeviceChild>(pShader)))
{}
