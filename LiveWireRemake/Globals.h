#pragma once

#include "BaghdadCore/Renderer.h"
#include "BaghdadCore/Logger.h"
#include "InputManager.h"
#include "WorldManager.h"

namespace LiveWireRemake
{
	class Globals final
	{
		friend class App;

	public:
		BaghdadCore::Renderer& GetRenderer() const noexcept;
		WorldManager& GetWorldManager() const noexcept;
		InputManager& GetInputManager() const noexcept;

		float GetDeltaTime() const noexcept;
		float GetScreenWidth() const noexcept;
		float GetScreenHeight() const noexcept;
		float GetScreenAspectRatio() const noexcept;

		BaghdadCore::Logger& GetLogger() const noexcept;

		static Globals& GetInstance() noexcept;

	private:
		BaghdadCore::Renderer* _pRenderer;
		WorldManager* _pWorldManager;
		InputManager* _pInputManager;

		float _deltaTime;
		float _screenWidth;
		float _screenHeight;
		BaghdadCore::Logger* _pLogger;
	};
}

