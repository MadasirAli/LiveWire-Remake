#include "Globals.h"

using namespace LiveWireRemake;

BaghdadCore::Renderer& Globals::GetRenderer() const noexcept
{
    return *_pRenderer;
}

WorldManager& Globals::GetWorldManager() const noexcept
{
    return *_pWorldManager;
}

InputManager& Globals::GetInputManager() const noexcept
{
    return *_pInputManager;
}

float Globals::GetDeltaTime() const noexcept
{
    return _deltaTime;
}

BaghdadCore::Logger& Globals::GetLogger() const noexcept
{
    return *_pLogger;
}

Globals& LiveWireRemake::Globals::GetInstance() noexcept
{
    static Globals globals{};
    return globals;
}
