#include "IComponent.h"

using namespace LiveWireRemake;

void IComponent::OnStart(std::weak_ptr<Entity>& pEntity)
{}

void IComponent::OnUpdate(std::weak_ptr<Entity>& pEntity)
{}

void IComponent::OnRender(std::weak_ptr<Entity>& pEntity)
{}

void IComponent::OnDestroy(std::weak_ptr<Entity>& pEntity)
{}

void IComponent::OnEnable(std::weak_ptr<Entity>& pEntity)
{}

void ::IComponent::OnDisable(std::weak_ptr<Entity>& pEntity)
{}