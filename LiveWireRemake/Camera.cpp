#include "Camera.h"

#include "BaghdadCore/BufferBuilder.h"
#include "Globals.h"
#include "CameraCBuffer.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Light.h"
#include "SkyboxRenderer.h"

using namespace LiveWireRemake;

void Camera::OnPreRender(std::weak_ptr<Entity>& pEntity)
{
	using namespace DirectX;

	auto& globals = Globals::GetInstance();
	auto& transform = pEntity.lock()->GetTransform();

	const auto width = globals.GetScreenWidth() * viewportWidth;
	const auto height = globals.GetScreenHeight() * viewportHeight;

	// updating transform c buffer
	auto quaternion = transform.Quaternion();

	auto forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), quaternion);
	auto up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), quaternion);

	auto viewMatrix = XMMatrixLookToLH(XMVectorSet(transform.position.x , transform.position.y, transform.position.z, 1), forward, up);
	viewMatrix = XMMatrixTranspose(viewMatrix);

	auto projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / height, nearPlane, farPlane);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// updating camera c buffer
	CameraCBuffer data = {};
	data.ViewMatrix = viewMatrix;
	data.ProjectionMatrix = projectionMatrix;

	const auto ptr = (CameraCBuffer*)_pBuffer->Map(D3D11_MAP_WRITE_DISCARD);
	ptr[0] = data;

	_pBuffer->UnMap();
}

void Camera::OnRender(std::weak_ptr<Entity>& pEntity)
{
	// rendering all entities
	auto& globals = Globals::GetInstance();
	auto& worldManager = globals.GetWorldManager();
	auto& renderer = globals.GetRenderer();

	// setting viewport
	{
		const auto width = globals.GetScreenWidth();
		const auto height = globals.GetScreenHeight();

		D3D11_VIEWPORT viewport = { 0 };
		viewport.TopLeftX = width * viewportOffsetX;
		viewport.TopLeftY = height * viewportOffsetY;
		viewport.Width = width * viewportWidth;
		viewport.Height = height * viewportHeight;
		viewport.MaxDepth = 1;
		renderer.SetViewport(viewport);
	}

	// gathering items
	std::vector<std::weak_ptr<Light>> pLights{};
	std::vector<std::weak_ptr<SkyboxRenderer>> pSkyboxes{};
	worldManager.GetActiveWorld().ForEach([&renderer, &pLights, &pSkyboxes](std::weak_ptr<Entity> pEntity)
		{
			// getting light if
			std::weak_ptr<Light> pLight{};
			bool result = pEntity.lock()->TryGetComponent<Light>(pLight);
			if (result)
			{
				pLights.emplace_back(std::move(pLight));
			}

			// getting skybox if
			std::weak_ptr<SkyboxRenderer> pSkybox{};
			result = pEntity.lock()->TryGetComponent<SkyboxRenderer>(pSkybox);
			if (result)
			{
				pSkyboxes.emplace_back(std::move(pSkybox));
			}
		});

	// rendering skybox
	for(const auto& pSkybox : pSkyboxes)
	{
		// obtaining mesh and materials
		auto& mesh = pSkybox.lock()->GetMesh();
		auto& material = pSkybox.lock()->GetMaterial();

		// setting c buffers
		material.SetVSCBuffer("CameraCBuffer", *_pBuffer);

		// drawing
		renderer.DrawMesh(mesh, material);
	}

	// iterating all entities to render
	worldManager.GetActiveWorld().ForEach([this, &renderer, &pLights](std::weak_ptr<Entity> pEntity)
		{
			// getting mesh renderer
			std::weak_ptr<MeshRenderer> pMeshRenderer{};
			bool result = pEntity.lock()->TryGetComponent<MeshRenderer>(pMeshRenderer);
			
			if (false == result)
				return;

			const bool useLights = pMeshRenderer.lock()->useLights;
			const auto passCount = (!useLights || pLights.size() == 0) ? 1u : pLights.size();
			// for each light
			for (auto i = 0; i < passCount; ++i)
			{
				// obtaining mesh and material and buffers
				auto& mesh = pMeshRenderer.lock()->GetMesh();
				auto& material = pMeshRenderer.lock()->GetMaterial();
				auto& transformCBuffer = pMeshRenderer.lock()->GetTransformCBuffer();

				// binding constant buffers
				material.SetVSCBuffer("TransformCBuffer", transformCBuffer);
				material.SetVSCBuffer("CameraCBuffer", *_pBuffer);

				if (useLights && pLights.size() > 0)
				{
					if (pMeshRenderer.lock()->goroudShading)
						material.SetVSCBuffer("LightCBuffer", pLights[i].lock()->GetLightCBuffer());
					else
						material.SetPSCBuffer("LightCBuffer", pLights[i].lock()->GetLightCBuffer());
				}

				// drawing
				renderer.DrawMesh(mesh, material);
			}
		});
}

Camera::Camera()
{
	const CameraCBuffer pData = {};

	_pBuffer = std::make_unique<BaghdadCore::ConstantBuffer>(std::move(
		Globals::GetInstance()
		.GetRenderer()
		.GetBufferBuilder()
		.Clear()
		.Write()
		.InitialData((char*)&pData, sizeof(pData))
		.BuildCBuffer()
	));

	auto& globals = Globals::GetInstance();
}