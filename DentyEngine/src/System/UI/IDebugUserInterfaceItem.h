#pragma once

namespace DentyEngine
{
	class IDebugUserInterfaceItem
	{
	public:
		IDebugUserInterfaceItem() = default;
		virtual ~IDebugUserInterfaceItem() = default;

		// Called when debug mode.
		virtual void DebugRender() = 0;
	};
}