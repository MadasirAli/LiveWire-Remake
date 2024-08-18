#pragma once

#include <string>

#include "Windows.h"

namespace BaghdadCore
{
	class Window
	{
	public:
		HWND GetHwnd() const noexcept;

		Window();

	private:
		HWND _hwnd;
	};
}

