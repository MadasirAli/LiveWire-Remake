#pragma once

#include <wrl/client.h>
#include <vector>

#include "D3D11.h"
#include "GraphicsCard.h"
#include "Swapchain.h"
#include "Window.h"
#include "Device.h"

namespace BaghdadCore
{
	class DXGIFactory {
	public:
		Swapchain CreateSwapchain(const Device& device, const Window& window) const;
		std::vector<GraphicsCard> EnumGraphicCards() const;

		DXGIFactory();

	private:
		Microsoft::WRL::ComPtr<IDXGIFactory> _ptr;
	};
}