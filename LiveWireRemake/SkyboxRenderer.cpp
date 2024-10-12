#include "SkyboxRenderer.h"

#include "Globals.h"

using namespace LiveWireRemake;

BaghdadCore::Texture2D& SkyboxRenderer::GetCubeMap() const noexcept
{
	return *_pCubemap;
}

void LiveWireRemake::SkyboxRenderer::SetCubeMap(BaghdadCore::Texture2D&& cubemap) noexcept
{
	_pCubemap = std::move(std::make_unique<BaghdadCore::Texture2D>(std::move(cubemap)));
}

BaghdadCore::Material& SkyboxRenderer::GetMaterial() const noexcept
{
	return *_pMaterial;
}

void SkyboxRenderer::SetMaterial(BaghdadCore::Material&& material) noexcept
{
	_pMaterial = std::move(std::make_unique<BaghdadCore::Material>(std::move(material)));
}

BaghdadCore::Mesh& SkyboxRenderer::GetMesh() const noexcept
{
	return *_pMesh;
}

SkyboxRenderer::SkyboxRenderer()
{
	auto& globals = Globals::GetInstance();

	_pCubemap = std::make_unique<BaghdadCore::Texture2D>(std::move(
		globals
		.GetRenderer()
		.GetTextureBuilder()
		.Clear()
		.FromFile("Resources\\Textures\\skybox\\left.bmp")
		.FromFile("Resources\\Textures\\skybox\\right.bmp")
		.FromFile("Resources\\Textures\\skybox\\up.bmp")
		.FromFile("Resources\\Textures\\skybox\\down.bmp")
		.FromFile("Resources\\Textures\\skybox\\front.bmp")
		.FromFile("Resources\\Textures\\skybox\\back.bmp")
		.ViewFlag(BaghdadCore::Resource::View::Type::SRV)
		.CubeMap()
		.Build()));

	_pMaterial = std::make_unique<BaghdadCore::Material>(std::move(
		globals
		.GetRenderer()
		.GetMaterialBuilder()
		.Clear()
		.VS("skybox.vert")
		.PS("skybox.pix")
		.Build()));

	_pMesh = std::make_unique<BaghdadCore::Mesh>(std::move(
		globals
		.GetRenderer()
		.GetMeshLoader()
		.PrimitiveCube()
		.Load()));

	_pMaterial->SetPSTexture("skybox", *_pCubemap);
	_pMaterial->ToggleDepth(false);
	_pMaterial->SetCullMode(D3D11_CULL_FRONT);
}
