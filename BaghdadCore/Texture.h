#pragma once

#include "Resource.h"

namespace BaghdadCore
{
	class Texture : public Resource
	{
	protected:
		Texture(Microsoft::WRL::ComPtr<ID3D11Resource>&& pResource, View&& view);

		virtual ~Texture() noexcept override = default;
	};
}