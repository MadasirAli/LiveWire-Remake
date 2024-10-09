#include "World.h"

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
				GenerateId(), std::pair<std::shared_ptr<Entity>, bool>(pEntity, false)));
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
	for (auto pBegin = _pEntities.begin(); pBegin != _pEntities.end(); ++pBegin)
	{
		// id: pair(pEntity, bool dead)
		if (pBegin->second.second)
			continue;

		// raising event
		// id: pair(pEntity, bool dead)
		std::weak_ptr<Entity> ptr = pBegin->second.first;
		pBegin->second.first->RaiseActiveEvents(ptr);

		// id: pair(pEntity, bool dead)
		_unactive_pEntities.push_back(pBegin->second.first);
		_pEntities.erase(pBegin);

		// reiterating
		pBegin = _pEntities.begin();
		--pBegin;
	}

	// starting re active entities
	for (auto pBegin = _unactive_pEntities.begin(); pBegin != _unactive_pEntities.end(); ++pBegin)
	{
		if ((*pBegin)->_active == false)
			continue;

		// raising event
		std::weak_ptr<Entity> ptr = *pBegin;
		(*pBegin)->RaiseActiveEvents(ptr);

		_pEntities.insert(
			std::pair<unsigned int, std::pair<std::shared_ptr<Entity>, bool>>(
				(*pBegin)->_id, std::pair<std::shared_ptr<Entity>, bool>(*pBegin, false)));
		_unactive_pEntities.erase(pBegin);

		// reiterating
		pBegin = _unactive_pEntities.begin();
		--pBegin;
	}

	// updating
	for (auto& pEntity : _pEntities)
	{
		// id: pair(pEntity, bool dead)
		std::weak_ptr<Entity> ptr = pEntity.second.first;
		pEntity.second.first->Update(ptr);
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

std::weak_ptr<Entity> World::CreateEntity() noexcept
{
	_toAdd_pEntities.emplace_back(std::move(std::make_shared<Entity>(GenerateId())));

	return _toAdd_pEntities.back();
}

void World::RemoveEntity(std::weak_ptr<Entity>& pEntity) noexcept
{
	assert(pEntity.expired() == false);

	_pEntities.at(pEntity.lock()->_id).second = true;
}