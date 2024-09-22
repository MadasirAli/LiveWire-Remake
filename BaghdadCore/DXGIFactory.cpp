#include "DXGIFactory.h"

#include "BaghdadError.h"

using namespace BaghdadCore;

Swapchain DXGIFactory::CreateSwapchain(const Device& device, const Window& window) const
{
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapchain;

	DXGI_SWAP_CHAIN_DESC desc{};
	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.OutputWindow = window.GetHwnd();
	desc.Windowed = true;

	WIN32_CALL(
	_ptr->CreateSwapChain(
		device.GetComPtr().Get(),
		&desc,
		pSwapchain.ReleaseAndGetAddressOf()
	));

	const auto swapchain = Swapchain(std::move(pSwapchain));
	return swapchain;
}

std::vector<GraphicsCard> DXGIFactory::EnumGraphicCards() const
{
	auto i = 0;

	std::vector<GraphicsCard> cards;
	Microsoft::WRL::ComPtr<IDXGIAdapter> pCard;
	while (_ptr->EnumAdapters(i, pCard.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		auto graphicsCard = GraphicsCard(std::move(pCard));
		cards.push_back(std::move(graphicsCard));
	}

	return cards;
}

DXGIFactory::DXGIFactory() {
	WIN32_CALL(
		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)_ptr.ReleaseAndGetAddressOf()));
}