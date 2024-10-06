#include "LiveWire.h"

#include "BaghdadCore/imgui.h"

using namespace LiveWireRemake;

void LiveWire::Update(PerTickData& data)
{
	//_logger.WriteLine("Live Wire Tick");

	ImGui::ShowDemoWindow();
}

LRESULT LiveWire::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	_logger.WriteLine(std::to_string(msg));

	return Window::WndProc(hwnd, msg, wParam, lParam);
}

LiveWire::PerTickData::PerTickData(BaghdadCore::Renderer& renderer) :
	Renderer(renderer)
{}
