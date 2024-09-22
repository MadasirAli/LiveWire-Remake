#pragma once

#include <wrl/Client.h>

#include "D3D11.h"

namespace BaghdadCore
{
	class DeviceChild
	{
	protected:
		void GetDevice(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice) const noexcept;

		DeviceChild(Microsoft::WRL::ComPtr<ID3D11DeviceChild>&& ptr);

		virtual ~DeviceChild() noexcept = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> _ptr;
	};
}

