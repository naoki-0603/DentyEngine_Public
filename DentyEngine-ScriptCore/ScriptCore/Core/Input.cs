using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public static class Input
    {
        //
        // Keyboard
        //
        
        public static bool IsKeyPressed(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyPressed(keyCode);
        }

        public static bool IsKeyTriggered(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyTriggered(keyCode);
        }

        public static bool IsKeyReleased(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyReleased(keyCode);
        }


        //
        // Mouse
        //
        
        public static bool IsMousePressed(MouseButton button)
        {
            return InternalCalls.Input_IsMousePressed(button);
        }

        public static bool IsMouseTriggered(MouseButton button)
        {
            return InternalCalls.Input_IsMouseTriggered(button);
        }

        public static bool IsMouseReleased(MouseButton button)
        {
            return InternalCalls.Input_IsMouseReleased(button);
        }

        public static void SetMousePosition(Vector2I position)
        {
            InternalCalls.Input_SetMousePosition(position.x, position.y);
        }

        public static float GetMouseWheelSpeed()
        {
            float speed = 0.0f;

            InternalCalls.Input_GetMouseWheelSpeed(out speed);

            return speed;
        }

        public static float GetMousePanX()
        {
            float panX = 0.0f;
            
            InternalCalls.Input_GetMousePanX(out panX);

            return panX;
        }

        public static float GetMousePanY()
        {
            float panY = 0.0f;

            InternalCalls.Input_GetMousePanY(out panY);

            return panY;
        }

        public static Vector2I GetMousePosition()
        {
            Vector2I position = new Vector2I();
            
            InternalCalls.Input_GetMousePosition(out position.x, out position.y);

            return position;
        }

        //
        // Gamepad
        //
        public static bool IsPadPressed(PadKeyCode padKeyCode, uint padNum = 0)
        {
            return InternalCalls.Input_IsPadPressed(padKeyCode, padNum);
        }

        public static bool IsPadTriggered(PadKeyCode padKeyCode, uint padNum = 0)
        {
            return InternalCalls.Input_IsPadTriggered(padKeyCode, padNum);
        }

        public static bool IsPadReleased(PadKeyCode padKeyCode, uint padNum = 0)
        {
            return InternalCalls.Input_IsPadReleased(padKeyCode, padNum);
        }

        public static bool IsPadLeftTriggerTriggered(uint padNum = 0)
        {
            return InternalCalls.Input_IsPadLeftTriggerTriggered(padNum);
        }

        public static bool IsPadRightTriggerTriggered(uint padNum = 0)
        {
            return InternalCalls.Input_IsPadRightTriggerTriggered(padNum);
        }

        public static bool IsPadConnectedAt(uint padNum)
        {
            return InternalCalls.Input_IsPadConnectedAt(padNum);
        }

        public static byte GetLeftTrigger(uint padNum = 0)
        {
            return InternalCalls.Input_GetLeftTrigger(padNum);
        }

        public static byte GetRightTrigger(uint padNum = 0)
        {
            return InternalCalls.Input_GetRightTrigger(padNum);
        }

        public static short GetThumbLX(uint padNum = 0)
        {
            return InternalCalls.Input_GetThumbLX(padNum);
        }

        public static short GetThumbLY(uint padNum = 0)
        {
            return InternalCalls.Input_GetThumbLY(padNum);
        }

        public static short GetThumbRX(uint padNum = 0)
        {
            return InternalCalls.Input_GetThumbRX(padNum);
        }

        public static short GetThumbRY(uint padNum = 0)
        {
            return InternalCalls.Input_GetThumbRY(padNum);
        }

        public static float GetNormalizedThumbLX(uint padNum = 0)
        {
            return InternalCalls.Input_GetNormalizedThumbLX(padNum);
        }

        public static float GetNormalizedThumbLY(uint padNum = 0)
        {
            return InternalCalls.Input_GetNormalizedThumbLY(padNum);
        }

        public static float GetNormalizedThumbRX(uint padNum = 0)
        {
            return InternalCalls.Input_GetNormalizedThumbRX(padNum);
        }

        public static float GetNormalizedThumbRY(uint padNum = 0)
        {
            return InternalCalls.Input_GetNormalizedThumbRY(padNum);
        }
    }
}
