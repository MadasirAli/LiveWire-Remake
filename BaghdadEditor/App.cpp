#include "App.h"

#include "BaghdadCore/Device.h"
#include "BaghdadCore/DeviceContext.h"
#include "BaghdadCore/Swapchain.h"
#include "BaghdadCore/imgui.h"
#include "BaghdadCore/Window.h"

using namespace BaghdadEditor;

int App::Run()
{
	BaghdadCore::Window window;

	ShowWindow(window.GetHwnd(), SW_SHOW);
	
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

				window.ForwardMessage(msg);
			}
		}

		// Now Code and execute here.
		//						//
	}

	return 0;
}
