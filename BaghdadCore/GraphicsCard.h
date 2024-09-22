#pragma once

#include <wrl/client.h>

#include "D3D11.h"

namespace BaghdadCore
{
	class GraphicsCard final
	{
	public:
		const Microsoft::WRL::ComPtr<IDXGIAdapter>& GetComPtr() const noexcept;
		GraphicsCard(Microsoft::WRL::ComPtr<IDXGIAdapter>&& pOther);

	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter> _ptr;
	};
}