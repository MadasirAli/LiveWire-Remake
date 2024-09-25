#pragma once

#include <string>

namespace BaghdadCore
{
	class Logger final
	{
	public:
		void Write(const std::string& message) const noexcept;
		void WriteLine(const std::string& message) const noexcept;
		void LogError(const std::string& message) const noexcept;
	};
}