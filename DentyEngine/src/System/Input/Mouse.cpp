#include "stdafx.h"

#include "Mouse.h"

#include "System/Core/Window.h"

namespace DentyEngine
{
	Mouse::Mouse() :
		_keys(), _prevKeys(), _prevPosition(), _curPosition(), _wheelSpeed()
	{
	}

	void Mouse::Update()
	{
		_prevKeys = _keys;

		if (GetKeyboardState(_keys.data()) == 0)
		{
			// Failed
			const DWORD errorCode = GetLastError();

			DENTY_SET_FATAL_COLOR
				DENTY_FATAL_LOG << "ErrorCode: " << errorCode;

			DENTY_ASSERT(false, "Failed to get keyboard state! (in Mouse::Update)");
		}

		{
			_prevPosition = _curPosition;

			POINT point = { };
			GetCursorPos(&point);

			ScreenToClient(Window::GetInfo().GetHwnd(), &point);

			_curPosition.x = point.x;
			_curPosition.y = point.y;
		}
	}

	void Mouse::OnGui()
	{
		ImGui::Text("PressedLButton: %s", IsPressed(MouseButton::LeftButton) ? "true" : "false");
		ImGui::Text("PressedMButton: %s", IsPressed(MouseButton::MiddleButton) ? "true" : "false");
		ImGui::Text("PressedRButton: %s", IsPressed(MouseButton::RightButton) ? "true" : "false");

		ImGui::Separator();

		ImGui::Text("TriggeredLButton: %s", IsTrigger(MouseButton::LeftButton) ? "true" : "false");
		ImGui::Text("TriggeredMButton: %s", IsTrigger(MouseButton::MiddleButton) ? "true" : "false");
		ImGui::Text("TriggeredRButton: %s", IsTrigger(MouseButton::RightButton) ? "true" : "false");

		ImGui::Separator();

		ImGui::Text("ReleasedLButton: %s", IsReleased(MouseButton::LeftButton) ? "true" : "false");
		ImGui::Text("ReleasedMButton: %s", IsReleased(MouseButton::MiddleButton) ? "true" : "false");
		ImGui::Text("ReleasedRButton: %s", IsReleased(MouseButton::RightButton) ? "true" : "false");

		ImGui::Separator();

		ImGui::Text("PanX: %f", GetPanX());
		ImGui::Text("PanY: %f", GetPanY());
		ImGui::Text("WheelSpeed: %f", GetWheelSpeed());

		ImGui::Separator();

		ImGui::Text("CurrentPosition: %d, %d", _curPosition.x, _curPosition.y);
	}

	bool Mouse::IsPressed(MouseButton button) const
	{
		const uint32_t virtualKey = GetVirtualKey(button);

		if (virtualKey == UINT_MAX) return false;

		return _keys[virtualKey] & 0x80u;
	}

	bool Mouse::IsTrigger(MouseButton button) const
	{
		const uint32_t virtualKey = GetVirtualKey(button);

		if (virtualKey == UINT_MAX) return false;

		return (~(_prevKeys[virtualKey]) & (_keys[virtualKey]));
	}

	bool Mouse::IsReleased(MouseButton button) const
	{
		const uint32_t virtualKey = GetVirtualKey(button);

		if (virtualKey == UINT_MAX) return false;

		return ((_prevKeys[virtualKey] & 0x80u) && !(_keys[virtualKey] & 0x80u));
	}

	void Mouse::SetMousePositionX(int32_t x)
	{
		POINT mousePoint = {};
		GetCursorPos(&mousePoint);

		SetCursorPos(x, mousePoint.y);
	}

	void Mouse::SetMousePositionY(int32_t y)
	{
		POINT mousePoint = {};
		GetCursorPos(&mousePoint);

		SetCursorPos(mousePoint.x, y);
	}

	void Mouse::SetMousePosition(const Vector2I& position)
	{
		SetCursorPos(position.x, position.y);
	}

	void Mouse::SetWheelSpeed(float wheelSpeed)
	{
		_wheelSpeed = wheelSpeed;
	}

	void Mouse::ResetWheelSpeed()
	{
		_wheelSpeed = 0.0f;
	}

	int32_t Mouse::GetMousePositionX() const
	{
		return _curPosition.x;
	}

	int32_t Mouse::GetMousePositionY() const
	{
		return _curPosition.y;
	}

	Vector2I Mouse::GetMousePosition() const
	{
		return _curPosition;
	}

	float Mouse::GetPanX() const
	{
		const float panX = static_cast<float>(_curPosition.x - _prevPosition.x);

		bool isMinus = (panX < 0.0f);
		if (fabsf(panX) > MAX_PAN_ABS)
		{
			return (MAX_PAN_ABS * (isMinus ? -1.0f : 1.0f));
		}

		return panX;
	}

	float Mouse::GetPanY() const
	{
		const float panY = static_cast<float>(_curPosition.y - _prevPosition.y);

		bool isMinus = (panY < 0.0f);
		if (fabsf(panY) > MAX_PAN_ABS)
		{
			return (MAX_PAN_ABS * (isMinus ? -1.0f : 1.0f));
		}

		return panY;
	}

	float Mouse::GetWheelSpeed() const
	{
		return _wheelSpeed;
	}

	uint32_t Mouse::GetVirtualKey(MouseButton button) const
	{
		switch (button)
		{
		case MouseButton::LeftButton:
			return VK_LBUTTON;
		case MouseButton::MiddleButton:
			return VK_MBUTTON;
		case MouseButton::RightButton:
			return VK_RBUTTON;
		}

		return UINT32_MAX;
	}
}