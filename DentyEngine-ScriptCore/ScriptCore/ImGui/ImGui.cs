using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Schema;

namespace DentyEngine
{
    public static class ImGui
    {
        public static bool CollapsingHeader(string title, ImGuiTreeNodeFlags flags)
        {
            return InternalCalls.ImGui_CollapsingHeader(title, flags);
        }

        public static void Text(string text) 
        {
            InternalCalls.ImGui_Text(text);
        }

        public static bool InputFloat(string text, ref float value) 
        {
            return InternalCalls.ImGui_InputFloat(text, ref value);
        }

        public static bool InputFloat2(string text, ref Vector2 value)
        {
            return InternalCalls.ImGui_InputFloat2(text, ref value.x, ref value.y);
        }

        public static bool InputFloat3(string text, ref Vector3 value)
        {
            return InternalCalls.ImGui_InputFloat3(text, ref value.x, ref value.y, ref value.z);
        }

        public static bool InputFloat4(string text, ref Vector4 value)
        { 
            return InternalCalls.ImGui_InputFloat4(text, out value.x, out value.y, out value.z, out value.w);
        }

        public static bool InputInt(string title, ref int value, int step = 1, int stepFast = 100)
        {
            return InternalCalls.ImGui_InputInt(title, out value, step, stepFast);
        }

        public static bool InputInt2(string title, ref Vector2I value)
        {
            return InternalCalls.ImGui_InputInt2(title, out value.x, out value.y);
        }
      
        public static bool InputInt3(string title, ref Vector3I value)
        {
            return InternalCalls.ImGui_InputInt3(title, out value.x, out value.y, out value.z);
        }

        public static bool InputInt4(string title, ref Vector4I value)
        {
            return InternalCalls.ImGui_InputInt4(title, out value.x, out value.y, out value.z, out value.w);
        }

        public static bool DragFloat(string text, ref float value, float speed = 1.0f, float min = 0.0f, float max = 0.0f) 
        {
            return InternalCalls.ImGui_DragFloat(text, out value, speed, min, max);
        }

        public static bool DragFloat2(string text, ref Vector2 value, float speed = 1.0f, float min = 0.0f, float max = 0.0f)
        {
            return InternalCalls.ImGui_DragFloat2(text, out value.x, out value.y, speed, min, max);
        }

        public static bool DragFloat3(string text, ref Vector3 value, float speed = 1.0f, float min = 0.0f, float max = 0.0f)
        {
            return InternalCalls.ImGui_DragFloat3(text, out value.x, out value.y, out value.z, speed, min, max);
        }

        public static bool DragFloat4(string text, ref Vector4 value, float speed = 1.0f, float min = 0.0f, float max = 0.0f)
        {
            return InternalCalls.ImGui_DragFloat4(text, out value.x, out value.y, out value.z, out value.w, speed, min, max);
        }

        public static bool DragInt(string text, ref int value, float speed = 1.0f, int min = 0, int max = 0)
        {
            return InternalCalls.ImGui_DragInt(text, out value, speed, min, max);
        }

        public static bool DragInt2(string text, ref Vector2I value, float speed = 1.0f, int min = 0, int max = 0)
        {
            return InternalCalls.ImGui_DragInt2(text, out value.x, out value.y, speed, min, max);
        }

        public static bool DragInt3(string text, ref Vector3I value, float speed = 1.0f, int min = 0, int max = 0)
        {
            return InternalCalls.ImGui_DragInt3(text, out value.x, out value.y, out value.z, speed, min, max);
        }

        public static bool DragInt4(string text, ref Vector4I value, float speed = 1.0f, int min = 0, int max = 0)
        {
            return InternalCalls.ImGui_DragInt4(text, out value.x, out value.y, out value.z, out value.w, speed, min, max);
        }

        public static bool SliderFloat(string text, ref float value, float min, float max)
        {
            return InternalCalls.ImGui_SliderFloat(text, out value, min, max);
        }

