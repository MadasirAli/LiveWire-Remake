#include "Entity.h"

using namespace LiveWireRemake;

void Entity::ClearPendings(std::weak_ptr<Entity>& pEntity)
{
	// clearing dead components
	for (auto pBegin = _pComponents.begin(); pBegin != _pComponents.end();)
	{
		if (pBegin->second == false)
		{
			++pBegin;
			continue;
		}

		// raising event
		pBegin->first->OnDestroy(pEntity);

		// remove component
		pBegin = _pComponents.erase(pBegin);
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

void Entity::PreRender(std::weak_ptr<Entity>& pEntity)
{
	for (const auto& pComponent : _pComponents)
	{
		pComponent.first->OnPreRender(pEntity);
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

Transform& Entity::GetTransform() const noexcept
{
	return *_pTransform.lock();
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
