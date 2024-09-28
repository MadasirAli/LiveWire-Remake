#include <memory>

#include "App.h"

int main()
{
    auto pApp = std::make_unique<LiveWireRemake::App>();
    const auto returnCode = pApp->Run();

    return returnCode;
}