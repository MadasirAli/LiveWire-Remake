#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>
#include <functional>

#include "Entity.h"
#include "BaghdadCore\BaghdadError.h"

namespace LiveWireRemake
{
	class World final
	{
		friend class WorldManager;

	public:
		template<typename T>
		std::vector<std::weak_ptr<T>> GetComponentsOfType() noexcept
		{
			std::vector<std::weak_ptr<T>> components{};

			for (auto& pair : _pEntities)
			{
				std::weak_ptr<T> ptr{};

				// id: (entity, bool dead)
				if (pair.second.first->TryGetComponent(ptr) == false)
					continue;

				components.emplace_back(std::move(ptr));
			}

			return components;
		}
		std::vector<std::weak_ptr<Entity>> FindWithName(const std::string& name) noexcept;
		std::vector<std::weak_ptr<Entity>> FindWithTag(const std::string& tag) noexcept;
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

