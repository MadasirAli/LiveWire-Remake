#pragma once

#include "IComponent.h"
#include "Transform.h"

namespace LiveWireRemake
{
	class CameraController final : public IComponent
	{
	public:
		void OnUpdate(std::weak_ptr<Entity>& pEntity) override;

		~CameraController() noexcept override = default;

	public:
		std::weak_ptr<Transform> pFollow;
		float followSpeed = 10;
		float lookSpeed = 10;
		float distance = 15;
		float sideLook = 0;
		float height = 5;
	};
}

