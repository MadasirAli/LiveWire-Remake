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
		class Data final
		{
		public:
			BaghdadCore::Mesh& GetSphereMesh() noexcept;
			BaghdadCore::Mesh& GetCubeMesh() noexcept;
			BaghdadCore::Material& GetDefaultMaterial() noexcept;
			BaghdadCore::Material& GetGizmosMaterial() noexcept;

			Data(BaghdadCore::Renderer& renderer);

		private:
			std::unique_ptr<BaghdadCore::Mesh> _pSphereMesh;
			std::unique_ptr<BaghdadCore::Mesh> _pCubeMesh;
			std::unique_ptr<BaghdadCore::Material> _pDefaultMaterial;
			std::unique_ptr<BaghdadCore::Material> _pGizmosMaterial;
		};

	public:
		BaghdadCore::Renderer& GetRenderer() const noexcept;
		WorldManager& GetWorldManager() const noexcept;
		InputManager& GetInputManager() const noexcept;
		Data& GetData() const noexcept;

		float GetDeltaTime() const noexcept;
		float GetScreenWidth() const noexcept;
		float GetScreenHeight() const noexcept;
		float GetScreenAspectRatio() const noexcept;

		bool IsGizmosEnabled() const noexcept;
		bool IsRenderGraphicsEnabled() const noexcept;

		BaghdadCore::Logger& GetLogger() const noexcept;

		static Globals& GetInstance() noexcept;

	private:
		BaghdadCore::Renderer* _pRenderer;
		WorldManager* _pWorldManager;
		InputManager* _pInputManager;
		Data* _pData;

		float _deltaTime;
		float _screenWidth;
		float _screenHeight;

		bool _gizmosEnabled = false;
		bool _renderGraphics = true;

		BaghdadCore::Logger* _pLogger;
	};
}