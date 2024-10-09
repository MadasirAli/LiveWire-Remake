#pragma once

#include <memory>

namespace LiveWireRemake
{
	class Entity;
	class IComponent
	{
		friend class Entity;

	public:
		virtual ~IComponent() noexcept = default;

	protected:
		virtual void OnStart(std::weak_ptr<Entity>& pEntity);
		virtual void OnUpdate(std::weak_ptr<Entity>& pEntity);
		virtual void OnPreRender(std::weak_ptr<Entity>& pEntity);
		virtual void OnRender(std::weak_ptr<Entity>& pEntity);
		virtual void OnDestroy(std::weak_ptr<Entity>& pEntity);

		virtual void OnEnable(std::weak_ptr<Entity>& pEntity);
		virtual void OnDisable(std::weak_ptr<Entity>& pEntity);

		IComponent() = default;
	};
}

