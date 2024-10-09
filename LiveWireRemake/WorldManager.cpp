#include "WorldManager.h"

using namespace LiveWireRemake;

void WorldManager::Update()
{
    _worlds.at(_activeWorld)->Update();
}

World& WorldManager::CreateWorld(const std::string& name) noexcept
{
    _worlds.insert(std::pair<std::string, std::unique_ptr<World>>(name, std::move(std::make_unique<World>())));
    return *_worlds.at(name);
}

World& WorldManager::GetWorld(const std::string& name) noexcept
{
    return *_worlds.at(name);
}

void WorldManager::SetActiveWorld(const std::string& name) noexcept
{
    _activeWorld = name;
}

World& WorldManager::GetActiveWorld() noexcept
{
    assert(_activeWorld != "");
    
    return *_worlds.at(_activeWorld);
}
