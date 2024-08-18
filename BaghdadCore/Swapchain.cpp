#include "Swapchain.h"

#include<memory>

using namespace BaghdadCore;

Swapchain::Swapchain(Microsoft::WRL::ComPtr<IDXGISwapChain>&& pOther) :
	_pSwapchain(std::move(pOther))
{}
