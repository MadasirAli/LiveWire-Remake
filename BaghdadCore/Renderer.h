#pragma once

#include <memory>

#include "DXGIFactory.h"
#include "GraphicsCard.h"
#include "Device.h"
#include "Swapchain.h"
#include "MaterialBuilder.h"

namespace BaghdadCore
{
	class Renderer final
	{
	public:
		Renderer();

		Renderer(const Renderer& renderer) = delete;

		Renderer& operator= (const Renderer& renderer) = delete;

	private:
		DXGIFactory _factory;
		std::unique_ptr<GraphicsCard> _pCard;
		std::unique_ptr<Device> _pDevice;
		std::unique_ptr<Swapchain> _pSwapchain;

		std::unique_ptr<MaterialBuilder> _pMaterialBuilder;
	};
}