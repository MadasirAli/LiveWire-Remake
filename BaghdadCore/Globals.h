#pragma once

#include "StartupFlags.h"
#include "Logger.h"

namespace BaghdadCore
{
	class Globals final
	{
	public:
		const Logger& GetLogger() const noexcept;
		const StartupFlags& GetStartupFlags() const noexcept;

	private:
		StartupFlags _startupFlags;
		Logger _logger;
	};
}

