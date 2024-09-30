#include "Swapchain.h"

#include<memory>
#include "GraphicsError.h"

using namespace BaghdadCore;

bool Swapchain::Present(unsigned int syncInverval, unsigned int flags) const
{
#if _DEBUG
	DXGIInfoQueue::GetInstance().SetInfoQueue();
#endif

	const auto result = _pSwapchain->Present(syncInverval, flags);

#if _DEBUG
	if (result != S_OK && result != DXGI_STATUS_OCCLUDED)
	{
		if (DXGIInfoQueue::GetInstance().Check())
		{
			THROW_GERROR(DXGIInfoQueue::GetInstance().GetMessages());
		}
	}
#endif

	return result != DXGI_STATUS_OCCLUDED;
}

const Microsoft::WRL::ComPtr<IDXGISwapChain>& Swapchain::GetComPtr() const noexcept
{
	return _pSwapchain;
}

Swapchain::Swapchain(Microsoft::WRL::ComPtr<IDXGISwapChain>&& pOther) :
	_pSwapchain(std::move(pOther))
{}
