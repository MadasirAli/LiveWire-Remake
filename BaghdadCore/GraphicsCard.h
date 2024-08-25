#pragma once

#include <wrl/client.h>

#include "D3D11.h"

namespace BaghdadCore
{
	class GraphicsCard final
	{
	public:
		Microsoft::WRL::ComPtr<IDXGIAdapter> GetNativePointer() const noexcept;
		GraphicsCard(Microsoft::WRL::ComPtr<IDXGIAdapter>&& pOther);

	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter> _ptr;
	};
}