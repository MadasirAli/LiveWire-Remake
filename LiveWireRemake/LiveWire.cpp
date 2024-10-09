#include "LiveWire.h"

#include "BaghdadCore/imgui.h"
#include "Globals.h"

using namespace LiveWireRemake;

void LiveWire::Update()
{
	auto& globals = Globals::GetInstance();
	auto& renderer = globals.GetRenderer();
	const auto& input = globals.GetInputManager();
	auto& worldManager = globals.GetWorldManager();

	ImGui::ShowDemoWindow();

	const auto mesh = renderer.GetMeshLoader()
		.Clear()
		.PrimitiveTriangle()
		.Load();
	const auto material = renderer.GetMaterialBuilder()
		.VS("Raw.vert")
		.PS("Raw.pix")
		.Build();

	renderer.DrawMesh(mesh, material);
}