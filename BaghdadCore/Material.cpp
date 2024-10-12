#include "Material.h"

#include <utility>

using namespace BaghdadCore;

void Material::Bind(const Device& device, const DeviceContext& context) const NOEXCEPT
{
	const auto& pContext = context.GetComPtr();

	// binding resources
	for (const auto& texture : _vertexBindedTextures)
	{
		D3D_CHECK_CALL(
			pContext->VSSetShaderResources(texture.first, 1u, texture.second.GetView().GetSRVComPtr().GetAddressOf())
		);
		D3D_CHECK_CALL(
			pContext->VSSetSamplers(texture.first, 1u, texture.second.GetView().GetSamplerComPtr().GetAddressOf())
		);
	}
	for (const auto& texture : _pixelBindedTextures)
	{
		D3D_CHECK_CALL(
			pContext->PSSetShaderResources(texture.first, 1u, texture.second.GetView().GetSRVComPtr().GetAddressOf())
		);
		D3D_CHECK_CALL(
			pContext->PSSetSamplers(texture.first, 1u, texture.second.GetView().GetSamplerComPtr().GetAddressOf())
		);
	}
	for (const auto& buffer : _vertexBindedCBuffers)
	{
		D3D_CHECK_CALL(
			pContext->VSSetConstantBuffers(buffer.first, 1u, buffer.second.GetComPtr().GetAddressOf())
		);
	}
	for (const auto& buffer : _pixelBindedCBuffers)
	{
		D3D_CHECK_CALL(
			pContext->PSSetConstantBuffers(buffer.first, 1u, buffer.second.GetComPtr().GetAddressOf())
		);
	}

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

bool Material::ToggleDepth(bool value) noexcept
{
	bool lastState = _depthEnabled;

	_depthEnabled = value;

	return lastState;
}

void Material::SetCullMode(D3D11_CULL_MODE mode) noexcept
{
	_cullMode = mode;
}

void Material::SetVSTexture(const std::string& name, const Texture2D& texture)
{
	unsigned int index;

	try
	{
		index = _vertexReflectionDB.Textures.at(name);
	}
	catch (...)
	{
		THROW_BERROR("No Texture decelaration found to bind in shader.");
	}
	
	// if already obtained
	for (const auto& texture : _vertexBindedTextures)
	{
		if (texture.first == index)
			return;
	}

	_vertexBindedTextures.insert(std::move(std::pair<unsigned int, Texture2D>(index, texture)));
}

void Material::SetVSCBuffer(const std::string& name, const ConstantBuffer& buffer)
{
	unsigned int index = -1;
	for (auto i = 0; i < _vertexReflectionDB.ConstantBufferNames.size(); ++i)
	{
		if (_vertexReflectionDB.ConstantBufferNames[i] != name)
			continue;

		index = i;
	}

	if (index == -1)
	{
		THROW_BERROR("Unable to bind CBuffer");
	}

	// if already obtained
	for (const auto& buffer : _vertexBindedCBuffers)
	{
		if (buffer.first == index)
			return;
	}

	_vertexBindedCBuffers.insert(std::move(std::pair<unsigned int, ConstantBuffer>(index, buffer)));
}

void Material::SetPSTexture(const std::string& name, const Texture2D texture)
{
	unsigned int index;

	try
	{
		index = _pixelReflectionDB.Textures.at(name);
	}
	catch (...)
	{
		THROW_BERROR("No Texture decelaration found to bind in shader.");
	}

	// if already obtained
	for (const auto& texture : _pixelBindedTextures)
	{
		if (texture.first == index)
			return;
	}

	_pixelBindedTextures.insert(std::move(std::pair<unsigned int, Texture2D>(index, texture)));
}

void Material::SetPSCBuffer(const std::string& name, const ConstantBuffer buffer)
{
	unsigned int index = -1;
	for (auto i = 0; i < _pixelReflectionDB.ConstantBufferNames.size(); ++i)
	{
		if (_pixelReflectionDB.ConstantBufferNames[i] != name)
			continue;

		index = i;
	}

	if (index == -1)
	{
		THROW_BERROR("Unable to bind CBuffer");
	}

	// if already obtained
	for (const auto& buffer : _pixelBindedCBuffers)
	{
		if (buffer.first == index)
			return;
	}

	_pixelBindedCBuffers.insert(std::move(std::pair<unsigned int, ConstantBuffer>(index, buffer)));
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