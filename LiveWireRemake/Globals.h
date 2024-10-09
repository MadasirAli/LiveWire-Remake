#pragma once

#include "BaghdadCore/Renderer.h"
#include "InputManager.h"
#include "WorldManager.h"

namespace LiveWireRemake
{
	class Globals final
	{
		friend class App;

	public:
		BaghdadCore::Renderer& GetRenderer() noexcept;
		WorldManager& GetWorldManager() noexcept;
		InputManager& GetInputManager() noexcept;

		static Globals& GetInstance() noexcept;

	private:
		BaghdadCore::Renderer* _pRenderer;
		WorldManager* _pWorldManager;
		InputManager* _pInputManager;
	};
}

