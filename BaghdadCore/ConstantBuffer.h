#pragma once

#include "Buffer.h"

namespace BaghdadCore
{
	class ConstantBuffer final : public Buffer
	{
	public:
		ConstantBuffer(Buffer&& buffer);

		~ConstantBuffer() noexcept override = default;
	};
}

