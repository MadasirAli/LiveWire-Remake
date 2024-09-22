#pragma once

#include "Shader.h"

namespace BaghdadCore
{
	class PixelShader final : public Shader
	{
	public:
		const Microsoft::WRL::ComPtr<ID3D11PixelShader>& GetComPtr() const noexcept;

		PixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader>&& pShader);

		~PixelShader() noexcept override = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> _ptr;
	};
}