#pragma once

#include <stdexcept>
#include <string>
#include <memory>

#include "Win32.h"

#define THROW_BERROR(msg) {														\
throw BaghdadError(std::string(msg) + std::string("\n\nFile: ") + std::string(__FILE__) + std::string("\nLine: ") + std::to_string(__LINE__));		\
}																				\

#define THROW_LAST_WIN32_BERROR() {												\
const auto errorCode = GetLastError();											\
const auto pBuffer = std::make_unique<TCHAR[]>(512);							\
FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode, 0, (LPSTR)pBuffer.get(), 512, nullptr);	\
THROW_BERROR(std::string((char*)pBuffer.get()));									    \
}																				\

#define WIN32_CALL(func)														\
{																				\
	const auto __result = func;													\
	if (__result != S_OK)														\
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
		void Show() const noexcept;

		BaghdadError(std::string msg);
		virtual ~BaghdadError() noexcept override = default;

	private:
		std::string _msg;
	};
}