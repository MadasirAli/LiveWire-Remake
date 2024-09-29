#pragma once

#include "Device.h"
#include "Logger.h"
#include "ConstantBuffer.h"

namespace BaghdadCore
{
	class BufferBuilder final
	{
	public:
		ConstantBuffer BuildCBuffer();

		BufferBuilder& Clear() noexcept;
		BufferBuilder& InitialData(const char* pData, const unsigned int size) noexcept;
		BufferBuilder& Writeable() noexcept;

		BufferBuilder(const Device& device);

	private:
		const Device& _device;
		const Logger& _logger;

		bool _write_able = true;
		const char* _pData = nullptr;
		unsigned int _size = 0u;
	};
}

