#include "Renderer.h"

#include <wrl/client.h>

#include "BaghdadError.h"
#include "GraphicsError.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace BaghdadCore;

void Renderer::DrawMesh(const Mesh& mesh, const Material& material) const noexcept(!_DEBUG)
{
	const auto& context = _pDevice->GetDeviceContext();
	const auto& pContext = context.GetComPtr();

	/// binding mesh	
	mesh.Bind(*_pDevice, context);
	// binding material
	material.Bind(*_pDevice, context);

	// binding topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// binding view port
	D3D_CHECK_CALL(
		pContext->RSSetViewports(1u, &_viewport)
	);

	// binding output views
	D3D_CHECK_CALL(
		pContext->OMSetRenderTargets(1u,
			_pRenderTexture->GetView().GetRTVComPtr().GetAddressOf(),
			_pDepthTexture->GetView().GetDSVComPtr().Get())
	);

	// binding depth stencil state
	D3D_CHECK_CALL(
		pContext->OMSetDepthStencilState(_pDepthState.Get(), 0u)
	);

	// binding blend state
	constexpr float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3D_CHECK_CALL(
		pContext->OMSetBlendState(_pBlendState.Get(), blendFactor, ~0u)
	);

	// binding rasterizer
	D3D_CHECK_CALL(
		pContext->RSSetState(_pRasterizerState.Get())
	);

	// issuing draw call
	D3D_CHECK_CALL(
		context.GetComPtr()->Draw(mesh._vertexCount, 0u)
	);
}

void Renderer::Blit(const Texture2D& source, const Texture2D& destination) const noexcept(!_DEBUG)
{
	D3D_CHECK_CALL(
	_pDevice->GetDeviceContext()
		.GetComPtr()->CopyResource(
			source.GetComResourcePtr().Get(), destination.GetComResourcePtr().Get());
	);
}

void Renderer::Blit(const Texture2D& source, const Texture2D& destination, unsigned int subResourceIndex) const noexcept(!_DEBUG)
{
	D3D11_TEXTURE2D_DESC srcDesc = { 0 };
	D3D11_TEXTURE2D_DESC destDesc = { 0 };

	source.GetComPtr()->GetDesc(&srcDesc);
	destination.GetComPtr()->GetDesc(&destDesc);

	// calculating sub resource size
	auto width = srcDesc.Width;
	auto height = srcDesc.Height;
	for (auto i = 0u; i < subResourceIndex; ++i)
	{
		width /= 2u;
		height /= 2u;
	}
	
	// region to copy
	auto box =  D3D11_BOX();
	box.left = 0;
	box.top = 0;
	box.right = width;
	box.bottom = height;
	box.back = 1u;

	D3D_CHECK_CALL(
		_pDevice->GetDeviceContext()
		.GetComPtr()->CopySubresourceRegion(destination.GetComResourcePtr().Get(), subResourceIndex, 0u, 0u, 0u,
			source.GetComResourcePtr().Get(), subResourceIndex, &box)
	);
}

void Renderer::SetRenderTexture(std::unique_ptr<Texture2D>&& pRenderTexture)
{
	// accuring descriptor for width and height
	D3D11_TEXTURE2D_DESC desc;
	pRenderTexture->GetComPtr()->GetDesc(&desc);

	// creating new depth texture
	// ** can throw ** //
	Texture2D depthTexture = _pTextureBuilder->Clear()
		.Format(DXGI_FORMAT::DXGI_FORMAT_D16_UNORM)
		.Size(desc.Width, desc.Height)
		.ViewFlag(Resource::View::Type::DSV)
		.Build();

	D3D11_VIEWPORT viewport = { 0 };
	viewport.Width = (float)desc.Width;
	viewport.Height = (float)desc.Height;
	viewport.MaxDepth = 1.0f;

	_viewport = std::move(viewport);

	_pDepthTexture = std::move(std::make_unique<Texture2D>(std::move(depthTexture)));
	_pRenderTexture = std::move(pRenderTexture);
}

void Renderer::ClearRenderTexture(const float color[4]) const noexcept(!_DEBUG)
{
	const auto& pContext = _pDevice->GetDeviceContext().GetComPtr();

	D3D_CHECK_CALL(
		pContext->ClearRenderTargetView(
			_pRenderTexture->GetView().GetRTVComPtr().Get(),
			color)
	);

	D3D_CHECK_CALL(
		pContext->ClearDepthStencilView(_pDepthTexture->GetView().GetDSVComPtr().Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f, 0u)
	);
}

void Renderer::RemoveRenderTexture() noexcept
{
	_pRenderTexture.reset();
	_pDepthTexture.reset();
}

Texture2D& Renderer::GetRenderTexture() const noexcept
{
	return *_pRenderTexture;
}

void Renderer::ImGUI_NewFrame() const noexcept
{
	_pUIRenderer->NewFrame();
}

