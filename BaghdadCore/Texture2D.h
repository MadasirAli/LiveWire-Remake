#pragma once

#include "Texture.h"

namespace  BaghdadCore
{
	class Texture2D final : public Texture
	{
	public:
		Texture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D>&& pTexture);

		~Texture2D() noexcept override = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _ptr;
	};
}

