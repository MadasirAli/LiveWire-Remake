#include "Material.h"

#include "GraphicsError.h"

using namespace BaghdadCore;

void Material::Bind(const Device& device, const DeviceContext& context) const noexcept(!_DEBUG)
{
	const auto& pContext = context.GetComPtr();

	// binding input layout
	D3D_CHECK_CALL(
		pContext->IASetInputLayout(_pInputLayout.Get())
	);

	// binding vertex shader
	D3D_CHECK_CALL(
		pContext->VSSetShader(_vertexShader.GetComPtr().Get(), nullptr, 0u);
	);
	// binding pixel shader
	D3D_CHECK_CALL(
		pContext->PSSetShader(_pixelShader.GetComPtr().Get(), nullptr, 0u);
	);
}

Material::Material(VertexShader&& vertexShader, PixelShader&& pixelShader,
	ShaderModule&& vertexModule,
	ShaderReflectionDB&& vertexReflectionDB, ShaderReflectionDB&& pixelReflectionDB,
	Microsoft::WRL::ComPtr<ID3D11InputLayout>&& pInputLayout) :
	_vertexShader(std::move(vertexShader)),
	_pixelShader(std::move(pixelShader)),
	_vertexModule(std::move(vertexModule)),
	_vertexReflectionDB(std::move(vertexReflectionDB)),
	_pixelReflectionDB(std::move(pixelReflectionDB)),
	_pInputLayout(std::move(pInputLayout))
{}