void Renderer::ImGUI_Render() const
{
	const auto& pContext = _pDevice->GetDeviceContext().GetComPtr();

	ImGui::Render();

	pContext->ClearState();

	// binding view port
	D3D_CHECK_CALL(
		pContext->RSSetViewports(1u, &_viewport)
	);

	// binding output views
	D3D_CHECK_CALL(
		pContext->OMSetRenderTargets(1u,
			_pRenderTexture->GetView().GetRTVComPtr().GetAddressOf(),
			nullptr)
	);

	// binding blend state
	constexpr float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3D_CHECK_CALL(
		pContext->OMSetBlendState(_pBlendState.Get(), blendFactor, ~0u)
	);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool Renderer::IMGUI_ForwardMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return _pUIRenderer->ForwardMessage(hwnd, msg, wParam, lParam);
}

void Renderer::InitializeImGUI(const Window& window) noexcept
{
	auto pUIRenderer = std::make_unique<ImGuiRenderer>(window, *_pDevice);
	_pUIRenderer = std::move(pUIRenderer);
}

Swapchain Renderer::CreateSwapchain(const Window& window) const
{
	// ** can throw ** //
	auto swapchain = _factory.CreateSwapchain(*_pDevice, window);
	return swapchain;
}

TextureBuilder& Renderer::GetTextureBuilder() const noexcept
{
	return *_pTextureBuilder;
}

MaterialBuilder& Renderer::GetMaterialBuilder() const noexcept
{
	return *_pMaterialBuilder;
}

MeshLoader& Renderer::GetMeshLoader() const noexcept
{
	return *_pMeshLoader;
}

BufferBuilder& Renderer::GetBufferBuilder() const noexcept
{
	return *_pBufferBuilder;
}

Renderer::Renderer()
{
	auto cards = _factory.EnumGraphicCards();
	if (cards.size() < 1)
	{
		THROW_BERROR("No Graphics Capable Adapter (Graphics Card) found !");
	}

	_pCard = std::make_unique<GraphicsCard>(std::move(cards[0]));
	_pDevice = std::make_unique<Device>(*_pCard);
	_pMaterialBuilder = std::make_unique<MaterialBuilder>(*_pDevice);
	_pMeshLoader = std::make_unique<MeshLoader>(*_pDevice);
	_pTextureBuilder = std::make_unique<TextureBuilder>(*_pDevice);
	_pBufferBuilder = std::make_unique<BufferBuilder>(*_pDevice);

	using namespace Microsoft::WRL;

	const auto& pDevice = _pDevice->GetComPtr();
	const auto& pContext = _pDevice->GetDeviceContext().GetComPtr();

	// creating depth state
	{
		D3D11_DEPTH_STENCIL_DESC desc = { 0 };
		desc.DepthEnable = true;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		ComPtr<ID3D11DepthStencilState> pDepthState{ };
		D3D_CALL(
			pDevice->CreateDepthStencilState(&desc, pDepthState.ReleaseAndGetAddressOf())
		);

		_pDepthState = std::move(pDepthState);
	}

	// creating blend state
	{
		D3D11_BLEND_DESC desc = { 0 };
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_COLOR;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_DEST_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		ComPtr<ID3D11BlendState> pBlendState{};
		D3D_CALL(
			pDevice->CreateBlendState(&desc, pBlendState.ReleaseAndGetAddressOf())
		);

		_pBlendState = std::move(pBlendState);
	}

	// creating rasterizer state
	{
		D3D11_RASTERIZER_DESC desc = {};
		ZeroMemory(&desc, sizeof(desc));
		
		desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		desc.DepthClipEnable = true;
		
		ComPtr<ID3D11RasterizerState> pRastState{};
		D3D_CALL(
			pDevice->CreateRasterizerState(&desc, pRastState.ReleaseAndGetAddressOf())
		);
		_pRasterizerState = std::move(pRastState);
	}

	// constructing initial viewport
	{
		D3D11_VIEWPORT viewport = { 0 };
		viewport.Width = 800;
		viewport.Height = 600;
		viewport.MaxDepth = 1;

		_viewport = std::move(viewport);
	}

	// creating default render and depth texture
	{
		// ** can throw ** //
		Texture2D renderTexture = _pTextureBuilder->Clear()
			.Size(800u, 600u)
			.ViewFlag(Resource::View::Type::RTV)
			.Format(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
			.Build();

		// ** can throw ** //
		Texture2D depthTexture = _pTextureBuilder->Clear()
			.Format(DXGI_FORMAT::DXGI_FORMAT_D16_UNORM)
			.Size(800u, 600u)
			.ViewFlag(Resource::View::Type::DSV)
			.Build();

		_pDepthTexture = std::move(std::make_unique<Texture2D>(std::move(depthTexture)));
		_pRenderTexture = std::move(std::make_unique<Texture2D>(std::move(renderTexture)));
	}
}

void Renderer::ImGuiRenderer::NewFrame() const noexcept
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

bool Renderer::ImGuiRenderer::ForwardMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
}

Renderer::ImGuiRenderer::ImGuiRenderer(const Window& window, const Device& device)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window.GetHwnd());
	ImGui_ImplDX11_Init(device.GetComPtr().Get(), device.GetDeviceContext().GetComPtr().Get());
}

Renderer::ImGuiRenderer::~ImGuiRenderer() noexcept
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}