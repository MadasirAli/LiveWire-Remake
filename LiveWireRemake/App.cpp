#include "App.h"

#include <chrono>

#include "BaghdadCore/Window.h"
#include "BaghdadCore/Renderer.h"
#include "BaghdadCore/imgui.h"

#include "LiveWire.h"
#include "Globals.h"

using namespace LiveWireRemake;

int App::Run()
{
	// ________________ GETTINGS THINGS READY __________________ //

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

	// creating world manager
	WorldManager worldManager{};

	// creating input manager
	InputManager inputManager{};

	// creating logger
	BaghdadCore::Logger logger{};

	Globals::Data data{renderer};

	// setting up globals
	auto& globals = Globals::GetInstance();
	globals._pRenderer = &renderer;
	globals._pWorldManager = &worldManager;
	globals._pInputManager = &inputManager;
	globals._pLogger = &logger;
	globals._pData = &data;

	{
		RECT rect = { 0 };
		GetClientRect(liveWire.GetHwnd(), &rect);

		globals._screenWidth = (float)(rect.right - rect.left);
		globals._screenHeight = (float)(rect.bottom - rect.top);
	}
	// _________________________________________________________ //

	liveWire.Start();

	// main loop
	MSG msg = { 0 };
	bool quit = false;
	while (true)
	{
		// taking start time stamp
		const auto startTimeStamp = std::chrono::high_resolution_clock::now();

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

				// feeding to input manager
				inputManager.Push(msg);

				// forwarding to imgui renderer
				renderer.IMGUI_ForwardMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);

				// forwarding to game window
				liveWire.ForwardMessage(msg);
			}
		}

		// setting renderer for new frame
		const float clearColor[4] = { 0.0f, 0, 0.0f, 1.0f };
		renderer.ClearRenderTexture(clearColor);

		renderer.ImGUI_NewFrame();

		// DEBUG SWITCHES 
		if (inputManager.GetKeyDown(InputManager::Key::F1))
		{
			globals._gizmosEnabled = !globals._gizmosEnabled;
		}
		if (inputManager.GetKeyDown(InputManager::Key::F2))
		{
			globals._renderGraphics = !globals._renderGraphics;
		}

		ImGui::Text(std::to_string(1.0f / globals._deltaTime).c_str());

		if (globals._gizmosEnabled)
			ImGui::Text("Gizmos: True");
		else
			ImGui::Text("Gizmos: False");

		if (globals._renderGraphics)
			ImGui::Text("Graphics: True");
		else
			ImGui::Text("Graphics: False");
		//				//

		//---------------------------//
		// Now Code and execute here //
		//---------------------------//
		//			HERE			 //
		
		liveWire.Update();
		inputManager.Flush();
		//			HERE			 //
		//---------------------------//

		// rendering ui at end
		renderer.ImGUI_Render();

		// presenting
		renderer.Blit(renderer.GetRenderTexture(), swapchain.GetBackTexture(), 0u);
#ifdef _DEBUG
		swapchain.Present(1u, 0u);
#else
		swapchain.Present(0u, 0u);
#endif

		//// resizing render texture
		//swapchain.ResizeBuffers();

		//D3D11_TEXTURE2D_DESC desc = { 0 };
		//swapchain.GetBackTexture().GetComPtr()->GetDesc(&desc);
		//auto pRenderTexture = std::make_unique<BaghdadCore::Texture2D>(std::move(
		//	renderer.GetTextureBuilder()
		//	.Clear()
		//	.Format(desc.Format)
		//	.Size(desc.Width, desc.Height)
		//	.ViewFlag(BaghdadCore::Resource::View::Type::RTV)
		//	.Build()));

		//renderer.SetRenderTexture(std::move(pRenderTexture));

		// updating globals

		const auto endTimeStamp = std::chrono::high_resolution_clock::now();
		float deltaTime = (std::chrono::duration_cast<std::chrono::microseconds>(endTimeStamp - startTimeStamp).count() / 1000.0f) / 1000.0f;
		globals._deltaTime = deltaTime;

		{
			RECT rect = { 0 };
			GetClientRect(liveWire.GetHwnd(), &rect);

			globals._screenWidth = (float)(rect.right - rect.left);
			globals._screenHeight = (float)(rect.bottom - rect.top);
		}
	}

	return 0;
}
