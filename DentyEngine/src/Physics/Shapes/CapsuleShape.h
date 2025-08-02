#pragma once

#include "IShape.h"

namespace DentyEngine
{
	class CapsuleShape final : public IShape
	{
	public:
		CapsuleShape();
		CapsuleShape(const CapsuleShape& source);
		~CapsuleShape() override = default;

		void Create();
		void Create(float radius, float height);

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
		const char* GetShapeType() const override { return "Capsule"; }

		[[nodiscard]]
		float GetRadius() const { return _radius; }

		[[nodiscard]]
		float GetHeight() const { return _height; }
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(_radius),
				CEREAL_NVP(_height)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(_radius),
				CEREAL_NVP(_height)
			);

			OnDeserialized();
		}
	private:
		Ref<btCapsuleShape> _shape;

		float _radius;
		float _height;
	private:
		static constexpr float DEFAULT_RADIUS = 0.104f;
		static constexpr float DEFAULT_HEIGHT = 0.145f;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::CapsuleShape, 1u)