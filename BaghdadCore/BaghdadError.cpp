#include "BaghdadError.h"

#include <memory>

#include "Win32.h"
#include "Globals.h"

using namespace BaghdadCore;

void BaghdadError::Show() const noexcept
{
	MessageBoxA(nullptr, _msg.c_str(), "Error", MB_ICONERROR);
}

BaghdadError::BaghdadError(std::string msg) :
	runtime_error(msg) 
{
	_msg = std::move(msg);

	Globals::GetLogger().LogError(_msg);
}