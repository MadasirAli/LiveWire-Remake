#include "LiveWire.h"

#include "BaghdadCore/imgui.h"
#include "Globals.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Movement.h"
#include "Light.h"
#include "SkyboxRenderer.h"
#include "SphereCollider.h"
#include "CollisionEngine.h"
#include "CameraController.h"

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
	auto pTriangleEntity2 = world.CreateEntity();
	auto pLightEntity = world.CreateEntity();
	auto pSkyboxEntity = world.CreateEntity();
	auto pCollisionEngineEntity = world.CreateEntity();

	// adding components
	auto pCamera = pCameraEntity.lock()->AddComponent<Camera>();
	auto pMeshRenderer = pTriangleEntity.lock()->AddComponent<MeshRenderer>();
	pTriangleEntity2.lock()->AddComponent<MeshRenderer>();
	pLightEntity.lock()->AddComponent<Light>();
	pSkyboxEntity.lock()->AddComponent<SkyboxRenderer>();

	pCameraEntity.lock()->GetTransform().position = DirectX::XMFLOAT3(0, 0, -20);
	auto pCameraController = pCameraEntity.lock()->AddComponent<CameraController>();

	pTriangleEntity2.lock()->GetTransform().position = DirectX::XMFLOAT3(5, 0, 0);
	pTriangleEntity.lock()->AddComponent<SphereCollider>();
	pTriangleEntity2.lock()->AddComponent<SphereCollider>();
	pCollisionEngineEntity.lock()->AddComponent<CollisionEngine>();

	pTriangleEntity.lock()->AddComponent<Movement>();

	pCameraController.lock()->pFollow = pTriangleEntity.lock()->GetComponent<Transform>();
}