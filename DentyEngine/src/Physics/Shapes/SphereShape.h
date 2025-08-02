#pragma once

#include "IShape.h"

namespace DentyEngine
{
	class SphereShape final : public IShape
	{
	public:
		SphereShape();
		SphereShape(const SphereShape& source);
		~SphereShape() override = default;

		void Create();
		void Create(float radius);

		void Update(const Vector3& size) override;

		void OnGui() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		//
		// Getter
		//
		[[nodiscard]]
		Ref<btCollisionShape> GetBtCollisionShape() const override { return _shape; }

		[[nodiscard]]
		const char* GetShapeType() const override { return "Sphere"; }
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(_radius)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version) 
		{
			archive(
				CEREAL_NVP(_radius)
			);

			OnDeserialized();
		}
	private:
		Ref<btSphereShape> _shape;

		float _radius;
	private:
		static constexpr float DEFAULT_RADIUS = 0.104f;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::SphereShape, 1u)