#pragma once

#include <vector>
#include <utility>
#include <assert.h>

#include <rpc.h>

#include "IComponent.h"

namespace LiveWireRemake
{
	class Entity final
	{
		friend class World;

	private:
		void ClearPendings(std::weak_ptr<Entity>& pEntity);
		void Update(std::weak_ptr<Entity>& pEntity);
		void Render(std::weak_ptr<Entity>& pEntity);

		void RaiseActiveEvents(std::weak_ptr<Entity>& pEntity);
		void RaiseDestroyEvent(std::weak_ptr<Entity>& pEntity);
	public:
		bool SetActive(bool value);

		template<typename T>
		bool TryGetComponent(std::weak_ptr<T>& pTarget)
		{
			for (const auto& pComponent : _pComponents)
			{
				T* pTarget = dynamic_cast<T*>(pComponent.first.get());
				if (pTarget == nullptr)
					continue;

				auto ptr = std::static_pointer_cast<T, IComponent>(std::shared_ptr<IComponent>(pComponent.first));
				pTarget = ptr;

				return true;
			}

			return false;
		}

		template<typename T>
		std::weak_ptr<T> GetComponent()
		{
			std::weak_ptr<T> ptr{};
			auto result = TryGetComponent<T>(ptr);

			assert(result);

			return ptr;
		}

		template<typename T>
		void RemoveComponent()
		{
			for (const auto& pComponent : _pComponents)
			{
				T* pTarget = dynamic_cast<T*>(pComponent.first.get());
				if (pTarget == nullptr)
					continue;

				pComponent.second = true;

				break;
			}
		}

		template<typename T>
		void AddComponent()
		{
			_toAdd_pComponents.emplace_back(std::static_pointer_cast<IComponent, T>(std::make_shared<T>()));
		}

	private:
		Entity(UUID id);

	private:
		std::vector<std::pair<std::shared_ptr<IComponent>, bool>> _pComponents;
		std::vector<std::shared_ptr<IComponent>> _toAdd_pComponents;

		bool _lastActiveState = false;
		bool _active = false;

		UUID _id;
	};
}