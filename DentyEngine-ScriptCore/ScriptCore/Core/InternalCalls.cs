using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public static class InternalCalls
    {
        #region Input
        // Keyboard
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyPressed(KeyCode key);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyTriggered(KeyCode key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyReleased(KeyCode key);

        // Mouse
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMousePressed(MouseButton button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseTriggered(MouseButton button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseReleased(MouseButton button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Input_SetMousePosition(int x, int y);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Input_GetMouseWheelSpeed(out float wheelSpeed);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Input_GetMousePanX(out float panX);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Input_GetMousePanY(out float panY);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Input_GetMousePosition(out int x, out int y);

        // Gamepad
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsPadPressed(PadKeyCode padKeyCode, uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsPadTriggered(PadKeyCode padKeyCode, uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsPadReleased(PadKeyCode padKeyCode, uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsPadLeftTriggerTriggered(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsPadRightTriggerTriggered(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsPadConnectedAt(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static byte Input_GetLeftTrigger(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static byte Input_GetRightTrigger(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static short Input_GetThumbLX(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static short Input_GetThumbLY(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static short Input_GetThumbRX(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static short Input_GetThumbRY(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Input_GetNormalizedThumbLX(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Input_GetNormalizedThumbLY(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Input_GetNormalizedThumbRX(uint padNum);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float Input_GetNormalizedThumbRY(uint padNum);

        #endregion

        #region Debug
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Debug_Log(string log);
        #endregion

        #region ImGui
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_CollapsingHeader(string title, ImGuiTreeNodeFlags flags);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_Text(string text);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_InputFloat(string title, ref float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_InputFloat2(string title, ref float valueX, ref float valueY);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_InputFloat3(string title, ref float valueX, ref float valueY, ref float valueZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_InputFloat4(string title, out float valueX, out float valueY, out float valueZ, out float valueW);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_InputInt(string title, out int value, int step = 1, int stepFast = 100);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_InputInt2(string title, out int valueX, out int valueY);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_InputInt3(string title, out int valueX, out int valueY, out int valueZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_InputInt4(string title, out int valueX, out int valueY, out int valueZ, out int valueW);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragFloat(string title, out float value, float speed = 1.0f, float min = 0.0f, float max = 0.0f);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragFloat2(string title, out float valueX, out float valueY, float speed = 1.0f, float min = 0.0f, float max = 0.0f);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragFloat3(string title, out float valueX, out float valueY, out float valueZ, float speed = 1.0f, float min = 0.0f, float max = 0.0f);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragFloat4(string title, out float valueX, out float valueY, out float valueZ, out float valueW, float speed = 1.0f, float min = 0.0f, float max = 0.0f);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragInt(string title, out int value, float speed = 1.0f, int min = 0, int max = 0);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragInt2(string title, out int valueX, out int valueY, float speed = 1.0f, int min = 0, int max = 0);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragInt3(string title, out int valueX, out int valueY, out int valueZ, float speed = 1.0f, int min = 0, int max = 0);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragInt4(string title, out int valueX, out int valueY, out int valueZ, out int valueW, float speed = 1.0f, int min = 0, int max = 0);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_SliderFloat(string title, out float value, float min, float max);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_SliderFloat2(string title, out float valueX, out float valueY, float min, float max);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_SliderFloat3(string title, out float valueX, out float valueY, out float valueZ, float min, float max);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_SliderFloat4(string title, out float valueX, out float valueY, out float valueZ, out float valueW, float min, float max);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_SliderInt(string title, out int value, int min, int max);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_SliderInt2(string title, out int valueX, out int valueY, int min, int max);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_SliderInt3(string title, out int valueX, out int valueY, out int valueZ, int min, int max);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_SliderInt4(string title, out int valueX, out int valueY, out int valueZ, out int valueW, int min, int max);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_ColorEdit3(string title, out float r, out float g, out float b);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_ColorEdit4(string title, out float r, out float g, out float b, out float a);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_InvisibleButton(string title, float sizeX, float sizeY);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_BeginDragAndDropTarget();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragAndDropTargetID32(string title, ImGuiDragAndDropType32 dataType, out uint outID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_DragAndDropTargetID64(string title, ImGuiDragAndDropType64 dataType, out ulong outID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_EndDragAndDropTarget();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_Separator();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_SameLine(float offsetFromStart = 0.0f, float spacing = -1.0f);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_Indent(float indentW = 0.0f);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_UnIndent();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_PushStyleColor(ImGuiCol idx, float r, float g, float b, float a);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_PopStyleColor(int count = 1);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_PushStyleVar(ImGuiStyleVar idx, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_PushStyleVarParam2(ImGuiStyleVar idx, float valueX, float valueY);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_PopStyleVar(int count = 1);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_GetContentRegionAvail(out float outX, out float outY);
        #endregion

        #region Timer
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Timer_GetDeltaTime(out float outDeltaTime);
        #endregion

        #region Math
        // Utils
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Math_ApproxEqual(
            float a, float b, out bool outEqual
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Math_ToRadian(float degree, out float radian);

        // Vector3
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static Vector3 Vector3_Dot(
            float lx, float ly, float lz,
            float rx, float ry, float rz,
            out float dot
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static Vector3 Vector3_AngleBetweenNormals(
            float lx, float ly, float lz,
            float rx, float ry, float rz,
            out float angle
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static Vector3 Vector3_Cross(
            float lx, float ly, float lz,
            float rx, float ry, float rz,
            out float outX, out float outY, out float outZ
        );

        // Matrix
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static Matrix Matrix_Transpose(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44,

            ref float r_11, ref float r_12, ref float r_13, ref float r_14,
            ref float r_21, ref float r_22, ref float r_23, ref float r_24,
            ref float r_31, ref float r_32, ref float r_33, ref float r_34,
            ref float r_41, ref float r_42, ref float r_43, ref float r_44
        );
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static Matrix Matrix_Inverse(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44,

            ref float r_11, ref float r_12, ref float r_13, ref float r_14,
            ref float r_21, ref float r_22, ref float r_23, ref float r_24,
            ref float r_31, ref float r_32, ref float r_33, ref float r_34,
            ref float r_41, ref float r_42, ref float r_43, ref float r_44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static object Matrix_Multiply(
            float m1_11, float m1_12, float m1_13, float m1_14,
            float m1_21, float m1_22, float m1_23, float m1_24,
            float m1_31, float m1_32, float m1_33, float m1_34,
            float m1_41, float m1_42, float m1_43, float m1_44,

            float m2_11, float m2_12, float m2_13, float m2_14,
            float m2_21, float m2_22, float m2_23, float m2_24,
            float m2_31, float m2_32, float m2_33, float m2_34,
            float m2_41, float m2_42, float m2_43, float m2_44,

            ref float r_11, ref float r_12, ref float r_13, ref float r_14,
            ref float r_21, ref float r_22, ref float r_23, ref float r_24,
            ref float r_31, ref float r_32, ref float r_33, ref float r_34,
            ref float r_41, ref float r_42, ref float r_43, ref float r_44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Matrix_TransformCoord(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44,

            float inX, float inY, float inZ,
            out float outX, out float outY, out float outZ
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Matrix_TransformNormal(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44,

            float inX, float inY, float inZ,
            out float outX, out float outY, out float outZ
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Matrix_ObtainEulerAngles(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44,

            out float x, out float y, out float z
            );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Matrix_ObtainScale(
           float _11, float _12, float _13, float _14,
           float _21, float _22, float _23, float _24,
           float _31, float _32, float _33, float _34,
           float _41, float _42, float _43, float _44,

           out float x, out float y, out float z
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Matrix_RotationAxis(
           out float _11, out float _12, out float _13, out float _14,
           out float _21, out float _22, out float _23, out float _24,
           out float _31, out float _32, out float _33, out float _34,
           out float _41, out float _42, out float _43, out float _44,

           float axisX, float axisY, float axisZ, float radian
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Matrix_RotationQuaternion(
           out float _11, out float _12, out float _13, out float _14,
           out float _21, out float _22, out float _23, out float _24,
           out float _31, out float _32, out float _33, out float _34,
           out float _41, out float _42, out float _43, out float _44,

           float qX, float qY, float qZ, float qW
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Matrix_RotationRollPitchYaw(
           out float _11, out float _12, out float _13, out float _14,
           out float _21, out float _22, out float _23, out float _24,
           out float _31, out float _32, out float _33, out float _34,
           out float _41, out float _42, out float _43, out float _44,

           float roll, float pitch, float yaw
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Matrix_Scaling(
           out float _11, out float _12, out float _13, out float _14,
           out float _21, out float _22, out float _23, out float _24,
           out float _31, out float _32, out float _33, out float _34,
           out float _41, out float _42, out float _43, out float _44,

           float x, float y, float z
        );

        // Quaternion
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Quaternion_Slerp(
            float q1X, float q1Y, float q1Z, float q1W,
            float q2X, float q2Y, float q2Z, float q2W, float t,
            
            out float qrX, out float qrY, out float qrZ, out float qrW
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Quaternion_RotationAxis(
            float axisX, float axisY, float axisZ, float angle,
            
            out float qrX, out float qrY, out float qrZ, out float qrW
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Quaternion_RollPitchYaw(
            float roll, float pitch, float yaw,
            
            out float qrX, out float qrY, out float qrZ, out float qrW
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Quaternion_ToMatrix(
           float x, float y, float z, float w,

           out float _11, out float _12, out float _13, out float _14,
           out float _21, out float _22, out float _23, out float _24,
           out float _31, out float _32, out float _33, out float _34,
           out float _41, out float _42, out float _43, out float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Quaternion_Multiply(
            float q1X, float q1Y, float q1Z, float q1W,
            float q2X, float q2Y, float q2Z, float q2W,
            
            out float qrX, out float qrY, out float qrZ, out float qrW
        );

        #endregion

        #region Physics
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Physics_GetGravity(out float x, out float y, out float z);
        #endregion

        #region Scene
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Scene_FindGameObjectByID(uint id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static uint Scene_FindGameObjectByTag(string tag);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static uint[] Scene_FindGameObjectsByTag(string tag);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static uint[] Scene_GetGameObjects();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RuntimeSceneManager_ChangeSceneFromIndex(int sceneIndex, bool reloadScene);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RuntimeSceneManager_ChangeSceneFromName(string sceneName, bool reloadScene);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static uint[] EditorSceneManager_FindGameObjectsOfSceneByName(string sceneName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static uint[] EditorSceneManager_FindGameObjectsByTagOfSceneByName(string sceneName, string tag);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string EditorSceneManager_GetActiveSceneName();
        #endregion

        #region GameObject
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameObject_SetName(string name, uint id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string GameObject_GetName(uint id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string GameObject_GetTag(uint id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool GameObject_GetEnabled(uint id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameObject_SetEnabled(bool enabled, uint id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string[] GameObject_GetComponents(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool GameObject_HasComponent(uint entityID, Type componentType);
        #endregion

        #region Component
        // All component.
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Component_SetEnabled(uint entityID, bool enabled, string componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Component_GetEnabled(uint entityID, string componentType);

        // Transform
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetLocalPosition(uint entityID, float localX, float localY, float localZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetPosition(uint entityID, out float x, out float y, out float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetLocalPosition(uint entityID, out float lx, out float ly, out float lz);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetLocalEulerAngles(uint entityID, float localEulerAngleX, float localEulerAngleY, float localEulerAngleZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetLocalEulerAngles(uint entityID, out float outLocalEulerAngleX, out float outLocalEulerAngleY, out float outLocalEulerAngleZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetLocalScale(uint entityID, float localScaleX, float localScaleY, float localScaleZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetLocalScale(uint entityID, out float outLocalScaleX, out float outLocalScaleY, out float outLocalScaleZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRight(uint entityID, out float outRightX, out float outRightY, out float outRightZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetUp(uint entityID, out float outUpX, out float outUpY, out float outUpZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetFront(uint entityID, out float outFrontX, out float outFrontY, out float outFrontZ);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetWorld(uint entityID,
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            out float _41, out float _42, out float _43, out float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetWorld(
            uint entityID,
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetLocal(uint entityID,
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            out float _41, out float _42, out float _43, out float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetLocal(
            uint entityID,
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool TransformComponent_GetUpdate(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetUpdate(uint entityID, bool update);

        // GameCamera
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameCameraComponent_SetFocusPosition(uint entityID, float x, float y, float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameCameraComponent_SetViewProjection(
            uint entityID, 
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameCameraComponent_SetView(
            uint entityID, 
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameCameraComponent_SetProjection(
            uint entityID, 
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameCameraComponent_ComputeView(
            float frontX, float frontY, float frontZ,
            float localPositionX, float localPositionY, float localPositionZ,
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            out float _41, out float _42, out float _43, out float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameCameraComponent_ComputePerspectiveProjection(
            float fovAngleY, float aspectRatio, float nearZ, float farZ,
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            out float _41, out float _42, out float _43, out float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameCameraComponent_ComputeOrthographicProjection(
            float viewSizeX, float viewSizeY,
            float nearZ, float farZ,
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            out float _41, out float _42, out float _43, out float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameCameraComponent_UpdateMainCameraConstants(
            float view_11, float view_12, float view_13, float view_14,
            float view_21, float view_22, float view_23, float view_24,
            float view_31, float view_32, float view_33, float view_34,
            float view_41, float view_42, float view_43, float view_44,

            float projection_11, float projection_12, float projection_13, float projection_14,
            float projection_21, float projection_22, float projection_23, float projection_24,
            float projection_31, float projection_32, float projection_33, float projection_34,
            float projection_41, float projection_42, float projection_43, float projection_44,

            float positionX, float positionY, float positionZ, float positionW
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameCamera_MainCamera_GetOrientation(
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            out float _41, out float _42, out float _43, out float _44
            );

        // RigidBody
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetLinearVelocity(
            uint entityID, float x, float y, float z
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_EnableAngularVelocity(
            uint entityID
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_DisableAngularVelocity(
            uint entityID
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_AddForce(
            uint entityID, float x, float y, float z
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_AddImpulse(
            uint entityID, float x, float y, float z
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_GetLinearVelocity(
            uint entityID, out float x, out float y, out float z
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetGravity(
            uint entityID, float x, float y, float z
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_GetRigidBodyWorld(uint entityID,
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            out float _41, out float _42, out float _43, out float _44
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyComponent_SetPosition(
            uint entityID, float x, float y, float z
        );

        // Dynamic character contorller.
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_AddLinearVelocity(uint entityID, float x, float y, float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_AddTorqueVelocity(uint entityID, float x, float y, float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_Jump(uint entityID, float x, float y, float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_SetGravity(uint entityID, float x, float y, float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_SetMoveDirection(uint entityID, float x, float y, float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_SetJumpDirection(uint entityID, float x, float y, float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_SetWalkSpeed(uint entityID, float speed);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_SetJumpSpeed(uint entityID, float speed);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_GetLinearVelocity(uint entityID, out float x, out float y, out float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_GetTorqueVelocity(uint entityID, out float x, out float y, out float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_GetGravity(uint entityID, out float x, out float y, out float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DynamicCharacterControllerComponent_GetMoveDirection(uint entityID, out float x, out float y, out float z);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float DynamicCharacterControllerComponent_GetWalkSpeed(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float DynamicCharacterControllerComponent_GetJumpSpeed(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool DynamicCharacterControllerComponent_OnGround(uint entityID);

        //
        // VehicleComponent
        //
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetActivate(bool forceActivate, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetVelocity(float x, float y, float z, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_GetVelocity(out float x, out float y, out float z, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetLinearVelocity(float x, float y, float z, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_GetLinearVelocity(out float x, out float y, out float z, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_ApplyCentralImpulse(float x, float y, float z, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_ApplyForce(float x, float y, float z, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetGravity(float x, float y, float z, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetFront(float x, float y, float z, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetOrientation(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            uint entityID
            );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_GetOrientation(
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            uint entityID
            );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetPosition(
            float x, float y, float z, uint entityID
            );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_GetPosition(
            out float x, out float y, out float z, uint entityID
            );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetMotionWorldTransform(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44,
            uint entityID
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_GetMotionWorldTransform(
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            out float _41, out float _42, out float _43, out float _44,
            uint entityID
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetWorldTransform(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44,
            uint entityID
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_GetWorldTransform(
            out float _11, out float _12, out float _13, out float _14,
            out float _21, out float _22, out float _23, out float _24,
            out float _31, out float _32, out float _33, out float _34,
            out float _41, out float _42, out float _43, out float _44,
            uint entityID
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_GetHitWallNormal(
            out float x, out float y, out float z,
            uint entityID
        );

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_Jump(float x, float y, float z, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetJumpFlag(bool flag, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool VehicleComponent_IsJump(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool VehicleComponent_IsDrifting(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_SetDrifting(bool drifting, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void VehicleComponent_ResetDrift(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int VehicleComponent_GetDriftDirection(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float VehicleComponent_GetOnDriftInputLX(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool VehicleComponent_OnGround(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool VehicleComponent_OnGrass(uint entityId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool VehicleComponent_HitWall(uint entityID);

        //
        // Game
        //

        //
        // RaceGame
        //
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int CheckPointComponent_GetDesireEntityPassingCheckPointCount(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int CheckPointComponent_GetNumber(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int CheckPointComponent_GetNextCheckPointNumber(uint entityID);
        #endregion

        #region Component2D
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string UIComponent_GetItemType(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_GetPosition(out float x, out float y, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float UIComponent_GetScale(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float UIComponent_GetRotation(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetPosition(float x, float y, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetScale(float scale, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_SetRotation(float rotation, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool UIComponent_GetRender(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool UIComponent_SetRender(bool render, uint entityID);

        //
        // Text item.
        //
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string UIComponent_Text_GetText(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float UIComponent_Text_GetSpacing(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_Text_GetTextColor(out float r, out float g, out float b, out float a, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_Text_SetText(string text, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_Text_SetSpacing(float spacing, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_Text_SetTextColor(float r, float g, float b, float a, uint entityID);

        //
        // Image item.
        //
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_Image_GetClipRect(out int left, out int top, out int right, out int bottom, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_Image_SetClipRect(int left, int top, int right, int bottom, uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_Image_GetTextureSize(out int x, out int y, uint entityID);
        #endregion

        #region ParticleSystem
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void CPUParticleSystem_Emit(
            float beginSize, float endSize, float sizeVariation,
            float lifeTime,
            float rotation,
            float positionX, float positionY, float positionZ,
            float beginPositionX, float beginPositionY, float beginPositionZ,
            float endPositionX, float endPositionY, float endPositionZ,
            float initialVelocityX, float initialVelocityY, float initialVelocityZ,
            float velocityVariationX, float velocityVariationY, float velocityVariationZ,
            float beginColorR, float beginColorG, float beginColorB, float beginColorA,
            float endColorR, float endColorG, float endColorB, float endColorA,
            bool applyGravityForce, bool useBeginAndEndPosition
        );
        #endregion
    }
}
