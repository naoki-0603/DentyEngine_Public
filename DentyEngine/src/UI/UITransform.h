#pragma once

#include "System/Core/Object.h"
#include "UITransformPivot.h"

namespace DentyEngine
{
	class UserInterfaceItem;

	class UITransform final : public Object
	{
	public:
		// For serialize.
		UITransform() = default;

		UITransform(const UserInterfaceItem* item);
		~UITransform() override = default;

		void OnGui();

		void OnSerialized() const override;
		void OnDeserialized() override;

		void OnResize(const Vector2& size);
	public:
		UITransformPivot pivot;

		Vector2 position;
		float scale;
		float rotation;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Object>(this),
					CEREAL_NVP(pivot),
					CEREAL_NVP(position),
					CEREAL_NVP(scale),
					CEREAL_NVP(rotation),
					CEREAL_NVP(_positionRate)
				);
			}

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Object>(this),
					CEREAL_NVP(pivot),
					CEREAL_NVP(position),
					CEREAL_NVP(scale),
					CEREAL_NVP(rotation),
					CEREAL_NVP(_positionRate)
				);
			}

			OnDeserialized();
		}
	private:
		Vector2 _positionRate;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::UITransform, 1u)
CEREAL_REGISTER_TYPE(DentyEngine::UITransform)