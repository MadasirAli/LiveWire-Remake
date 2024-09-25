#include "Globals.h"

using namespace BaghdadCore;

const Logger& BaghdadCore::Globals::GetLogger() noexcept
{
    return _logger;
}

const StartupFlags& Globals::GetStartupFlags() noexcept
{
    return _startupFlags;
}