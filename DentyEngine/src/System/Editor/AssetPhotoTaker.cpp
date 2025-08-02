#include "stdafx.h"

#include "AssetPhotoTaker.h"

#include "Component/Graphics/Renderer/SkinnedMeshRenderer.h"

#include "Graphics/Buffer/ColorBuffer.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Buffer/FrameBuffer.h"

#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/StaticMesh.h"

#include "Graphics/States/SamplerState.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Resource/Prefab.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"

namespace DentyEngine
{
	const Vector2 AssetPhotoTaker::BUFFER_SIZE = Vector2(256, 256.0f);

	AssetPhotoTaker::AssetPhotoTaker() :
		_camera(), _frameBuffer()
	{
	}

	void AssetPhotoTaker::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		// Create camera.
		{
			_camera = std::make_unique<PreviewCamera>();

			// Perspective
			Camera::Perspective perspective = { };
			{
				perspective.distance = 10.0f;
				perspective.fovAngleY = 45.0f;
				perspective.nearZ = 0.01f;
				perspective.farZ = 5000.0f;

				// BUFFER_SIZE.x / BUFFER_SIZE.y
				perspective.aspectRatio = BUFFER_SIZE.x / BUFFER_SIZE.y;
				perspective.up = Vector3::Up();
			}

			const auto eyePosition = Vector3(0, -5, -perspective.distance);

			_camera->CalculatePerspectiveProjection(perspective);
			_camera->CalculateView(_camera->transform.world, eyePosition, Vector3::Zero(), Vector3::Up());
			
			_camera->transform.world.SetOffsetVec3(eyePosition);

			// Create constants.
			{
				Camera::Constants constants = { };

				constants.cameraPosition = _camera->transform.world.GetOffset();
				constants.view = _camera->GetView();
				constants.projection = _camera->GetProjection();
				constants.viewProjection = constants.view * constants.projection;

				_camera->GetConstantBuffer()->UpdateSubresource(context, &constants);
				_camera->GetConstantBuffer()->BindAll(context, static_cast<UINT>(ConstantBuffer::ReservedBindSlot::PhotoCamera));
			}
		}

