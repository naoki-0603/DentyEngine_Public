#pragma once

#include "System/UI/UserInterfaceItem.h"

namespace DentyEngine
{
	class UIGroup final : public UserInterfaceItem, public Object
	{
	public:
		UIGroup();
		~UIGroup() override = default;

		void Create();
		void Render() override;

		void OnGui() override;
		void OnUpdate() override;

		void OnSerialized() const override;;
		void OnDeserialized() override;

		void OnSceneActivated() override;
		void OnSceneDeActivated() override;
		void OnRuntimeSceneChanged() override;

		//
		// Getter
		//
		[[nodiscard]]
		const char* GetItemType() const override;

		[[nodiscard]]
		Vector2 GetSize() const override;
	private:
		void Initialize();

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Object>(this),
					cereal::base_class<UserInterfaceItem>(this)
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
					cereal::base_class<UserInterfaceItem>(this)
				);
			}

			OnDeserialized();
		}
	};
}

CEREAL_CLASS_VERSION(DentyEngine::UIGroup, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::UIGroup)