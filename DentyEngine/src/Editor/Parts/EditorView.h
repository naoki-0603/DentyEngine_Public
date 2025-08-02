#pragma once

#include "IEditor.h"

namespace DentyEngine
{
	class EditorView : public IEditor
	{
	public:
		EditorView();
		virtual ~EditorView() override = default;

		virtual void OnEvent(Event* e) override;
		virtual void OnGui(Ref<Scene> activeScene) override;
	protected:
		bool IsViewSizeChanged(const Vector2& prev, const Vector2& cur);
	protected:
		Vector2 _currentSize;
		Vector2 _previousSize;

		static constexpr float WINDOW_PADDING_Y = 6.0;
	};
}
