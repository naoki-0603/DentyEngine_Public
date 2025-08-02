#pragma once

#include "IParticleSystem.h"

#include "Graphics/Vertex.h"

namespace DentyEngine
{
	class DXVertexBuffer;
	class DXIndexBuffer;

	class Shader;

	struct CPUParticleConstants
	{
	public:
		CPUParticleConstants();

	public:
		Matrix world;

		Color color;
	};

	struct CPUParticleSystemContext
	{
	public:
		CPUParticleSystemContext();

	public:
		std::array<ParticleVertex, 4u> vertices;
		std::array<uint32_t, 6u> indices;

		Ref<DXVertexBuffer> vertexBuffer;
		Ref<DXIndexBuffer> indexBuffer;
		Ref<Shader> particleShader;

		ID3D11DeviceContext* context;

		CPUParticleConstants constants;
	};

	class CPUParticleSystem final : public IParticleSystem
	{
	public:
		~CPUParticleSystem() override;

		void Initialize() override;

		void OnUpdate(float deltaTime) override;
		void OnRender(const Matrix& view) override;

		void OnGui() override;

		void Emit(const ParticleProps& props) override;

		void OnScenePlayButtonPressed() override;

		//
		// Static function.
		//
		static CPUParticleSystem* GetInstance()
		{
			return &_instance;
		}
	private:
		CPUParticleSystem();

	private:
		void Reset();

	private:
		uint32_t _particlePoolIndex;
		uint32_t _currentEmitParticleCount;

		CPUParticleSystemContext _context;

		std::vector<Particle> _particlePool;
	private:
		static CPUParticleSystem _instance;

	private:
		static constexpr Vector3 PARTICLE_GRAVITY = Vector3(0.0f, -9.8f, 0.0f);

		static constexpr uint32_t PARTICLE_POOL_MAX_SIZE = 1000u;
	};
}