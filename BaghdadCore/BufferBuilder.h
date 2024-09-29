#pragma once

#include "Device.h"
#include "Logger.h"

namespace BaghdadCore
{
	class BufferBuilder final
	{
	public:
		BufferBuilder(const Device& device);

	private:
		const Device& _device;
		const Logger& _logger;
	};
}

