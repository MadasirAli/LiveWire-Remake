#pragma once

#include "BaghdadCore/Window.h"
#include "BaghdadCore/Renderer.h"
#include "BaghdadCore/Logger.h"

namespace LiveWireRemake
{
	class LiveWire final : public BaghdadCore::Window
	{
	public:
		void Start();
		void Update();

		~LiveWire() noexcept override = default;
	};
}

