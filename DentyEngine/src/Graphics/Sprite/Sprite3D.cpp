#include "stdafx.h"

#include "Sprite3D.h"

#include "Graphics/Buffer/ConstantBuffer.h"

#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/Texture2D.h"

#include "Graphics/States/RasterizerState.h"

#include "System/Graphics/GraphicsManager.h"

#include "Editor/Graphics/Rendering/EditorCamera.h"
#include "Editor/Parts/EditorSceneView.h"
#include "Graphics/Buffer/DirectX/DXIndexBuffer.h"
#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"
#include "Graphics/Buffer/DirectX/IDirectXBuffer.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

namespace DentyEngine
{
	Sprite3DCommonResource::Sprite3DCommonResource() :
		vertices(), indices(), vertexBuffer(), indexBuffer(), shader()
	{
	}

	Sprite3DResource::Sprite3DResource() :
		texture()
	{
	}

	void Sprite3DResource::Create(Ref<Texture2D> paramTexture)
	{
		this->texture = paramTexture;
	}

	Sprite3DConstants::Sprite3DConstants() :
		id(), padding(), color(), world()
	{
	}

	Sprite3DCommonResource Sprite3D::_commonResource = { };

	Sprite3D::Sprite3D() :
		Object(), _resource(), _constants()
	{
		_name = "Sprite3D";
	}

	void Sprite3D::Create(Ref<Texture2D> texture)
	{
		_resource.Create(texture);
	}

	void Sprite3D::Update(const Matrix& world, const Color& color, EntityID id)
	{
		const Scope<EditorCamera>& editorCamera = EditorSceneView::GetEditorCamera();

		const Vector3 scale = world.ObtainScale();
		const Vector3 position = world.GetOffsetVec3();

		const Matrix S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		const Matrix R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		const Matrix T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

		Matrix inverseView = editorCamera->GetInverseView();
		inverseView.SetOffsetVec3(Vector3::Zero());

		_constants.world = S * R * inverseView * T;
		_constants.color = color;
		_constants.id = id;
	}

	void Sprite3D::Render()
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		// Get context caches.
		ComRef<ID3D11RasterizerState> cachedRasterizerState;
		context->RSGetState(cachedRasterizerState.GetAddressOf());

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_commonResource.vertexBuffer->Bind();
		_commonResource.indexBuffer->Bind();

		_commonResource.shader->BindVertexShader(context);
		_commonResource.shader->BindPixelShader(context);

		ID3D11RasterizerState* rasterizerState = RasterizerState::GetBuildInRasterizerStatesAt(RasterizerState::Type::FillNoCull);
		context->RSSetState(rasterizerState);

		_commonResource.shader->constantBuffer->UpdateSubresource(context, &_constants);
		_commonResource.shader->constantBuffer->BindAll(context, 0u);

		context->PSSetShaderResources(0u, 1u, _resource.texture->GetResource());

		context->DrawIndexed(_commonResource.indices.size(), 0u, 0);

		context->RSSetState(cachedRasterizerState.Get());
	}

	void Sprite3D::Setup()
	{
		// Setup context.
		{
			// Vertices
			{
				SpriteVertex& vertex0 = _commonResource.vertices.at(0);
				vertex0.position = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
				vertex0.color = Color::WHITE;
				vertex0.texcoord = Vector2(0.0f, 1.0f);

				SpriteVertex& vertex1 = _commonResource.vertices.at(1);
				vertex1.position = Vector4(-0.5f, +0.5f, 0.0f, 1.0f);
				vertex1.color = Color::WHITE;
				vertex1.texcoord = Vector2(0.0f, +0.0f);

				SpriteVertex& vertex2 = _commonResource.vertices.at(2);
				vertex2.position = Vector4(+0.5f, -0.5f, 0.0f, 1.0f);
				vertex2.color = Color::WHITE;
				vertex2.texcoord = Vector2(+1.0f, 1.0f);

				SpriteVertex& vertex3 = _commonResource.vertices.at(3);
				vertex3.position = Vector4(+0.5f, +0.5f, 0.0f, 1.0f);
				vertex3.color = Color::WHITE;
				vertex3.texcoord = Vector2(+1.0f, 0.0f);
			}

			// Indices
			{
				_commonResource.indices =
				{
					1, 0, 2, 2, 3, 1
				};
			}

			// Vertex buffer
			{
				using VertexType = DXVertexBuffer::VertexBufferInfo::VertexType;
				using AccessFlag = IDirectXBuffer::CpuAccessFlag;
				using Usage = IDirectXBuffer::Usage;
				using MapType = IDirectXBuffer::MapType;

				DXVertexBuffer::VertexBufferInfo vertexBufferInfo = {};
				vertexBufferInfo.usage = Usage::Default;
				vertexBufferInfo.accessFlag = AccessFlag::NoAccessWriteAndRead;
				vertexBufferInfo.mapType = MapType::None;
				vertexBufferInfo.vertexType = VertexType::SpriteVertex;
				vertexBufferInfo.stride = sizeof(SpriteVertex);
				vertexBufferInfo.size = sizeof(SpriteVertex) * static_cast<uint32_t>(_commonResource.vertices.size());
				vertexBufferInfo.count = static_cast<uint32_t>(_commonResource.vertices.size());
				vertexBufferInfo.data = std::make_shared<SpriteVertex[]>(_commonResource.vertices.size());
				vertexBufferInfo.AssignData(_commonResource.vertices.data());

				_commonResource.vertexBuffer = std::make_shared<DXVertexBuffer>();
				_commonResource.vertexBuffer->Create(vertexBufferInfo);
			}

			// Index buffer
			{
				using AccessFlag = IDirectXBuffer::CpuAccessFlag;
				using Usage = IDirectXBuffer::Usage;
				using MapType = IDirectXBuffer::MapType;

				DXIndexBuffer::IndexBufferInfo indexBufferInfo = {};
				indexBufferInfo.usage = Usage::Default;
				indexBufferInfo.accessFlag = AccessFlag::NoAccessWriteAndRead;
				indexBufferInfo.mapType = MapType::None;
				indexBufferInfo.size = sizeof(uint32_t) * _commonResource.indices.size();
				indexBufferInfo.count = static_cast<uint32_t>(_commonResource.indices.size());
				indexBufferInfo.data = std::make_shared<uint32_t[]>(_commonResource.indices.size());
				indexBufferInfo.AssignData(_commonResource.indices.data());

				_commonResource.indexBuffer = std::make_shared<DXIndexBuffer>();
				_commonResource.indexBuffer->Create(indexBufferInfo);
			}

			// Get shader from library.
			{
				const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

				const std::optional<Ref<Shader>> shader = shaderLibrary->Find(ShaderLibrary::SPRITE_3D_SHADER_PATH);
				DENTY_CHECK_CONDITION(shader.has_value());

				_commonResource.shader = shader.value();
			}
		}
	}
}
