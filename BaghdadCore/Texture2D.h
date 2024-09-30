#pragma once

#include "Texture.h"

namespace  BaghdadCore
{
	class Texture2D final : public Texture
	{
	public:
		const Microsoft::WRL::ComPtr<ID3D11Texture2D>& GetComPtr() const noexcept;

		Texture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D>&& pTexture, View&& view);

		~Texture2D() noexcept override = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _ptr;
	};
}

