#pragma once

#include <wrl/client.h>

#include "D3D11.h"

namespace BaghdadCore
{
	class Blob final
	{
	public:
		const Microsoft::WRL::ComPtr<ID3DBlob>& GetComPtr() const noexcept;

		Blob(Microsoft::WRL::ComPtr<ID3DBlob>&& pBlob);

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> _ptr;
	};
}