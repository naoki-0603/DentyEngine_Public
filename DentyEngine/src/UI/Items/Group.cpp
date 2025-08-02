#include "stdafx.h"

#include "Group.h"

namespace DentyEngine
{
	UIGroup::UIGroup() :
		UserInterfaceItem(), Object()
	{
		_name = "UIGroup";
	}

	void UIGroup::Create()
	{
	}

	void UIGroup::Render()
	{
	}

	void UIGroup::OnGui()
	{
		UserInterfaceItem::OnGui();
	}

	void UIGroup::OnUpdate()
	{
	}

	void UIGroup::OnSerialized() const
	{
		Object::OnSerialized();
	}

	void UIGroup::OnDeserialized()
	{
		Object::OnDeserialized();
	}

	void UIGroup::OnSceneActivated()
	{
	}

	void UIGroup::OnSceneDeActivated()
	{
	}

	void UIGroup::OnRuntimeSceneChanged()
	{
	}

	const char* UIGroup::GetItemType() const
	{
		return "Group";
	}

	Vector2 UIGroup::GetSize() const
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return Vector2();
	}

	void UIGroup::Initialize()
	{

	}
}
