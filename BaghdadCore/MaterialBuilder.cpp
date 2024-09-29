#include "MaterialBuilder.h"

#include <wrl/client.h>
#include <memory>
#include <assert.h>
#include <utility>

#include "ShaderModule.h"
#include "Globals.h"
#include "GraphicsError.h"

using namespace BaghdadCore;

Material BaghdadCore::MaterialBuilder::Build()
{
	using namespace Microsoft::WRL;

	// creating vertex shader
	ShaderModule vertexModule = ShaderModule(_vertexShaderName);
	const auto& vertexBlob = vertexModule.Compile();

	ComPtr<ID3D11VertexShader> pVertexShader{};

	D3D_CALL(
		_device.GetComPtr()->CreateVertexShader(
			vertexBlob.GetComPtr()->GetBufferPointer(),
			vertexBlob.GetComPtr()->GetBufferSize(), nullptr,
			pVertexShader.ReleaseAndGetAddressOf())
	);

	_logger.WriteLine("Vertex Shader Created.");

	// creating pixel shader
	ShaderModule pixelModule = ShaderModule(_pixelShaderName);
	const auto& pixelBlob = pixelModule.Compile();

	ComPtr<ID3D11PixelShader> pPixelShader{};

	D3D_CALL(
		_device.GetComPtr()->CreatePixelShader(
			pixelBlob.GetComPtr()->GetBufferPointer(),
			pixelBlob.GetComPtr()->GetBufferSize(), nullptr,
			pPixelShader.ReleaseAndGetAddressOf())
	);

	_logger.WriteLine("Pixel Shader Created.");

	// creating vertex shader reflection database
	ShaderReflectionDB vertexReflectionDB = _CreateReflectionDB(vertexModule);

	// creating pixel shader reflection database
	ShaderReflectionDB pixelReflectionDB = _CreateReflectionDB(pixelModule);

	_logger.WriteLine("Material Created.");

	// creating input layout
	ComPtr<ID3D11InputLayout> pInputLayout{};
	D3D_CALL(
		_device.GetComPtr()->CreateInputLayout(vertexReflectionDB.InputDescs.data(),
			vertexReflectionDB.InputDescs.size(),
			vertexBlob.GetComPtr()->GetBufferPointer(),
			vertexBlob.GetComPtr()->GetBufferSize(),
			&pInputLayout)
	);

	return Material(
		std::move(VertexShader(std::move(pVertexShader))),
		std::move(PixelShader(std::move(pPixelShader))),
		std::move(vertexModule),
		std::move(vertexReflectionDB),
		std::move(pixelReflectionDB),
		std::move(pInputLayout));
}

ShaderReflectionDB MaterialBuilder::_CreateReflectionDB(const ShaderModule& shaderModule) const noexcept
{
	ShaderReflectionDB db{};

	const auto& reflector = shaderModule.GetComReflectionPointer();

	D3D11_SHADER_DESC desc = { 0 };
	reflector->GetDesc(&desc);

	// reflecting bindable resources
	for (auto i = 0; i < desc.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC inputDesc = { 0 };
		reflector->GetResourceBindingDesc(i, &inputDesc);

		// for textures
		if (inputDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE)
		{
			db.Textures.insert(std::move(std::pair<std::string, unsigned int>(
				std::string(inputDesc.Name), inputDesc.BindPoint)));
		}
		// for samplers
		else if (inputDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER)
		{
			db.SamplerNames.emplace_back(inputDesc.Name);
			db.SamplerRegisters.push_back(inputDesc.BindPoint);
		}
		// for constant buffers
		else if (inputDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER)
		{
			db.ConstantBufferNames.emplace_back(inputDesc.Name);
			db.ConstantBufferRegisters.push_back(inputDesc.BindPoint);
		}
	}

	// reflecting input layout or input params
	for (auto i = 0; i < desc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc = { 0 };
		reflector->GetInputParameterDesc(i, &paramDesc);

		// determine DXGI format
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		db.InputSementics.emplace_back(paramDesc.SemanticName);

		D3D11_INPUT_ELEMENT_DESC inputDesc = { 0 };
		inputDesc.SemanticName = db.InputSementics.back().c_str();
		inputDesc.SemanticIndex = paramDesc.SemanticIndex;
		inputDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputDesc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		inputDesc.InputSlot = 0u;
		inputDesc.InstanceDataStepRate = 0u; // cannot work with instanced rendering
		inputDesc.Format = format;

		db.InputDescs.emplace_back(std::move(inputDesc));
	}

	return db;
}

MaterialBuilder& BaghdadCore::MaterialBuilder::VS(const std::string& name)
{
	_vertexShaderName = std::string(name);

	return *this;
}

MaterialBuilder& MaterialBuilder::PS(const std::string& name)
{
	_pixelShaderName = std::string(name);

	return *this;
}

MaterialBuilder::MaterialBuilder(const Device& device) :
	_device(device),
	_logger(Globals::GetLogger())
{}