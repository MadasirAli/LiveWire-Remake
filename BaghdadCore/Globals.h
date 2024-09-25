#pragma once

#include "StartupFlags.h"
#include "Logger.h"

namespace BaghdadCore
{
	class Globals final
	{
	public:
		static const Logger& GetLogger() noexcept;
		static const StartupFlags& GetStartupFlags() noexcept;

	private:
		Globals() = default;

	private:
		static StartupFlags _startupFlags;
		static Logger _logger;
	};
}

