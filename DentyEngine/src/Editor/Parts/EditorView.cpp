#include "stdafx.h"

#include "EditorView.h"

namespace DentyEngine
{
	EditorView::EditorView() :
		IEditor(), _currentSize(), _previousSize()
	{
	}

	void EditorView::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void EditorView::OnGui(Ref<Scene> activeScene)
	{
		// Previous content size
		_previousSize = _currentSize;

		// Current content size
		_currentSize = Vector2(
			ImGui::GetContentRegionAvail().x,
			ImGui::GetContentRegionAvail().y
		);
	}

	bool EditorView::IsViewSizeChanged(const Vector2& prev, const Vector2& cur)
	{
		const auto prevX = static_cast<int>(prev.x);
		const auto prevY = static_cast<int>(prev.y);

		const auto curX = static_cast<int>(cur.x);
		const auto curY = static_cast<int>(cur.y);

		return (prevX != curX or prevY != curY);
	}
}