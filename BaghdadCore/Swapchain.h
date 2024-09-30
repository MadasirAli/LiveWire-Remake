#pragma once

#include <wrl/client.h>

#include "D3D11.h"
#include "Window.h"

namespace BaghdadCore
{
	class Swapchain final
	{
	public:
		/// <summary>
		/// Present on screen
		/// </summary>
		/// <param name="syncInverval"></param>
		/// <param name="flags"></param>
		/// <returns>False if swapchain occuluded</returns>
		bool Present(unsigned int syncInverval, unsigned int flags) const noexcept(!_DEBUG);

		const Microsoft::WRL::ComPtr<IDXGISwapChain>& GetComPtr() const noexcept;

		Swapchain(Microsoft::WRL::ComPtr<IDXGISwapChain>&& pOther);

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> _pSwapchain;
	};
}

