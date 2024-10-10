#pragma once

#include "BaghdadCore/D3D11.h"

namespace LiveWireRemake
{
	struct CameraCBuffer
	{
		DirectX::XMMATRIX ViewMatrix;
		DirectX::XMMATRIX ProjectionMatrix;
	};
}
