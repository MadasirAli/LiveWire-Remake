#pragma once

#include <string>
#include <memory>
#include <wrl/client.h>

#include "Blob.h"
#include "Logger.h"
#include "Globals.h"

namespace BaghdadCore
{
	class ShaderModule final
	{
	public:
		const Microsoft::WRL::ComPtr<ID3D11ShaderReflection>& GetComReflectionPointer() const noexcept;
		ShaderModule& EntryPoint(std::string& entryName);
		ShaderModule& TargetFeatureLevel(const std::string& level) noexcept;

		const Blob& GetBlob() const noexcept;
		bool IsCompiled() const noexcept;

		ShaderModule(const std::string& name);

		/// <summary>
		/// Compiles the shader.
		/// </summary>
		/// <returns>Compiled shader blob.</returns>
		/// <exception cref="BaghdadError">On compilation failed or failing to acuire reflection interface.</exception>
		const Blob& Compile();

	private:
		std::string _entryPoint = "main";
		std::string _targetFeatureLevel = "cs_5_0";

		bool _compiled;
		std::string _nName;
		std::wstring _name;
		std::unique_ptr<Blob> _pBlob;

		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> _pReflection;

		const Logger& _logger;
	};
}