        public static bool SliderFloat2(string text, ref Vector2 value, float min, float max)
        {
            return InternalCalls.ImGui_SliderFloat2(text, out value.x, out value.y, min, max);
        }

        public static bool SliderFloat3(string text, ref Vector3 value, float min, float max)
        {
            return InternalCalls.ImGui_SliderFloat3(text, out value.x, out value.y, out value.z, min, max);
        }

        public static bool SliderFloat4(string text, ref Vector4 value, float min, float max)
        {
            return InternalCalls.ImGui_SliderFloat4(text, out value.x, out value.y, out value.z, out value.w, min, max);
        }

        public static bool SliderInt(string text, ref int value, int min, int max) 
        {
            return InternalCalls.ImGui_SliderInt(text, out value, min, max);
        }

        public static bool SliderInt2(string text, ref Vector2I value, int min, int max) 
        {
            return InternalCalls.ImGui_SliderInt2(text, out value.x, out value.y, min, max);
        }

        public static bool SliderInt3(string text, ref Vector3I value, int min, int max) 
        {
            return InternalCalls.ImGui_SliderInt3(text, out value.x, out value.y, out value.z, min, max);
        }

        public static bool SliderInt4(string text, ref Vector4I value, int min, int max) 
        {
            return InternalCalls.ImGui_SliderInt4(text, out value.x, out value.y, out value.z, out value.w, min, max);
        }

        public static bool ColorEdit3(string text, ref Vector3 color)
        {
            return InternalCalls.ImGui_ColorEdit3(text, out color.x, out color.y, out color.z);
        }

        public static bool ColorEdit4(string text, ref Vector4 color)
        {
            return InternalCalls.ImGui_ColorEdit4(text, out color.x, out color.y, out color.z, out color.w);
        }

        public static bool InvisibleButton(string text, Vector2 size)
        {
            return InternalCalls.ImGui_InvisibleButton(text, size.x, size.y);
        }

        public static bool BeginDragAndDropTarget()
        {
            return InternalCalls.ImGui_BeginDragAndDropTarget();
        }

        public static bool DragAndDropTargetID32(string title, ImGuiDragAndDropType32 dataType, ref uint outID)
        {
            return InternalCalls.ImGui_DragAndDropTargetID32(title, dataType, out outID);
        }

        public static bool DragAndDropTargetID64(string title, ImGuiDragAndDropType64 dataType, ref ulong outID)
        {
            return InternalCalls.ImGui_DragAndDropTargetID64(title, dataType, out outID);
        }

        public static void EndDragAndDropTarget()
        {
            InternalCalls.ImGui_EndDragAndDropTarget();
        }

        public static void Separator()
        {
            InternalCalls.ImGui_Separator();
        }
        public static void SameLine(float offsetFromStart = 0.0f, float spacing = -1.0f)
        {
            InternalCalls.ImGui_SameLine(offsetFromStart, spacing);
        }

        public static void Indent(float indentW = 0.0f)
        {
            InternalCalls.ImGui_Indent(indentW);
        }

        public static void UnIndent()
        {
            InternalCalls.ImGui_UnIndent();
        }

        public static void PushStyleColor(ImGuiCol idx, float r, float g, float b, float a)
        {
            InternalCalls.ImGui_PushStyleColor(idx, r, g, b, a);
        }

        public static void PopStyleColor(int count = 1)
        {
            InternalCalls.ImGui_PopStyleColor(count);
        }

        public static void PushStyleVar(ImGuiStyleVar idx, float value)
        {
            InternalCalls.ImGui_PushStyleVar(idx, value);
        }

        public static void PushStyleVar(ImGuiStyleVar idx, Vector2 value)
        {
            InternalCalls.ImGui_PushStyleVarParam2(idx, value.x, value.y);
        }

        public static void PopStyleVar(int count = 1)
        {
            InternalCalls.ImGui_PopStyleVar(count);
        }

        public static Vector2 GetContentRegionAvail()
        {
            Vector2 size = new Vector2();
            InternalCalls.ImGui_GetContentRegionAvail(out size.x, out size.y);

            return size;
        }
    }
}