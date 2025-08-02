#include "stdafx.h"

#include "DragAndDropTarget.h"

namespace DentyEngine
{
	DragAndDropTarget::DragAndDropTarget(const DragAndDropSpecification& specification) :
		DragAndDrop(specification), _beginDragAndDrop()
	{
		_beginDragAndDrop = ImGui::BeginDragDropTarget();

		if (_beginDragAndDrop)
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(specification.type, specification.flags))
			{
				DENTY_ASSERT(payload->DataSize == specification.dataSize, "Data size must be same!");

				specification.callback(payload->Data, specification.dragAndDropDataType, specification.userData);
			}
		}
	}

	DragAndDropTarget::~DragAndDropTarget()
	{
		if (_beginDragAndDrop)
		{
			ImGui::EndDragDropTarget();
		}
		else
		{
			const auto specification = GetSpecification();
		    specification.callbackInDestructor(nullptr, specification.dragAndDropDataType, nullptr);
		}
	}
}
