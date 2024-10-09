#include "Camera.h"

#include "BaghdadCore/BufferBuilder.h"
#include "Globals.h"
#include "CameraCBuffer.h"
#include "Transform.h"
#include "MeshRenderer.h"

using namespace LiveWireRemake;

void Camera::OnPreRender(std::weak_ptr<Entity>& pEntity)
{
	using namespace DirectX;

	// updating camera c buffer
	CameraCBuffer data = {};
	data.ProjectionMatrix = XMMatrixPerspectiveFovLH(fov, Globals::GetInstance().GetScreenAspectRatio(), nearPlane, farPlane);

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

	// iterating all entities
	worldManager.GetActiveWorld().ForEach([this, &renderer](std::weak_ptr<Entity> pEntity)
		{
			// getting transform
			std::weak_ptr<Transform> pTransform{};
			bool result = pEntity.lock()->TryGetComponent<Transform>(pTransform);

			if (result == false)
				return;

			// getting mesh renderer
			std::weak_ptr<MeshRenderer> pMeshRenderer{};
			result = pEntity.lock()->TryGetComponent<MeshRenderer>(pMeshRenderer);
			
			if (result = false)
				return;

			// obtaining mesh and material and buffers
			auto& mesh = pMeshRenderer.lock()->GetMesh();
			auto& material = pMeshRenderer.lock()->GetMaterial();
			auto& transformCBuffer = pTransform.lock()->GetTransformCBuffer();

			// binding constant buffers
			material.SetVSCBuffer("TransformCBuffer", transformCBuffer);
			material.SetVSCBuffer("CameraCBuffer", *_pBuffer);

			// drawing
			renderer.DrawMesh(mesh, material);
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
		.Bind(D3D11_BIND_VERTEX_BUFFER)
		.InitialData((char*)&pData, sizeof(pData))
		.BuildCBuffer()
	));
}
