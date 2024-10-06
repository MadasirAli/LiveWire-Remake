#include "App.h"

#include "BaghdadCore/Window.h"
#include "BaghdadCore/Renderer.h"
#include "BaghdadCore/imgui.h"

#include "LiveWire.h"

using namespace LiveWireRemake;

int App::Run()
{
	// creating game window
	LiveWire liveWire{};
	ShowWindow(liveWire.GetHwnd(), SW_SHOW);

	// creating renderer
	BaghdadCore::Renderer renderer{};
	BaghdadCore::Swapchain swapchain = renderer.CreateSwapchain(liveWire);

	// creating render texture
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		swapchain.GetBackTexture().GetComPtr()->GetDesc(&desc);
		auto pRenderTexture = std::make_unique<BaghdadCore::Texture2D>(std::move(
			renderer.GetTextureBuilder()
			.Clear()
			.Format(desc.Format)
			.Size(desc.Width, desc.Height)
			.ViewFlag(BaghdadCore::Resource::View::Type::RTV)
			.Build()));

		renderer.SetRenderTexture(std::move(pRenderTexture));
	}

	renderer.InitializeImGUI(liveWire);

	// per tick data
	LiveWire::PerTickData data{ renderer };

	// main loop
	MSG msg = { 0 };
	bool quit = false;
	while (true)
	{
		{
			if (quit)
				break;

			while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					quit = true;
					continue;
				}

				// forwarding to imgui renderer
				renderer.IMGUI_ForwardMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);

				// forwarding to game window
				liveWire.ForwardMessage(msg);
			}
		}

		// setting renderer for new frame
		const float clearColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
		renderer.ClearRenderTexture(clearColor);

		renderer.ImGUI_NewFrame();

		//---------------------------//
		// Now Code and execute here //
		//---------------------------//
		//			HERE			 //
		
		liveWire.Update(data);

		//			HERE			 //
		//---------------------------//

		// rendering ui at end
		renderer.ImGUI_Render();

		// presenting
		renderer.Blit(renderer.GetRenderTexture(), swapchain.GetBackTexture(), 0u);
		swapchain.Present(0u, 0u);

		// resizing render texture
		swapchain.ResizeBuffers();

		D3D11_TEXTURE2D_DESC desc = { 0 };
		swapchain.GetBackTexture().GetComPtr()->GetDesc(&desc);
		auto pRenderTexture = std::make_unique<BaghdadCore::Texture2D>(std::move(
			renderer.GetTextureBuilder()
			.Clear()
			.Format(desc.Format)
			.Size(desc.Width, desc.Height)
			.ViewFlag(BaghdadCore::Resource::View::Type::RTV)
			.Build()));

		renderer.SetRenderTexture(std::move(pRenderTexture));
	}

	return 0;
}
