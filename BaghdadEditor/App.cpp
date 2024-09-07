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

			while (window.PeekAndDispatchMessage(&msg))
			{
				if (msg.message == WM_QUIT)
					quit = true;
			}
		}

		// Now Code and execute here.
		//						//
	}

	return 0;
}
