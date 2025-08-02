#pragma once

#include "Utilities/Utilities.h"

namespace DentyEngine
{
	class IManager
	{
	public:
		IManager() = default;
		virtual ~IManager() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(IManager)

		virtual void OnGui() = 0;
	};
};