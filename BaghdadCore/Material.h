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
		class MaterialBuilder;
		friend class MaterialBuilder;

		class Renderer;
		friend class Renderer;
		
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

