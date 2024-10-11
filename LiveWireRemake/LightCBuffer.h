#pragma once

#include "BaghdadCore/D3D11.h"

namespace LiveWireRemake
{
	struct LightCBuffer
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 rotation;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 color;
	};
}
