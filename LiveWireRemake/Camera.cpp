#include "Camera.h"

#include "BaghdadCore/BufferBuilder.h"
#include "Globals.h"
#include "CameraCBuffer.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Light.h"

using namespace LiveWireRemake;

void Camera::OnPreRender(std::weak_ptr<Entity>& pEntity)
{
	using namespace DirectX;

	auto& transform = pEntity.lock()->GetTransform();

	auto quaternion = XMQuaternionRotationRollPitchYawFromVector(transform.rotation);

	auto forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), quaternion);
	auto up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), quaternion);

	auto viewMatrix = XMMatrixLookToLH(transform.position, forward, up);
	viewMatrix = XMMatrixTranspose(viewMatrix);

	auto projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), Globals::GetInstance().GetScreenAspectRatio(), nearPlane, farPlane);
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

	// gathering lights
	std::vector<std::weak_ptr<Light>> pLights{};
	worldManager.GetActiveWorld().ForEach([&renderer, &pLights](std::weak_ptr<Entity> pEntity)
		{
			// getting mesh renderer
			std::weak_ptr<Light> pLight{};
			bool result = pEntity.lock()->TryGetComponent<Light>(pLight);

			if (false == result)
				return;

			pLights.emplace_back(std::move(pLight));
		});

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
}
