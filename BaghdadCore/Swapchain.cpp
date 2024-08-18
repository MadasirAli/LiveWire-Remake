#include "Swapchain.h"

#include<memory>
#include "GraphicsError.h"

using namespace BaghdadCore;

void Swapchain::Present(unsigned int syncInverval, unsigned int flags)
{
	D3D_CHECK_CALL(
	_pSwapchain->Present(syncInverval, flags));
}

Swapchain::Swapchain(Microsoft::WRL::ComPtr<IDXGISwapChain>&& pOther) :
	_pSwapchain(std::move(pOther))
{}
