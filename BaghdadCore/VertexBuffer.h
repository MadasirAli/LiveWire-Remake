#pragma once

#include "Buffer.h"

namespace BaghdadCore
{
	class VertexBuffer final : public Buffer
	{
	public:
		VertexBuffer(Buffer&& buffer);

		~VertexBuffer() noexcept override = default;
	};
}

