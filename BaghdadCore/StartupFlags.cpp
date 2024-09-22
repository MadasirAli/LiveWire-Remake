#include "StartupFlags.h"

using namespace BaghdadCore;

bool StartupFlags::IsLogging() const noexcept
{
	return _logging;
}

StartupFlags::StartupFlags(bool logging) :
	_logging(logging)
{}
