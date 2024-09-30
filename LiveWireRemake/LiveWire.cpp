#include "LiveWire.h"

#include "BaghdadCore/imgui.h"

using namespace LiveWireRemake;

void LiveWire::Update(PerTickData& data)
{
	_logger.WriteLine("Live Wire Tick");

	ImGui::ShowDemoWindow();
}

LiveWire::PerTickData::PerTickData(BaghdadCore::Renderer& renderer) :
	Renderer(renderer)
{}
