#pragma once

namespace DentyEngine
{
	class ISelectableUserInterfaceItem
	{
	public:
		ISelectableUserInterfaceItem() = default;
		virtual ~ISelectableUserInterfaceItem() = default;

		virtual void OnSelected() = 0;
	};
}