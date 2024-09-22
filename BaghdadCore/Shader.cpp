#include "Shader.h"

#include <memory>

using namespace BaghdadCore;

Shader::Shader(Microsoft::WRL::ComPtr<ID3D11DeviceChild>&& pShader) :
	DeviceChild(std::move(pShader))
{}
