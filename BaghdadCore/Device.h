#pragma once

#include <wrl/client.h>
#include <memory>

#include "GraphicsCard.h"
#include "DeviceContext.h"

namespace BaghdadCore
{
	class Device
	{
	public:
		const DeviceContext& GetDeviceContext() const noexcept;
		Microsoft::WRL::ComPtr<ID3D11Device> GetNativePointer() const noexcept;
		Device(const GraphicsCard& card);

	private:
		std::unique_ptr<DeviceContext> _pContext;
		D3D_FEATURE_LEVEL _featureLevel;
		Microsoft::WRL::ComPtr<ID3D11Device> _ptr;
	};
}

