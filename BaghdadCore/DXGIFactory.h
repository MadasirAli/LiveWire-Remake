#pragma once

#include <wrl/client.h>

#include "D3D11.h"

namespace BaghdadCore
{
	class DXGIFactory {
	public:
		DXGIFactory();

	private:
		Microsoft::WRL::ComPtr<IDXGIFactory> _ptr;
	};
}