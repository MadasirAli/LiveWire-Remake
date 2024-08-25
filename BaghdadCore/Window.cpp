#include "Window.h"

#include <assert.h>

#include "BaghdadError.h"

using namespace BaghdadCore;

void Window::InternalCreate(const std::string& name, RECT rect)
{
	const auto& wndClass = Class::Initialize();

	assert(
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, 0));

	const auto hwnd = CreateWindowExA(
		0,
		wndClass.Name.c_str(),
		name.c_str(),
		WS_OVERLAPPEDWINDOW,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		nullptr,
		nullptr, 
		GetModuleHandleA(nullptr),
		this);

	if(hwnd == nullptr)
		THROW_BERROR("Failed to create window.");
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}

HWND Window::GetHwnd() const noexcept
{
	return _hwnd;
}

Window::Window() :
	_hwnd()
{
	const std::string windowName = "Baghdad Window";

	InternalCreate(windowName, {0, 0, 800, 600});
}

bool Window::PeekAndDispatchMessage(MSG* pMsg) const noexcept
{
	assert(_hwnd != nullptr);

	bool result = false;
	result = PeekMessageW(pMsg, _hwnd, 0, 0, PM_REMOVE);

	if (pMsg->message == WM_QUIT)
	{
		return result;
	}

	TranslateMessage(pMsg);
	DispatchMessageW(pMsg);

	return result;
}

Window::Class::Class()
{
	WNDCLASSEXA wndClass = {0};

	wndClass.hInstance = GetModuleHandleA(nullptr);
	wndClass.lpszClassName = Name.c_str();
	wndClass.lpfnWndProc = InitWndProc;

	WIN32_CALL(
	RegisterClassExA(&wndClass));
}

const Window::Class& Window::Class::Initialize()
{
	static auto instance = Class();
	return instance;
}

Window::Class::~Class()
{
	UnregisterClassA(Name.c_str(), GetModuleHandleA(nullptr));
}

LRESULT CALLBACK BaghdadCore::StubWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const auto ptr = GetWindowLongPtrA(hwnd, GWLP_USERDATA);
	assert(ptr != 0);
	auto* pWindow = reinterpret_cast<Window*>(ptr);
	assert(pWindow != nullptr);
	return pWindow->WndProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK BaghdadCore::InitWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const auto* const ptr = reinterpret_cast<CREATESTRUCT*>(lParam);
		assert(ptr != nullptr);
		auto* const window = static_cast<Window*>(ptr->lpCreateParams);
		assert(window != nullptr);

		assert(
		SetWindowLongPtrA(hwnd, GWLP_WNDPROC, (LONG_PTR)StubWndProc) != 0);
		assert(
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)window) != 0);

		return window->WndProc(hwnd, msg, wParam, lParam);
	}
	else
	{
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
}