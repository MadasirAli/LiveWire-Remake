#pragma once

#include <memory>

#include "PixelShader.h"
#include "VertexShader.h"
#include "ShaderReflectionDB.h"
#include "ShaderModule.h"

namespace BaghdadCore
{
	class Material final
	{
		friend class MaterialBuilder;

	public:
		void Bind() const;

	private:
		Material(VertexShader&& vertexShader, PixelShader&& pixelShader, 
			ShaderModule&& vertexModule,
			ShaderReflectionDB&& vertexReflectionDB, ShaderReflectionDB&& pixelReflectionDB);

	private:
		VertexShader _vertexShader;
		PixelShader _pixelShader;
		ShaderModule _vertexModule;
		ShaderReflectionDB _pixelReflectionDB;
		ShaderReflectionDB _vertexReflectionDB;
	};
}

