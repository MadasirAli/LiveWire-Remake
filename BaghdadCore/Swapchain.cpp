#include "Swapchain.h"

#include "GraphicsError.h"
#include "ComUtility.h"

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

Texture2D& Swapchain::GetBackTexture() const noexcept
{
	return *_pBackBuffer;
}

const Microsoft::WRL::ComPtr<IDXGISwapChain>& Swapchain::GetComPtr() const noexcept
{
	return _pSwapchain;
}

Swapchain::Swapchain(Microsoft::WRL::ComPtr<IDXGISwapChain>&& pOther) :
	_pSwapchain(std::move(pOther))
{
	// obtaining back buffer
	Microsoft::WRL::ComPtr<ID3D11Resource> pBuffer{};
	D3D_CALL(
		_pSwapchain->GetBuffer(0u, __uuidof(ID3D11Resource), (void**)pBuffer.ReleaseAndGetAddressOf())
	);

	_pBackBuffer = std::move(std::make_unique<Texture2D>(
		ComUtility::As<ID3D11Resource, ID3D11Texture2D>(pBuffer), Resource::View()));
}
