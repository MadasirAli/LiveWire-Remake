#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>

#include <rpc.h>
#include <rpcdce.h>

#include "Entity.h"

namespace LiveWireRemake
{
	class World final
	{
		friend class WorldManager;

	public:
		std::weak_ptr<Entity> CreateEntity() noexcept;
		void RemoveEntity(std::weak_ptr<Entity>& pEntity) noexcept;

	private:
		void Update();

		World() = default;

	private:
		std::unordered_map<UUID, std::pair<std::shared_ptr<Entity>, bool>> _pEntities;

		std::vector<std::shared_ptr<Entity>> _toAdd_pEntities;
		std::vector<std::shared_ptr<Entity>> _unactive_pEntities;
	};
}

