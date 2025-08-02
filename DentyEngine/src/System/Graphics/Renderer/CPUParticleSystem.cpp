#include "stdafx.h"

#include "CPUParticleSystem.h"

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"
#include "Graphics/Buffer/DirectX/DXIndexBuffer.h"

#include "Graphics/Renderer/Shader.h"

#include "Graphics/States/RasterizerState.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
	CPUParticleConstants::CPUParticleConstants() :
		world(), color()
	{
	}

	CPUParticleSystemContext::CPUParticleSystemContext() :
		vertices(), indices(), vertexBuffer(), indexBuffer(), particleShader(), context(), constants()
	{
	}

	CPUParticleSystem CPUParticleSystem::_instance = { };

	CPUParticleSystem::CPUParticleSystem() :
		IParticleSystem(), _particlePoolIndex(PARTICLE_POOL_MAX_SIZE - 1u),
		_currentEmitParticleCount(), _context(), _particlePool()
	{
	}

	void CPUParticleSystem::Reset()
	{
		_currentEmitParticleCount = 0;
		_particlePoolIndex = PARTICLE_POOL_MAX_SIZE - 1u;

		for (auto& particle : _particlePool)
		{
			if (!particle.isActive)
				continue;

			particle = { };
		}
	}

	CPUParticleSystem::~CPUParticleSystem()
	{
	}

	void CPUParticleSystem::Initialize()
	{
		// Resize
		{
			_particlePool.resize(PARTICLE_POOL_MAX_SIZE);
		}

		// Setup context.
		{
			// Vertices
			{
				ParticleVertex& vertex0 = _context.vertices.at(0);
				vertex0.position = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
				vertex0.texcoord = Vector2(0.0f, 0.0f);

				ParticleVertex& vertex1 = _context.vertices.at(1);
				vertex1.position = Vector4(-0.5f, +0.5f, 0.0f, 1.0f);
				vertex1.texcoord = Vector2(0.0f, +1.0f);

				ParticleVertex& vertex2 = _context.vertices.at(2);
				vertex2.position = Vector4(+0.5f, -0.5f, 0.0f, 1.0f);
				vertex2.texcoord = Vector2(+1.0f, 0.0f);

				ParticleVertex& vertex3 = _context.vertices.at(3);
				vertex3.position = Vector4(+0.5f, +0.5f, 0.0f, 1.0f);
				vertex3.texcoord = Vector2(+1.0f, +1.0f);
			}

			// Indices
			{
				_context.indices =
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
				vertexBufferInfo.vertexType = VertexType::ParticleVertex;
				vertexBufferInfo.stride = sizeof(ParticleVertex);
				vertexBufferInfo.size = sizeof(ParticleVertex) * static_cast<uint32_t>(_context.vertices.size());
				vertexBufferInfo.count = static_cast<uint32_t>(_context.vertices.size());
				vertexBufferInfo.data = std::make_shared<ParticleVertex[]>(_context.vertices.size());
				vertexBufferInfo.AssignData(_context.vertices.data());

				_context.vertexBuffer = std::make_shared<DXVertexBuffer>();
				_context.vertexBuffer->Create(vertexBufferInfo);
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
				indexBufferInfo.size = sizeof(uint32_t) * _context.indices.size();
				indexBufferInfo.count = static_cast<uint32_t>(_context.indices.size());
				indexBufferInfo.data = std::make_shared<uint32_t[]>(_context.indices.size());
				indexBufferInfo.AssignData(_context.indices.data());

				_context.indexBuffer = std::make_shared<DXIndexBuffer>();
				_context.indexBuffer->Create(indexBufferInfo);
			}

			// Get shader from library.
			{
				const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

				const std::optional<Ref<Shader>> shader = shaderLibrary->Find(ShaderLibrary::CPU_PARTICLE_SHADER_PATH);
				DENTY_CHECK_CONDITION(shader.has_value());

				_context.particleShader = shader.value();
			}

			// Get context
			{
				_context.context = GraphicsManager::GetInstance().GetContext();
			}
		}
	}

	void CPUParticleSystem::OnUpdate(float deltaTime)
	{
		for (auto& particle : _particlePool)
		{
			if (!particle.isActive)
				continue;

			if (particle.remainingLifeTime <= 0.0f)
			{
				particle.isActive = false;

				--_currentEmitParticleCount;
			}

			if (particle.applyGravityForce)
			{
				particle.velocity += PARTICLE_GRAVITY * deltaTime;
			}

			particle.remainingLifeTime -= deltaTime;

			if (particle.useBeginAndEndPosition)
			{
				const float life = particle.remainingLifeTime / particle.lifeTime;

				const float x = std::lerp(particle.beginPosition.x, particle.endPosition.x, 1.0f - life);
				const float y = std::lerp(particle.beginPosition.y, particle.endPosition.y, 1.0f - life);
				const float z = std::lerp(particle.beginPosition.z, particle.endPosition.z, 1.0f - life);

				particle.position = Vector3(x, y, z);
			}
			else
			{
				particle.position += particle.velocity * deltaTime;
			}

			particle.rotation += 0.1f * deltaTime;
		}
	}

	void CPUParticleSystem::OnRender(const Matrix& view)
	{
		// Get context caches.
		ComRef<ID3D11RasterizerState> cachedRasterizerState;
		_context.context->RSGetState(cachedRasterizerState.GetAddressOf());

		_context.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_context.vertexBuffer->Bind();
		_context.indexBuffer->Bind();

		_context.particleShader->BindVertexShader(_context.context);
		_context.particleShader->BindPixelShader(_context.context);

		ID3D11RasterizerState* rasterizerState = RasterizerState::GetBuildInRasterizerStatesAt(RasterizerState::Type::FillNoCull);
		_context.context->RSSetState(rasterizerState);

		for (auto&& particle : _particlePool)
		{
			if (!particle.isActive)
				continue;

			const float life = particle.remainingLifeTime / particle.lifeTime;

			const float resultSize = std::lerp(particle.beginSize, particle.endSize, 1.0f - life);
			const Color resultColor = Color::Lerp(particle.beginColor, particle.endColor, 1.0f - life);

			// Update constants.
			{
				const Matrix S = DirectX::XMMatrixScaling(resultSize, resultSize, 1.0f);
				const Matrix R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, particle.rotation);
				const Matrix T = DirectX::XMMatrixTranslation(particle.position.x, particle.position.y, particle.position.z);

				Matrix inverseView = view.Inverse();
				inverseView.SetOffsetVec3(Vector3::Zero());

				_context.constants.color = resultColor;
				_context.constants.world = S * R * inverseView * T;
			}

			_context.particleShader->constantBuffer->UpdateSubresource(_context.context, &_context.constants);
			_context.particleShader->constantBuffer->BindAll(_context.context, 0u);

			_context.context->DrawIndexed(_context.indices.size(), 0u, 0);
		}

		_context.context->RSSetState(cachedRasterizerState.Get());
	}

	void CPUParticleSystem::OnGui()
	{
		if (ImGui::CollapsingHeader("CPUParticleSystem##CoreLayer"))
		{
			ImGui::TextWrapped("CurrentEmitCount: %u", _currentEmitParticleCount);
		}
	}

	void CPUParticleSystem::Emit(const ParticleProps& props)
	{
		Particle& particle = _particlePool.at(_particlePoolIndex);
		particle.position = props.position;

		particle.beginPosition = props.beginPosition;
		particle.endPosition = props.endPosition;

		particle.velocity = props.initialVelocity;
		particle.velocity += props.velocityVariation * (Random::GetRandomValueAsFloat() - 0.5f);

		particle.rotation = props.rotation + 2.0f * (Random::GetRandomValueAsFloat() - 0.5f);

		particle.beginSize = props.beginSize + props.sizeVariation * (Random::GetRandomValueAsFloat() - 0.5f);
		particle.endSize = props.endSize;

		particle.beginColor = props.beginColor;
		particle.endColor = props.endColor;

		particle.lifeTime = props.lifeTime;
		particle.remainingLifeTime = props.lifeTime;

		particle.applyGravityForce = props.applyGravityForce;
		particle.useBeginAndEndPosition = props.useBeginAndEndPosition;

		particle.isActive = true;

		++_currentEmitParticleCount;

		_particlePoolIndex = --_particlePoolIndex % _particlePool.size();
	}

	void CPUParticleSystem::OnScenePlayButtonPressed()
	{
		Reset();
	}
}
