#pragma once

#include "Buffer.h"

namespace BaghdadCore
{
	class IndexBuffer final : public Buffer
	{
	public:
		IndexBuffer(Buffer&& buffer);

		~IndexBuffer() noexcept override = default;
	};
}

