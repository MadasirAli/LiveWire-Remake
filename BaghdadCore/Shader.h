#pragma once

#include "DeviceChild.h"

namespace BaghdadCore
{

	class Shader : protected DeviceChild
	{
	protected:
		Shader(Microsoft::WRL::ComPtr<ID3D11DeviceChild>&& pShader);

		virtual ~Shader() noexcept override = default;
	};
}

