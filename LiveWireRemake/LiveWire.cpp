#include "LiveWire.h"

#include "BaghdadCore/imgui.h"
#include "Globals.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Movement.h"
#include "Light.h"

using namespace LiveWireRemake;

void LiveWire::Update()
{
	auto& globals = Globals::GetInstance();
	auto& renderer = globals.GetRenderer();
	const auto& input = globals.GetInputManager();
	auto& worldManager = globals.GetWorldManager();

	worldManager.Update();
}

void LiveWire::Start()
{
	auto& globals = Globals::GetInstance();
	auto& renderer = globals.GetRenderer();
	const auto& input = globals.GetInputManager();
	auto& worldManager = globals.GetWorldManager();

	// creating world
	auto& world = worldManager.CreateWorld("Test");
	worldManager.SetActiveWorld("Test");

	// creating entities
	auto pCameraEntity = world.CreateEntity();
	auto pTriangleEntity = world.CreateEntity();
	auto pLightEntity = world.CreateEntity();

	// adding components
	auto pCamera = pCameraEntity.lock()->AddComponent<Camera>();
	auto pMeshRenderer = pTriangleEntity.lock()->AddComponent<MeshRenderer>();
	pLightEntity.lock()->AddComponent<Light>();

	pCameraEntity.lock()->AddComponent<Movement>();
}