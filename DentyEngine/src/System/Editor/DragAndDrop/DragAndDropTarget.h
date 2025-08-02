#pragma once

#include "DragAndDrop.h"

namespace DentyEngine
{
	class DragAndDropTarget final : public DragAndDrop
	{
	public:
		DragAndDropTarget(const DragAndDropSpecification&);
		~DragAndDropTarget() override;

		DELETE_COPY_MOVE_CONSTRUCTORS(DragAndDropTarget)
	private:
		bool _beginDragAndDrop;
	};
}
