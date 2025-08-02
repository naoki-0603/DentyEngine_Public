#include "stdafx.h"

#include "DragAndDropSource.h"

namespace DentyEngine
{
	DragAndDropSource::DragAndDropSource(const DragAndDropSpecification& specification) :
		DragAndDrop(specification), _beginDragAndDrop()
	{
		_beginDragAndDrop = ImGui::BeginDragDropSource(specification.flags);
		if (_beginDragAndDrop)
		{
			specification.callback(specification.data, specification.dragAndDropDataType, specification.userData);
		}
	}

	DragAndDropSource::~DragAndDropSource()
	{
		if (_beginDragAndDrop)
		{
			ImGui::EndDragDropSource();
		}
	}
}
