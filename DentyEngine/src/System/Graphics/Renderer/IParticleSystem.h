#pragma once

#include "Graphics/Color.h"

namespace DentyEngine
{
	struct ParticleProps
	{
	public:
		ParticleProps() :
			beginSize(), endSize(), sizeVariation(),
			lifeTime(1.0f), rotation(), position(), beginPosition(), endPosition(), initialVelocity(), velocityVariation(),
			beginColor(), endColor(), applyGravityForce(true), useBeginAndEndPosition() {}
			
	public:
		float beginSize;
		float endSize;
		float sizeVariation;

		float lifeTime;

		float rotation;

		Vector3 position;

		Vector3 beginPosition;
		Vector3 endPosition;

		Vector3 initialVelocity;
		Vector3 velocityVariation;

		Color beginColor;
		Color endColor;

		bool applyGravityForce;
		bool useBeginAndEndPosition;
	};

	struct Particle
	{
	public:
		Particle() :
			lifeTime(), remainingLifeTime(), beginSize(), endSize(), rotation(), position(), beginPosition(),
			endPosition(), velocity(), beginColor(), endColor(), isActive(), applyGravityForce(), useBeginAndEndPosition() {}

	public:
		float lifeTime;
		float remainingLifeTime;

		float beginSize;
		float endSize;

		float rotation;

		Vector3 position;

		Vector3 beginPosition;
		Vector3 endPosition;

		Vector3 velocity;

		Color beginColor;
		Color endColor;

		bool isActive;
		bool applyGravityForce;
		bool useBeginAndEndPosition;
	};

	class DENTY_NOVTABLE IParticleSystem
	{
	public:
		IParticleSystem() = default;
		virtual ~IParticleSystem() = default;

		virtual void Initialize() = 0;
		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender(const Matrix& view) = 0;

		virtual void OnGui() = 0;

		virtual void Emit(const ParticleProps& props) = 0;

		virtual void OnScenePlayButtonPressed() = 0;
	};
}
