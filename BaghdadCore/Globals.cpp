#include "Globals.h"

using namespace BaghdadCore;

const Logger& BaghdadCore::Globals::GetLogger() const noexcept
{
    return _logger;
}

const StartupFlags& Globals::GetStartupFlags() const noexcept
{
    return _startupFlags;
}