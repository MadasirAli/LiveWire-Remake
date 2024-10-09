#include "Globals.h"

using namespace LiveWireRemake;

BaghdadCore::Renderer& Globals::GetRenderer() noexcept
{
    return *_pRenderer;
}

WorldManager& Globals::GetWorldManager() noexcept
{
    return *_pWorldManager;
}

InputManager& Globals::GetInputManager() noexcept
{
    return *_pInputManager;
}

Globals& LiveWireRemake::Globals::GetInstance() noexcept
{
    static Globals globals{};
    return globals;
}
