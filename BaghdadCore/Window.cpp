#include "Window.h"

using namespace BaghdadCore;

HWND Window::GetHwnd() const noexcept
{
	return _hwnd;
}

Window::Window() :
	_hwnd()
{}