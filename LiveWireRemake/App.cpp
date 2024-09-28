#include "App.h"

#include "BaghdadCore/Window.h"

#include "LiveWire.h"

using namespace LiveWireRemake;

int App::Run()
{
	LiveWire liveWire{};

	ShowWindow(liveWire.GetHwnd(), SW_SHOW);

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

				liveWire.ForwardMessage(msg);
			}
		}

		// Now Code and execute here.
		//			HERE			//
		//							//
	}

	return 0;
}
