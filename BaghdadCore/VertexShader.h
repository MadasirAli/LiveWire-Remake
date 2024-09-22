#pragma once

#include "Shader.h"

namespace BaghdadCore
{
	class VertexShader final : public Shader
	{
	public:
		const Microsoft::WRL::ComPtr<ID3D11VertexShader>& GetComPtr() const noexcept;

		VertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader>&& pShader);

		~VertexShader() noexcept override = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> _ptr;
	};
}

