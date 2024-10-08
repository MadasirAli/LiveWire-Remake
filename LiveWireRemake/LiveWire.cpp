#include "LiveWire.h"

#include "BaghdadCore/imgui.h"

using namespace LiveWireRemake;

void LiveWire::Update(PerTickData& data)
{
	ImGui::ShowDemoWindow();

	const auto mesh = data.Renderer.GetMeshLoader()
		.Clear()
		.PrimitiveTriangle()
		.Load();
	const auto material = data.Renderer.GetMaterialBuilder()
		.VS("Raw.vert")
		.PS("Raw.pix")
		.Build();

	data.Renderer.DrawMesh(mesh, material);
}

LRESULT LiveWire::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Window::WndProc(hwnd, msg, wParam, lParam);
}

LiveWire::PerTickData::PerTickData(BaghdadCore::Renderer& renderer) :
	Renderer(renderer)
{}
