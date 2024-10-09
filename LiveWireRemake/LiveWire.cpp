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

	// creating mesh and material
	auto mesh = renderer.GetMeshLoader()
		.Clear()
		.PrimitiveTriangle()
		.Load();
	auto material = renderer.GetMaterialBuilder()
		.VS("Raw.vert")
		.PS("Raw.pix")
		.Build();

	pMeshRenderer.lock()->SetMaterial(std::move(material));
	pMeshRenderer.lock()->SetMesh(std::move(mesh));

	// -------------------------------------------- //

	worldManager.Update();
}
