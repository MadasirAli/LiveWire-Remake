#include "Globals.h"

using namespace BaghdadCore;

const Logger& BaghdadCore::Globals::GetLogger() noexcept
{
    static Logger logger{};
    return logger;
}