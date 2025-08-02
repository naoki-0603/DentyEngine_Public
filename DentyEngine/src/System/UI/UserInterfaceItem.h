#pragma once

#include "UI/UITransform.h"

namespace DentyEngine
{
	enum class UserInterfaceItemRenderingOrder : int32_t
	{
		None = -1,
		DistanceView = 0,
		MiddleDistance,
		CloseView,

		Max
	};

	class UserInterfaceItem : public std::enable_shared_from_this<UserInterfaceItem>
	{
	public:
		UserInterfaceItem();
		virtual ~UserInterfaceItem() = default;

		virtual void OnUpdate() = 0;
		virtual void Render() = 0;
		virtual void OnGui();

		virtual void OnSceneActivated() = 0;
		virtual void OnSceneDeActivated() = 0;
		virtual void OnRuntimeSceneChanged() = 0;

		void SetRender(bool render) { _render = render; }

		[[nodiscard]]
		virtual const char* GetItemType() const = 0;

		[[nodiscard]]
		virtual Vector2 GetSize() const = 0;

		[[nodiscard]]
		UserInterfaceItemRenderingOrder GetRenderingOrder() const { return _renderingOrder; }

		[[nodiscard]]
		bool IsRender() const { return _render; }

	public:
		UITransform transform;

	protected:
		UserInterfaceItemRenderingOrder _renderingOrder;

		bool _render;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(transform),
					CEREAL_NVP(_renderingOrder),
					CEREAL_NVP(_render)
				);
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(transform),
					CEREAL_NVP(_renderingOrder),
					CEREAL_NVP(_render)
				);
			}
		}
	private:
		std::string _renderingOrderString;

	private:
		static const std::array<std::string, 3u> RENDERING_ORDER_TYPES;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::UserInterfaceItem, 1u)