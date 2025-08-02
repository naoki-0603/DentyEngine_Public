#pragma once

#include "UserInterfaceItem.h"

namespace DentyEngine
{
	class EventOnGameViewSizeChanged;

	struct UISystemSpecification
	{
	public:
		UISystemSpecification();

	public:
		std::unordered_map<UserInterfaceItemRenderingOrder, std::vector<Ref<UserInterfaceItem>>> items;

		Vector2 canvasSize;
	};

	class EventOnSceneActivated;
	class EventOnSceneDeActivated;
	class EventRuntimeSceneChanged;

	class UIText;
	class UIImage;

	class UISystem final
	{
	public:
		UISystem();
		~UISystem() = default;

		static void Initialize();

		static void OnEvent(Event* e);

		static void AddItem(Ref<UserInterfaceItem> item);
		static void RemoveItem(const Ref<UserInterfaceItem>& item);

		static void Clear();

		//
		// Event functions
		//

		// Called on RenderManager::OnSceneRender function.
		static void OnSceneRender();

		//
		// Getter
		//
		[[nodiscard]]
		static const Vector2& GetCanvasSize();
	private:
		static void OnSceneActivated(EventOnSceneActivated* e);
		static void OnSceneDeActivated(EventOnSceneDeActivated* e);
		static void OnRuntimeSceneChanged(EventRuntimeSceneChanged* e);
		static void OnGameViewSizeChanged(EventOnGameViewSizeChanged* e);

		static void Resize();

		static void RenderText(const Ref<UIText>& text);
		static void RenderImage(const Ref<UIImage>& image);
	private:
		static UISystemSpecification _specification;
	};
}
