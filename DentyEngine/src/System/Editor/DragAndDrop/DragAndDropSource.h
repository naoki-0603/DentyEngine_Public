#pragma once

#include "DragAndDrop.h"

namespace DentyEngine
{
	class DragAndDropSource final : public DragAndDrop
	{
	public:
		DragAndDropSource(const DragAndDropSpecification&);
		~DragAndDropSource() override;

		DELETE_COPY_MOVE_CONSTRUCTORS(DragAndDropSource)
	private:
		bool _beginDragAndDrop;
	};
}
