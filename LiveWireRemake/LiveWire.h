#pragma once

#include "BaghdadCore/Window.h"
#include "BaghdadCore/Renderer.h"
#include "BaghdadCore/Logger.h"

namespace LiveWireRemake
{
	class LiveWire final : public BaghdadCore::Window
	{
	public:
		class PerTickData final
		{
		public:
			PerTickData(BaghdadCore::Renderer& renderer);

		public:
			BaghdadCore::Renderer& Renderer;
		};

	public:
		void Update(PerTickData& data);

		~LiveWire() noexcept override = default;

	protected:
		virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	private:
		BaghdadCore::Logger _logger;
	};
}

