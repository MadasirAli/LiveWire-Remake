#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "D3D11.h"

namespace BaghdadCore
{
	struct ShaderReflectionDB final
	{
		std::vector<std::string> InputSementics;
		std::vector<D3D11_INPUT_ELEMENT_DESC> InputDescs;

		std::unordered_map<std::string, unsigned int> Textures;

		std::vector<std::string> SamplerNames;
		std::vector<unsigned int> SamplerRegisters;

		std::vector<std::string> ConstantBufferNames;
		std::vector<unsigned int> ConstantBufferRegisters;
	};
}