		// Create frame buffer.
		{
			using BlendType = BlendState::Type;

			_frameBuffer = std::make_shared<FrameBuffer>();
			_frameBuffer->Create(device, BUFFER_SIZE, BlendType::NoBlend, DXGI_FORMAT_R8G8B8A8_UNORM);
		}
	}

	Ref<ColorBuffer> AssetPhotoTaker::TakeMeshPhoto(Mesh* mesh)
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		{
			// Setup
			{
				Setup(context, mesh, Vector3(0, 1, 1));
			}

			// Rendering
			{
				Begin(context);

				mesh->OnUpdate(Matrix::Identity(), 0u, mesh->GetMainMaterial(), 0.0f);
				mesh->OnRender(context, mesh->GetMainMaterial(), mesh->GetMaterials());

				End(context);
			}

			// End rendering.
			{
				
			}
		}

		// Get color buffer.
		return _frameBuffer->GetColorBuffer();
	}

	Ref<ColorBuffer> AssetPhotoTaker::TakeMaterialPhoto(Material* material)
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
		const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

		const auto sphere = meshLibrary->Find<StaticMesh>(MeshLibrary::SPHERE_FILE_PATH);
		if (sphere.has_value())
		{
			//// Clone original
			//const auto cloneMaterial = std::make_shared<Material>();
			//{
		 //       material->Clone(cloneMaterial);

			//    cloneMaterial->shader = shaderLibrary->Find(ShaderLibrary::PHOTO_STATIC_MESH_SHADER_PATH).value();
			//}

			//// Change mesh material.
			//{
			//    sphere.value()->ChangeMaterial("DefaultMaterial", cloneMaterial);
			//}

			//// Setup
			//{
			//	Setup(context, sphere.value().get(), Vector3(0, 1, 1));
			//}

			//// Rendering
			//{
			//    Begin(context);

			//	sphere.value()->OnUpdate(Matrix::Identity(), 0u, cloneMaterial, 0.0f);
			//	sphere.value()->OnRender(context, sphere.value()->GetMainMaterial(), sphere.value()->GetMaterials());

			//	End(context);
			//}

			//// End rendering.
			//{
			//	const auto defaultMaterial = Material::CreateDefaultMaterial(false);
			//    sphere.value()->ChangeMaterial("DefaultMaterial", defaultMaterial);
			//}
		}

		return _frameBuffer->GetColorBuffer();
	}

	Ref<ColorBuffer> AssetPhotoTaker::TakePrefabPhoto([[maybe_unused]] Prefab* prefab)
	{
		//ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		//const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
		//const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

		////const auto meshFilePath = ObtainMeshFilePathFromComponents(*prefab->GetInfos().at(0));

		//// Get mesh.
		//std::optional<Ref<Mesh>> mesh = std::nullopt;
		//if (meshFilePath.has_value())
		//{
		//	mesh = meshLibrary->Find<Mesh>(meshFilePath.value());
		//}

		//// Change shader.
		//{
		//	if (mesh.has_value())
		//	{
		//		std::optional<Ref<Shader>> shader = std::nullopt;

		//		if (mesh.value()->GetMeshType() == std::string("StaticMesh"))
		//		{
		//			shader = shaderLibrary->Find(ShaderLibrary::PHOTO_STATIC_MESH_SHADER_PATH);
		//		}
		//		else if (mesh.value()->GetMeshType() == std::string("SkinnedMesh"))
		//		{
		//			shader = shaderLibrary->Find(ShaderLibrary::PHOTO_SKINNED_MESH_SHADER_PATH);
		//		}

		//		if (shader.has_value())
		//		{
		//			for (auto&& material : mesh.value()->GetMaterials())
		//			{
		//				material->shader = shader.value();
		//			}
		//		}
		//	}
		//}

		//// Setup camera.
		////Setup(context, mesh.get());

		//{
		//	Begin(context);

		//	//mesh->OnUpdate(Matrix::Identity(), 0u, mesh->GetMainMaterial(), 0.0f);
		//	//mesh->OnRender(context, mesh->GetMainMaterial(), mesh->GetMaterials(), nullptr);

		//	End(context);
		//}

		//// Restore shaders to their original state.
		//{
		//	const auto meshShader = shaderLibrary->Find(ShaderLibrary::STATIC_MESH_SHADER_PATH);
		//	if (meshShader.has_value())
		//	{
		//		/*for (auto&& material : mesh->GetMaterials())
		//		{
		//			material->shader = meshShader.value();
		//		}*/
		//	}
		//}

		return _frameBuffer->GetColorBuffer();
	}

	void AssetPhotoTaker::Begin(ID3D11DeviceContext* context)
	{
		_frameBuffer->Clear(context, Color::GRAY);
		_frameBuffer->Activate(context);
	}

	void AssetPhotoTaker::End(ID3D11DeviceContext* context)
	{
		_frameBuffer->DeActivate(context);
	}

	void AssetPhotoTaker::Setup(ID3D11DeviceContext* context, [[maybe_unused]] const Mesh* mesh, const Vector3& direction)
	{
		//const Vector3 sphereCenter = mesh->GetBoundingSphere().GetCenter();
		const Vector3 sphereCenter = {};
		//const float sphereRadius = mesh->GetBoundingSphere().GetRadius() + 0.25f;
		const float sphereRadius = 0.25f;

		Matrix view = Matrix::Identity();
		{
			view = Camera::CalculateViewMatrixTakingSphereIn(
				sphereCenter, sphereRadius,
				45.0f, BUFFER_SIZE.x / BUFFER_SIZE.y,
				direction, Vector3::Up()
			);
		}

		// Update constants and bind.
		{
			Camera::Constants constants = { };
			constants.view = view;
			constants.projection = _camera->GetProjection();
			constants.viewProjection = constants.view * constants.projection;
			constants.cameraPosition = view.Inverse().GetOffset();

			_camera->GetConstantBuffer()->UpdateSubresource(context, &constants);
			_camera->GetConstantBuffer()->BindAll(context, static_cast<UINT>(ConstantBuffer::ReservedBindSlot::PhotoCamera));
		}

		// Bind sampler states.
		{
		    ID3D11SamplerState* samplerStates[] =
			{
				SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::PointWrap),
				SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::LinearWrap),
				SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::AnisotropicWrap)
			};

			context->PSSetSamplers(0, _countof(samplerStates), samplerStates);
		}
	}

	/*std::optional<std::string> AssetPhotoTaker::ObtainMeshFilePathFromComponents(const Prefab::Info& info) const
	{
		std::string meshFilePath = {};

		for (const auto& component : info.components)
		{
			const String componentName = component->GetName();

			if (componentName.Contains("Renderer"))
			{
				if (const auto rendererComponent = std::dynamic_pointer_cast<MeshRenderer>(component))
				{
					meshFilePath = rendererComponent->GetFilePathToSerialized().string();

					return meshFilePath;
				}

				const auto rendererComponent = std::static_pointer_cast<SkinnedMeshRenderer>(component);
				meshFilePath = rendererComponent->GetFilePathToSerialized().GetAsString();

				return meshFilePath;
			}
		}

		return std::nullopt;
	}*/

}
