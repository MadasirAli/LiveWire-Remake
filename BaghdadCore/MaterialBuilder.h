#pragma once

#include "PixelShader.h"
#include "VertexShader.h"
#include "Device.h"
#include "Material.h"
#include "Logger.h"
#include "ShaderModule.h"

namespace BaghdadCore
{
	class MaterialBuilder final
	{
	public:
		Material Build();

		MaterialBuilder& VS(const std::string& name) noexcept;
		MaterialBuilder& PS(const std::string& name) noexcept;

		MaterialBuilder(const Device& device);

		MaterialBuilder(const MaterialBuilder& materialBuilder) = delete;
		MaterialBuilder& operator= (MaterialBuilder&& materialBuilder) noexcept = delete;

	private:
		ShaderReflectionDB _CreateReflectionDB(const ShaderModule& shaderModule) const noexcept;
	private:
		const Device& _device;

		std::string _vertexShaderName;
		std::string _pixelShaderName;

		const Logger& _logger;
	};
}