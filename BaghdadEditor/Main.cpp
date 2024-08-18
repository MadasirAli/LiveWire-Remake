#include "App.h"

#include <memory>

using namespace BaghdadEditor;

int main()
{
	auto pApp = std::make_unique<App>();
	auto exitCode = pApp->Run();

	return exitCode;
}