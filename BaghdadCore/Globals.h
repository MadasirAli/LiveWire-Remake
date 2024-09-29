#pragma once

#include "StartupFlags.h"
#include "Logger.h"

namespace BaghdadCore
{
	class Globals final
	{
	public:
		static const Logger& GetLogger() noexcept;

	private:
		Globals() = default;
	};
}

