#pragma once

#include <memory>

namespace LiveWireRemake
{
	class Entity;
	class IComponent
	{

	public:
		virtual void OnStart(std::weak_ptr<Entity>& pEntity);
		virtual void OnUpdate(std::weak_ptr<Entity>& pEntity);
		virtual void OnRender(std::weak_ptr<Entity>& pEntity);
		virtual void OnDestroy(std::weak_ptr<Entity>& pEntity);

		virtual void OnEnable(std::weak_ptr<Entity>& pEntity);
		virtual void OnDisable(std::weak_ptr<Entity>& pEntity);

		virtual ~IComponent() noexcept = 0;

	protected:
		IComponent() = default;
	};
}

