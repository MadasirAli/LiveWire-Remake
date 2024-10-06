#pragma once

#include <memory>
#include <wrl/client.h>
#include <unordered_map>

#include "PixelShader.h"
#include "VertexShader.h"
#include "ShaderReflectionDB.h"
#include "ShaderModule.h"
#include "Texture2D.h"
#include "ConstantBuffer.h"

#include "Device.h"
#include "DeviceContext.h"

namespace BaghdadCore
{
	class Material final
	{
		friend class MaterialBuilder;
		friend class Renderer;

	public:
		/// <summary>
		/// Binds texture to material or shader.
		/// </summary>
		/// <param name="name">The deceleration name in shader</param>
		/// <param name="texture">The texture to bind.</param>
		/// <exception cref="BaghdadError">On Failture.</exception>
		void SetVSTexture(const std::string& name, const Texture2D& texture);
		/// <summary>
		/// Binds constant buffer to material or shader.
		/// </summary>
		/// <param name="name">The deceleration of the variable.</param>
		/// <param name="buffer">The constant buffer to bind.</param>
		void SetVSCBuffer(const std::string& name, const ConstantBuffer& buffer);

		/// <summary>
		/// Binds texture to material or shader.
		/// </summary>
		/// <param name="name">The deceleration name in shader</param>
		/// <param name="texture">The texture to bind.</param>
		/// <exception cref="BaghdadError">On Failture.</exception>
		void SetPSTexture(const std::string& name, const Texture2D texture);
		/// <summary>
		/// Binds constant buffer to material or shader.
		/// </summary>
		/// <param name="name">The deceleration of the variable.</param>
		/// <param name="buffer">The constant buffer to bind.</param>
		void SetPSCBuffer(const std::string& name, const ConstantBuffer buffer);
		
	private:
		void Bind(const Device& device, const DeviceContext& context) const noexcept(!_DEBUG);

		Material(VertexShader&& vertexShader, PixelShader&& pixelShader, 
			ShaderModule&& vertexModule,
			ShaderReflectionDB&& vertexReflectionDB, ShaderReflectionDB&& pixelReflectionDB,
			Microsoft::WRL::ComPtr<ID3D11InputLayout>&& pInputLayout);

	private:
		VertexShader _vertexShader;
		PixelShader _pixelShader;
		ShaderModule _vertexModule;
		ShaderReflectionDB _pixelReflectionDB;
		ShaderReflectionDB _vertexReflectionDB;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> _pInputLayout;

		std::unordered_map<unsigned int, Texture2D> _vertexBindedTextures;
		std::unordered_map<unsigned int, ConstantBuffer> _vertexBindedCBuffers;
		std::unordered_map<unsigned int, Texture2D> _pixelBindedTextures;
		std::unordered_map<unsigned int, ConstantBuffer> _pixelBindedCBuffers;
	};
}

