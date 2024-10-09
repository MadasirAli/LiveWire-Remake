#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "World.h"

namespace LiveWireRemake
{
	class WorldManager final
	{
	public:
		void Update();
		World& CreateWorld(const std::string& name) noexcept;
		World& GetWorld(const std::string& name) noexcept;

		void SetActiveWorld(const std::string& name) noexcept;
		World& GetActiveWorld() noexcept;

	private:
		std::unordered_map<std::string, std::unique_ptr<World>> _worlds;
		std::string _activeWorld;
	};
}

