#include "Entity.h"

using namespace LiveWireRemake;

void Entity::ClearPendings(std::weak_ptr<Entity>& pEntity)
{
	// clearing dead components
	for (auto pBegin = _pComponents.begin(); pBegin != _pComponents.end(); ++pBegin)
	{
		if (pBegin->second == false)
			continue;

		// raising event
		pBegin->first->OnDestroy(pEntity);

		// remove component
		_pComponents.erase(pBegin);
		// reiterating
		pBegin = _pComponents.begin();
		--pBegin;
	}

	// adding new components
	for (const auto& pComponent : _toAdd_pComponents)
	{
		_pComponents.emplace_back(pComponent, false);
	}
	_toAdd_pComponents.clear();
}

void Entity::Update(std::weak_ptr<Entity>& pEntity)
{
	for (const auto& pComponent : _pComponents)
	{
		pComponent.first->OnUpdate(pEntity);
	}
}

void Entity::Render(std::weak_ptr<Entity>& pEntity)
{
	for (const auto& pComponent : _pComponents)
	{
		pComponent.first->OnRender(pEntity);
	}
}

void Entity::RaiseActiveEvents(std::weak_ptr<Entity>& pEntity)
{
	if (_active)
	{
		for (const auto& pComponent : _pComponents)
		{
			pComponent.first->OnEnable(pEntity);
		}
	}
	else
	{
		for (const auto& pComponent : _pComponents)
		{
			pComponent.first->OnDisable(pEntity);
		}
	}
}

void Entity::RaiseDestroyEvent(std::weak_ptr<Entity>& pEntity)
{
	for (const auto& pComponent : _pComponents)
	{
		pComponent.first->OnDestroy(pEntity);
	}
}

bool Entity::SetActive(bool value)
{
	bool lastState = _lastActiveState;
	_lastActiveState = _active;

	_active = value;

	return lastState;
}

Entity::Entity(unsigned int id) :
	_id(id)
{}
