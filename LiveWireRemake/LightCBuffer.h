#pragma once

#include "BaghdadCore/D3D11.h"

namespace LiveWireRemake
{
	struct LightCBuffer
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR rotation;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 color;
	};
}
