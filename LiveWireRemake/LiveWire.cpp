#include "LiveWire.h"

#include "BaghdadCore/imgui.h"
#include "Globals.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"

using namespace LiveWireRemake;

void LiveWire::Update()
{
	auto& globals = Globals::GetInstance();
	auto& renderer = globals.GetRenderer();
	const auto& input = globals.GetInputManager();
	auto& worldManager = globals.GetWorldManager();

	// --------------- START HERE ---------------- //

	static bool init = false;

	if (init == false)
	{
		init = true;

		// creating world
		auto& world = worldManager.CreateWorld("Test");
		worldManager.SetActiveWorld("Test");

		// creating entities
		auto pCameraEntity = world.CreateEntity();
		auto pTriangleEntity = world.CreateEntity();

		pCameraEntity.lock()->SetActive(true);
		pTriangleEntity.lock()->SetActive(true);

		// adding components
		pCameraEntity.lock()->AddComponent<Transform>();
		auto pCamera = pCameraEntity.lock()->AddComponent<Camera>();

		pTriangleEntity.lock()->AddComponent<Transform>();
		auto pMeshRenderer = pTriangleEntity.lock()->AddComponent<MeshRenderer>();
	}

	// -------------------------------------------- //

	worldManager.Update();
}