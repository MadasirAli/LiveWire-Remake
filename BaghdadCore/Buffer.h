#pragma once

#include "Resource.h"

namespace BaghdadCore
{
	class Buffer : public Resource
	{
	public:
		const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetComPtr() const noexcept;

		Buffer(Microsoft::WRL::ComPtr<ID3D11Buffer>&& pBuffer);

		virtual ~Buffer() noexcept override = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> _ptr;
	};
}

