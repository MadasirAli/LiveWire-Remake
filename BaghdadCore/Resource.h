#pragma once

#include <wrl/client.h>

#include "DeviceChild.h"

namespace BaghdadCore
{
	class Resource : protected DeviceChild
	{
	public:
		const Microsoft::WRL::ComPtr<ID3D11Resource>& GetComResourcePtr() const noexcept;

	protected:
		Resource(Microsoft::WRL::ComPtr<ID3D11Resource>&& pResource);

		virtual ~Resource() noexcept override = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11Resource> _ptr;
	};
}
