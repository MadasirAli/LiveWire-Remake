#pragma once

#include <stdexcept>
#include <string>
#include <memory>

#include "Windows.h"

#define THROW_BERROR(msg) {														\
throw BaghdadError(msg + "\n\nFile: " + __FILE__ + "\nLine: " + std::to_string(__LINE__));		\
}																				\

#define THROW_LAST_WIN32_BERROR() {												\
const auto errorCode = GetLastError();											\
const auto pBuffer = std::make_unique<char>(513);								\
FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode, 0, (LPSTR)pBuffer.get(), 512, nullptr);	\
THROW_BERROR(std::string(pBuffer.get()));									    \
}																				\

#define WIN32_CALL(func)														\
{																				\
	const auto result = func;													\
	if (result != S_OK)															\
	{																			\
		THROW_LAST_WIN32_BERROR();												\
	}																			\
}																				\

namespace BaghdadCore
{
	class BaghdadError : 
		public std::runtime_error
	{
	public:
		BaghdadError(std::string msg);
	};
}