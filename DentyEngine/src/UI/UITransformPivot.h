#pragma once

namespace DentyEngine
{
	enum class Pivot : int32_t
	{
		None = -1,
		TopLeft = 0,
		TopRight,
		BottomLeft,
		BottomRight,
		Center,

		Max
	};

	class UserInterfaceItem;

	class UITransformPivot final
	{
	public:
		// For serialize.
		UITransformPivot() = default;

		UITransformPivot(const UserInterfaceItem* item);
		~UITransformPivot() = default;

		void OnGui();

		//
		// Setter
		//
		void SetPivot(Pivot pivot);

		//
		// Getter
		//
		[[nodiscard]]
		Vector2 GetPivotOffset() const;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(_pivot),
					CEREAL_NVP(_pivotString)
				);
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(_pivot),
					CEREAL_NVP(_pivotString)
				);
			}
		}
	private:
		const UserInterfaceItem* _itemRef;

		Pivot _pivot;

		std::string _pivotString;
	private:
		static const std::array<std::string, 5u> PIVOT_TYPES;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::UITransformPivot, 1u)