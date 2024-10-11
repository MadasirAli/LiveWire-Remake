#pragma once

#include "BaghdadCore/ConstantBuffer.h"

#include "IComponent.h"

namespace LiveWireRemake
{
	class Camera final : public IComponent
	{
	private:
		void OnPreRender(std::weak_ptr<Entity>& pEntity) override;
		void OnRender(std::weak_ptr<Entity>& pEntity) override;

	public:
		Camera();
		~Camera() noexcept override = default;

	public:
		float fov = 60;
		float nearPlane = 0.03f;
		float farPlane = 100000;

		float viewportOffsetX = 0;
		float viewportOffsetY = 0;
		float viewportWidth = 1;
		float viewportHeight = 1;

	private:
		std::unique_ptr<BaghdadCore::ConstantBuffer> _pBuffer;
	};
}

