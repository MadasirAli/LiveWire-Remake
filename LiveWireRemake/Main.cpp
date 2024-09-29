#include <memory>

#include "App.h"
#include "BaghdadCore/BaghdadError.h"

int main()
{
    auto pApp = std::make_unique<LiveWireRemake::App>();
    int returnCode = 0;
    try
    {
        returnCode = pApp->Run();
    }
    catch (const BaghdadCore::BaghdadError& e)
    {
        e.Show();
    }
    catch (const std::exception& e)
    {
        BaghdadCore::BaghdadError error(e.what());
        error.Show();
    }

    return returnCode;
}