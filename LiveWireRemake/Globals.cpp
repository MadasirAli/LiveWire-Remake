#include "Globals.h"

using namespace LiveWireRemake;

BaghdadCore::Renderer& Globals::GetRenderer() const noexcept
{
    return *_pRenderer;
}

WorldManager& Globals::GetWorldManager() const noexcept
{
    return *_pWorldManager;
}

InputManager& Globals::GetInputManager() const noexcept
{
    return *_pInputManager;
}

Globals::Data& Globals::GetData() const noexcept
{
	return *_pData;
}

float Globals::GetDeltaTime() const noexcept
{
    return _deltaTime;
}

float LiveWireRemake::Globals::GetScreenWidth() const noexcept
{
    return _screenWidth;
}

float LiveWireRemake::Globals::GetScreenHeight() const noexcept
{
    return _screenHeight;
}

float Globals::GetScreenAspectRatio() const noexcept
{
    return _screenWidth / _screenHeight;
}

bool Globals::IsGizmosEnabled() const noexcept
{
	return _gizmosEnabled;
}

bool Globals::IsRenderGraphicsEnabled() const noexcept
{
	return _renderGraphics;
}

BaghdadCore::Logger& Globals::GetLogger() const noexcept
{
    return *_pLogger;
}

Globals& LiveWireRemake::Globals::GetInstance() noexcept
{
    static Globals globals{};
    return globals;

}

BaghdadCore::Mesh& Globals::Data::GetSphereMesh() noexcept
{
	return *_pSphereMesh;
}

BaghdadCore::Mesh& Globals::Data::GetCubeMesh() noexcept
{
	return *_pCubeMesh;
}

BaghdadCore::Material& Globals::Data::GetDefaultMaterial() noexcept
{
	return *_pDefaultMaterial;
}

BaghdadCore::Material& Globals::Data::GetGizmosMaterial() noexcept
{
	return *_pGizmosMaterial;
}

Globals::Data::Data(BaghdadCore::Renderer& renderer)
{
	// creating mesh and material
	auto cubeMesh = renderer.GetMeshLoader()
		.Clear()
		.FromFile("Resources\\Models\\cube.obj")
		.Load();
	auto sphereMesh = renderer.GetMeshLoader()
		.Clear()
		.FromFile("Resources\\Models\\sphere.obj")
		.Load();
	auto material = renderer.GetMaterialBuilder()
		.Clear()
		.VS("Basic_goroud.vert")
		.PS("Basic_goroud.pix")
		.Build();

	auto gizmosMaterial = renderer.GetMaterialBuilder()
		.Clear()
		.VS("gizmo.vert")
		.PS("gizmo.pix")
		.Build();
	gizmosMaterial.ToggleWireframe(true);

	// binding default texture
	auto defaultTexture = renderer.GetTextureBuilder()
		.Clear()
		.FromFile("Resources\\Textures\\landscape.bmp")
		.ViewFlag(BaghdadCore::Resource::View::Type::SRV)
		.Build();

	material.SetPSTexture("albedo", defaultTexture);

	_pSphereMesh = std::move(std::make_unique<BaghdadCore::Mesh>(std::move(sphereMesh)));
	_pCubeMesh = std::move(std::make_unique<BaghdadCore::Mesh>(std::move(cubeMesh)));;
	_pDefaultMaterial = std::move(std::make_unique<BaghdadCore::Material>(std::move(material)));
	_pGizmosMaterial = std::move(std::make_unique<BaghdadCore::Material>(std::move(gizmosMaterial)));
}