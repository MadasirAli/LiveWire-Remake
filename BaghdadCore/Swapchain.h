#pragma once

#include <wrl/client.h>

#include "D3D11.h"
#include "Window.h"

namespace BaghdadCore
{
	class Swapchain
	{
	public:
		Swapchain(Microsoft::WRL::ComPtr<IDXGISwapChain>&& pOther);

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> _pSwapchain;
	};
}

