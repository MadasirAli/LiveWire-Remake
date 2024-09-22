#include "Logger.h"

#include <stdio.h>
#include <iostream>

using namespace BaghdadCore;

void Logger::Write(const std::string& message) const noexcept
{
	std::cout << message;
}

void Logger::WriteLine(const std::string& message) const noexcept
{
	std::cout << message << std::endl;
}
