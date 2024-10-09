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
		BufferBuilder& Read() noexcept;
		BufferBuilder& Write() noexcept;
		BufferBuilder& Bind(D3D11_BIND_FLAG bindFlag) noexcept;

		BufferBuilder(const Device& device);

	private:
		const Device& _device;
		const Logger& _logger;

		bool _write = false;
		bool _read = false;
		unsigned int _bindFlags = (unsigned int)D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

		const char* _pData = nullptr;
		unsigned int _size = 0u;
	};
}

