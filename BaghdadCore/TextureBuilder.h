#pragma once

#include "Device.h"

namespace BaghdadCore
{
	class TextureBuilder final
	{
	public:
		TextureBuilder(const Device& device);

	private:
		const Device& _device;
	};
}

