#include "World.h"

#include "Transform.h"

using namespace LiveWireRemake;

void LiveWireRemake::World::Update()
{
	// clearing dead entities
	for (auto pBegin = _pEntities.begin(); pBegin != _pEntities.end(); ++pBegin)
	{
		// id: pair(pEntity, bool dead)
		if (pBegin->second.second == false)
			continue;

		// removing
		_pEntities.erase(pBegin->first);
		// reiterating
		pBegin = _pEntities.begin();
		--pBegin;
	}

	// adding new entities
	for (const auto& pEntity : _toAdd_pEntities)
	{
		_pEntities.insert(
			std::pair<unsigned int, std::pair<std::shared_ptr<Entity>, bool>>(
				pEntity->_id, std::pair<std::shared_ptr<Entity>, bool>(pEntity, false)));
	}
	_toAdd_pEntities.clear();

	// clearing pendings
	for (auto& pEntity : _pEntities)
	{
		// id: pair(pEntity, bool dead)
		std::weak_ptr<Entity> ptr = pEntity.second.first;
		pEntity.second.first->ClearPendings(ptr);
	}

	// removing unactive entities
	std::vector<unsigned int> toRemoveEntities{};
	for (auto pBegin = _pEntities.begin(); pBegin != _pEntities.end(); ++pBegin)
	{
		// id: pair(pEntity, bool dead)
		if (pBegin->second.first->_active)
			continue;

		// raising event
		// id: pair(pEntity, bool dead)
		std::weak_ptr<Entity> ptr = pBegin->second.first;
		pBegin->second.first->RaiseActiveEvents(ptr);

		// id: pair(pEntity, bool dead)
		_unactive_pEntities.push_back(pBegin->second.first);
		toRemoveEntities.push_back(pBegin->first);
	}
	// removing marked ones
	for (const auto id : toRemoveEntities)
	{
		_pEntities.erase(id);
	}
	toRemoveEntities.clear();

	// starting re active entities
	for (auto pBegin = _unactive_pEntities.begin(); pBegin != _unactive_pEntities.end();)
	{
		if ((*pBegin)->_active == false)
		{
			++pBegin;
			continue;
		}

		// raising event
		std::weak_ptr<Entity> ptr = *pBegin;
		(*pBegin)->RaiseActiveEvents(ptr);

		_pEntities.insert(
			std::pair<unsigned int, std::pair<std::shared_ptr<Entity>, bool>>(
				(*pBegin)->_id, std::pair<std::shared_ptr<Entity>, bool>(*pBegin, false)));
		pBegin = _unactive_pEntities.erase(pBegin);
	}

	// pre updating
	for (auto& pEntity : _pEntities)
	{
		// id: pair(pEntity, bool dead)
		std::weak_ptr<Entity> ptr = pEntity.second.first;
		pEntity.second.first->PreUpdate(ptr);
	}

	// updating
	for (auto& pEntity : _pEntities)
	{
		// id: pair(pEntity, bool dead)
		std::weak_ptr<Entity> ptr = pEntity.second.first;
		pEntity.second.first->Update(ptr);
	}

	// post updating
		// updating
	for (auto& pEntity : _pEntities)
	{
		// id: pair(pEntity, bool dead)
		std::weak_ptr<Entity> ptr = pEntity.second.first;
		pEntity.second.first->LateUpdate(ptr);
	}

	// pre render
	for (auto& pEntity : _pEntities)
	{
		// id: pair(pEntity, bool dead)
		std::weak_ptr<Entity> ptr = pEntity.second.first;
		pEntity.second.first->PreRender(ptr);
	}

	// rendering
	for (auto& pEntity : _pEntities)
	{
		// id: pair(pEntity, bool dead)
		std::weak_ptr<Entity> ptr = pEntity.second.first;
		pEntity.second.first->Render(ptr);
	}
}

unsigned int World::GenerateId() noexcept
{
	static unsigned int lastAssignedId = 0u;
	++lastAssignedId;
	return lastAssignedId;
}

std::vector<std::weak_ptr<Entity>> World::FindWithName(const std::string& name) noexcept
{
	std::vector<std::weak_ptr<Entity>> entities{};

	for (auto& pair : _pEntities)
	{
		// id: (entity, bool dead)
		if (pair.second.first->name.compare(name) != 0)
			continue;

		entities.emplace_back(std::move(std::weak_ptr<Entity>(pair.second.first)));
	}

	return entities;
}

std::vector<std::weak_ptr<Entity>> World::FindWithTag(const std::string& tag) noexcept
{
	std::vector<std::weak_ptr<Entity>> entities{};

	for (auto& pair : _pEntities)
	{
		// id: (entity, bool dead)
		if (pair.second.first->tag.compare(tag) != 0)
			continue;

		entities.emplace_back(std::move(std::weak_ptr<Entity>(pair.second.first)));
	}

	return entities;
}

void World::ForEach(std::function<void(std::weak_ptr<Entity>)> action)
{
	for (auto& pair : _pEntities)
	{
		// id: (entity, bool dead)
		action(std::weak_ptr<Entity>(pair.second.first));
	}
}

std::weak_ptr<Entity> World::CreateEntity() noexcept
{
	_toAdd_pEntities.emplace_back(std::move(std::make_shared<Entity>(GenerateId())));

	auto ptr = _toAdd_pEntities.back();
	ptr->_pSelf = ptr;

	ptr->_pTransform = ptr->AddComponent<Transform>();

	return _toAdd_pEntities.back();
}

void World::RemoveEntity(std::weak_ptr<Entity>& pEntity) noexcept
{
	assert(pEntity.expired() == false);

	_pEntities.at(pEntity.lock()->_id).second = true;
}