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
	auto pCameraEntity2 = world.CreateEntity();
	auto pTriangleEntity = world.CreateEntity();
	auto pLightEntity = world.CreateEntity();

	// adding components
	auto pCamera = pCameraEntity.lock()->AddComponent<Camera>();
	auto pCamera2 = pCameraEntity2.lock()->AddComponent<Camera>();
	auto pMeshRenderer = pTriangleEntity.lock()->AddComponent<MeshRenderer>();
	pLightEntity.lock()->AddComponent<Light>();

	pCameraEntity.lock()->GetTransform().position = DirectX::XMFLOAT3(0, 0, -20);

	pCameraEntity.lock()->AddComponent<Movement>();

	pCamera.lock()->viewportWidth = 0.5f;
	pCamera2.lock()->viewportWidth = 0.5f;
	pCamera2.lock()->viewportOffsetX = 0.5f;
}