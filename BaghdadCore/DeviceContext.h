#pragma once

#include <wrl/client.h>

#include "D3D11.h"

namespace BaghdadCore
{
	class DeviceContext final
	{
	public:
		const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetComPtr() const noexcept;

		DeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext>&& pOther);

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _ptr;
	};
}
