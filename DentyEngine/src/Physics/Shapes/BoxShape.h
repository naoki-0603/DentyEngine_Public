#pragma once

#include "IShape.h"

namespace DentyEngine
{
	class BoxShape final : public IShape
	{
	public:
		BoxShape();
		BoxShape(const BoxShape& source);
		~BoxShape() override;

		void Create();
		void Create(const Vector3& size);

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
		const char* GetShapeType() const override { return "Box"; }

		[[nodiscard]]
		const Vector3& GetSize() const { return _size; }
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(_size)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(_size)
			);

			OnDeserialized();
		}
	private:
		Ref<btBoxShape> _shape;

		Vector3 _size;
	private:
		static constexpr Vector3 DEFAULT_HALF_EXTENTS = { 0.1f, 0.1f, 0.1f };
	};
}

CEREAL_CLASS_VERSION(DentyEngine::BoxShape, 1u)