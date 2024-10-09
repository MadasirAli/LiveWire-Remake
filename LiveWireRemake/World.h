#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>
#include <functional>

#include "Entity.h"

namespace LiveWireRemake
{
	class World final
	{
		friend class WorldManager;

	public:
		void ForEach(std::function<void(std::weak_ptr<Entity>)> action);

		std::weak_ptr<Entity> CreateEntity() noexcept;
		void RemoveEntity(std::weak_ptr<Entity>& pEntity) noexcept;

	private:
		void Update();

		static unsigned int GenerateId() noexcept;

	private:
		std::unordered_map<unsigned int, std::pair<std::shared_ptr<Entity>, bool>> _pEntities;

		std::vector<std::shared_ptr<Entity>> _toAdd_pEntities;
		std::vector<std::shared_ptr<Entity>> _unactive_pEntities;
	};
}

