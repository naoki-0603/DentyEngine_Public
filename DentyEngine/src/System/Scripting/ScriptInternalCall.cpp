#include "stdafx.h"

#include "ScriptEngine.h"
#include "ScriptInternalCall.h"

#include <mono/metadata/appdomain.h>

#include "ScriptUtils.h"

#include "System/Core/ID/EntityID.h"
#include "System/Core/ID/MaterialID.h"
#include "System/Core/ID/TextureID.h"
#include "System/Core/ID/PrefabID.h"

//
// Components
//
#include "Component/Graphics/Renderer/DirectionalLight.h"
#include "Component/Graphics/Renderer/PointLight.h"
#include "Component/Graphics/Renderer/SpotLight.h"
#include "Component/Graphics/Renderer/MeshRenderer.h"
#include "Component/Graphics/Renderer/SkinnedMeshRenderer.h"

#include "Component/Physics/Colliders/BoxCollider.h"
#include "Component/Physics/Colliders/SphereCollider.h"
#include "Component/Physics/Colliders/CapsuleCollider.h"
#include "Component/Physics/Colliders/MeshCollider.h"
#include "Component/Physics/Colliders/ConvexMeshCollider.h"
#include "Component/Physics/Colliders/ApproximationMeshCollider.h"
#include "Component/Physics/RigidBody.h"
#include "Component/Physics/DynamicCharacterController.h"

#include "Component/Animations/Animator.h"

#include "System/Scene/Scene.h"

#include "System/Core/RuntimeSceneManager.h"
#include "System/Core/Timer.h"

#include "Component/Graphics/Renderer/GameCamera.h"

#include "Component/Physics/Vehicle/VehicleComponent.h"

#include "Component/Game/Race/CheckPoint.h"

#include "Component2D/UI/UIComponent.h"

#include <mono/metadata/loader.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/class.h>

#include "Physics/Vehicle/SimplifiedCarAction.h"

#include "System/Editor/EditorSceneManager.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/SceneLibrary.h"

#include "System/Core/Setting/SceneSetting.h"

#include "System/Graphics/Renderer/CPUParticleSystem.h"

namespace DentyEngine
{
    class GameObject;

    ScriptInternalCall::ScriptInternalCall()
    {
    }

    //
    // Internal call functions.
    //

    // Input
#pragma region Input

    //
    // Keyboard
    //
    static bool Input_IsKeyPressed(KeyCode key)
    {
        return Input::IsKeyPressed(key);
    }

    static bool Input_IsKeyTriggered(KeyCode key)
    {
        return Input::IsKeyTriggered(key);
    }

    static bool Input_IsKeyReleased(KeyCode key)
    {
        return Input::IsKeyReleased(key);
    }

    //
    // Mouse
    //
    static bool Input_IsMousePressed(MouseButton button)
    {
        return Input::IsMousePressed(button);
    }

    static bool Input_IsMouseTriggered(MouseButton button)
    {
        return Input::IsMouseTriggered(button);
    }

    static bool Input_IsMouseReleased(MouseButton button)
    {
        return Input::IsMouseReleased(button);
    }

    static void Input_SetMousePosition(int32_t x, int32_t y)
    {
        Input::SetMousePosition(Vector2I(x, y));
    }

    static void Input_GetMouseWheelSpeed(float* outWheelSpeed)
    {
        const float wheelSpeed = Input::GetMouseWheelSpeed();

        *outWheelSpeed = wheelSpeed;
    }

    static void Input_GetMousePanX(float* outPanX)
    {
        const float panX = Input::GetMousePanX();

        *outPanX = panX;
    }

    static void Input_GetMousePanY(float* outPanY)
    {
        const float panY = Input::GetMousePanY();

        *outPanY = panY;
    }

    static void Input_GetMousePosition(int* outX, int* outY)
    {
        const Vector2I position = Input::GetMousePosition();

        *outX = position.x;
        *outY = position.y;
    }

    //
    // GamePad
    //
    static bool Input_IsPadPressed(PadKeyCode padKeyCode, uint32_t padNum = 0u)
    {
        return Input::IsPadPressed(padKeyCode, padNum);
    }

    static bool Input_IsPadTriggered(PadKeyCode padKeyCode, uint32_t padNum = 0u)
    {
        return Input::IsPadTriggered(padKeyCode, padNum);
    }

    static bool Input_IsPadReleased(PadKeyCode padKeyCode, uint32_t padNum = 0u)
    {
        return Input::IsPadReleased(padKeyCode, padNum);
    }

    static bool Input_IsPadLeftTriggerTriggered(uint32_t padNum = 0u)
    {
        return Input::IsPadLeftTriggerTriggered(padNum);
    }

    static bool Input_IsPadRightTriggerTriggered(uint32_t padNum = 0u)
    {
        return Input::IsPadRightTriggerTriggered(padNum);
    }

    static bool Input_IsPadConnectedAt(uint32_t padNum)
    {
        return Input::IsPadConnectedAt(padNum);
    }

    static uint8_t Input_GetLeftTrigger(uint32_t padNum = 0)
    {
        return Input::GetLeftTrigger(padNum);
    }

    static uint8_t Input_GetRightTrigger(uint32_t padNum = 0)
    {
        return Input::GetRightTrigger(padNum);
    }

    static int16_t Input_GetThumbLX(uint32_t padNum = 0)
    {
        return Input::GetThumbLX(padNum);
    }

    static int16_t Input_GetThumbLY(uint32_t padNum = 0)
    {
        return Input::GetThumbLY(padNum);
    }

    static int16_t Input_GetThumbRX(uint32_t padNum = 0)
    {
        return Input::GetThumbRX(padNum);
    }

    static int16_t Input_GetThumbRY(uint32_t padNum = 0)
    {
        return Input::GetThumbRY(padNum);
    }

    static float Input_GetNormalizedThumbLX(uint32_t padNum = 0)
    {
        return Input::GetNormalizedThumbLX(padNum);
    }

    static float Input_GetNormalizedThumbLY(uint32_t padNum = 0)
    {
        return Input::GetNormalizedThumbLY(padNum);
    }

    static float Input_GetNormalizedThumbRX(uint32_t padNum = 0)
    {
        return Input::GetNormalizedThumbRX(padNum);
    }

    static float Input_GetNormalizedThumbRY(uint32_t padNum = 0)
    {
        return Input::GetNormalizedThumbRY(padNum);
    }
#pragma endregion

#pragma region Debug
    static void Debug_Log(MonoString* monoString)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        DENTY_DEBUG_CONSOLE_LOG(string.c_str());
    }

#pragma endregion

#pragma region ImGui 
    static bool ImGui_CollapsingHeader(MonoString* monoString, ImGuiTreeNodeFlags flags)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        return ImGui::CollapsingHeader(string.c_str(), flags);
    }

    static void ImGui_Text(MonoString* monoString)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        ImGui::Text(string.c_str());
    }

    static bool ImGui_InputFloat(MonoString* monoString, float* value)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        return ImGui::InputFloat(string.c_str(), value);
    }

    static bool ImGui_InputFloat2(MonoString* monoString, float* valueX, float* valueY)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY };
        const bool flag = ImGui::InputFloat2(string.c_str(), values.data());

        *valueX = values.at(0);
        *valueY = values.at(1);

        return flag;
    }

    static bool ImGui_InputFloat3(MonoString* monoString, float* valueX, float* valueY, float* valueZ)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ };
        const bool flag = ImGui::InputFloat3(string.c_str(), values.data());

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);

        return flag;
    }

    static bool ImGui_InputFloat4(MonoString* monoString, float* valueX, float* valueY, float* valueZ, float* valueW)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ, *valueW };
        const bool flag = ImGui::InputFloat3(string.c_str(), values.data());

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);
        *valueW = values.at(3);

        return flag;
    }

    static bool ImGui_InputInt(MonoString* monoString, int32_t* value, int32_t step = 1, int32_t stepFast = 100)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        return ImGui::InputInt(string.c_str(), value, step, stepFast);
    }

    static bool ImGui_InputInt2(MonoString* monoString, int32_t* valueX, int32_t* valueY)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY };
        const bool flag = ImGui::InputInt2(string.c_str(), values.data());

        *valueX = values.at(0);
        *valueY = values.at(1);

        return flag;
    }

    static bool ImGui_InputInt3(MonoString* monoString, int32_t* valueX, int32_t* valueY, int32_t* valueZ)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ };
        const bool flag = ImGui::InputInt3(string.c_str(), values.data());

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);

        return flag;
    }

    static bool ImGui_InputInt4(MonoString* monoString, int32_t* valueX, int32_t* valueY, int32_t* valueZ, int32_t* valueW)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ, *valueW };
        const bool flag = ImGui::InputInt4(string.c_str(), values.data());

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);
        *valueW = values.at(3);

        return flag;
    }

    static bool ImGui_DragFloat(MonoString* monoString, float* value, float speed = 1.0f, float min = 0.0f, float max = 0.0f)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        return ImGui::DragFloat(string.c_str(), value, speed, min, max);
    }

    static bool ImGui_DragFloat2(MonoString* monoString, float* valueX, float* valueY, float speed = 1.0f, float min = 0.0f, float max = 0.0f)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY };
        const bool flag = ImGui::DragFloat2(string.c_str(), values.data(), speed, min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);

        return flag;
    }

    static bool ImGui_DragFloat3(MonoString* monoString, float* valueX, float* valueY, float* valueZ, float speed = 1.0f, float min = 0.0f, float max = 0.0f)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ };
        const bool flag = ImGui::DragFloat3(string.c_str(), values.data(), speed, min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);

        return flag;
    }

    static bool ImGui_DragFloat4(MonoString* monoString, float* valueX, float* valueY, float* valueZ, float* valueW, float speed = 1.0f, float min = 0.0f, float max = 0.0f)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ, *valueW };
        const bool flag = ImGui::DragFloat4(string.c_str(), values.data(), speed, min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);
        *valueW = values.at(3);

        return flag;
    }

    static bool ImGui_DragInt(MonoString* monoString, int32_t* value, float speed = 1.0f, int32_t min = 0.0f, int32_t max = 0.0f)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        return ImGui::DragInt(string.c_str(), value, speed, min, max);
    }

    static bool ImGui_DragInt2(MonoString* monoString, int32_t* valueX, int32_t* valueY, float speed = 1.0f, int32_t min = 0.0f, int32_t max = 0.0f)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY };
        const bool flag = ImGui::DragInt2(string.c_str(), values.data(), speed, min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);

        return flag;
    }

    static bool ImGui_DragInt3(MonoString* monoString, int32_t* valueX, int32_t* valueY, int32_t* valueZ, float speed = 1.0f, int32_t min = 0.0f, int32_t max = 0.0f)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ };
        const bool flag = ImGui::DragInt3(string.c_str(), values.data(), speed, min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);

        return flag;
    }

    static bool ImGui_DragInt4(MonoString* monoString, int32_t* valueX, int32_t* valueY, int32_t* valueZ, int32_t* valueW, float speed = 1.0f, int32_t min = 0.0f, int32_t max = 0.0f)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ, *valueW };
        const bool flag = ImGui::DragInt4(string.c_str(), values.data(), speed, min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);
        *valueW = values.at(3);

        return flag;
    }

    static bool ImGui_SliderFloat(MonoString* monoString, float* value, float min, float max)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        return ImGui::SliderFloat(string.c_str(), value, min, max);
    }

    static bool ImGui_SliderFloat2(MonoString* monoString, float* valueX, float* valueY, float min, float max)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY };
        const bool flag = ImGui::SliderFloat2(string.c_str(), values.data(), min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);

        return flag;
    }

    static bool ImGui_SliderFloat3(MonoString* monoString, float* valueX, float* valueY, float* valueZ, float min, float max)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ };
        const bool flag = ImGui::SliderFloat3(string.c_str(), values.data(), min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);

        return flag;
    }

    static bool ImGui_SliderFloat4(MonoString* monoString, float* valueX, float* valueY, float* valueZ, float* valueW, float min, float max)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ, *valueW };
        const bool flag = ImGui::SliderFloat4(string.c_str(), values.data(), min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);
        *valueW = values.at(3);

        return flag;
    }

    static bool ImGui_SliderInt(MonoString* monoString, int32_t* value, int32_t min, int32_t max)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        return ImGui::SliderInt(string.c_str(), value, min, max);
    }

    static bool ImGui_SliderInt2(MonoString* monoString, int32_t* valueX, int32_t* valueY, int32_t min, int32_t max)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY };
        const bool flag = ImGui::SliderInt2(string.c_str(), values.data(), min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);

        return flag;
    }

    static bool ImGui_SliderInt3(MonoString* monoString, int32_t* valueX, int32_t* valueY, int32_t* valueZ, int32_t min, int32_t max)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ };
        const bool flag = ImGui::SliderInt3(string.c_str(), values.data(), min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);

        return flag;
    }

    static bool ImGui_SliderInt4(MonoString* monoString, int32_t* valueX, int32_t* valueY, int32_t* valueZ, int32_t* valueW, int32_t min, int32_t max)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array values = { *valueX, *valueY, *valueZ, *valueW };
        const bool flag = ImGui::SliderInt4(string.c_str(), values.data(), min, max);

        *valueX = values.at(0);
        *valueY = values.at(1);
        *valueZ = values.at(2);
        *valueW = values.at(3);

        return flag;
    }

    static bool ImGui_ColorEdit3(MonoString* monoString, float* r, float* g, float* b)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array colors = { *r, *g, *b };
        const bool flag = ImGui::ColorEdit3(string.c_str(), colors.data());

        *r = colors.at(0);
        *g = colors.at(1);
        *b = colors.at(2);

        return flag;
    }

    static bool ImGui_ColorEdit4(MonoString* monoString, float* r, float* g, float* b, float* a)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        std::array colors = { *r, *g, *b, *a };

        const bool flag = ImGui::ColorEdit4(string.c_str(), colors.data());

        *r = colors.at(0);
        *g = colors.at(1);
        *b = colors.at(2);
        *a = colors.at(3);

        return flag;
    }

    static bool ImGui_InvisibleButton(MonoString* monoString, float sizeX, float sizeY)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        return ImGui::InvisibleButton(string.c_str(), ImVec2(sizeX, sizeY));
    }

    static bool ImGui_BeginDragAndDropTarget()
    {
        return ImGui::BeginDragDropTarget();
    }

    static bool ImGui_DragAndDropTargetID32(MonoString* monoString, int32_t dataType, uint32_t* outID)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(string.c_str()))
        {
	        switch (dataType)
	        {
                // Material id
	        case 0:
		        {
					*outID = *static_cast<MaterialID*>(payload->Data);
		        }
                break;
                // Sprite
	        case 13:
                break;
                // GameObject
	        case 100:
		        {
					*outID = *static_cast<EntityID*>(payload->Data);
		        }
                break;
	        default:
                DENTY_ERR_CONSOLE_LOG("Invalid data type! (value: %u, in ImGui_DragAndDropTargetID32)", dataType);
                return false;
	        }

            return true;
        }

        return false;
    }

    static bool ImGui_DragAndDropTargetID64(MonoString* monoString, int32_t dataType, uint64_t* outID)
    {
        const std::string string = Utils::MonoStringToUTF8(monoString);

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(string.c_str()))
        {
            switch (dataType)
            {
                // Texture id
            case 5:
	            {
					*outID = *static_cast<TextureID*>(payload->Data);
	            }
                break;
                // Prefab id
            case 6:
				{
					*outID = *static_cast<PrefabID*>(payload->Data);
				}
                break;
            default:
                DENTY_ERR_CONSOLE_LOG("Invalid data type! (value: %u, in ImGui_DragAndDropTargetID64)", dataType);
                return false;
            }

            return true;
        }

        return false;
    }

    static void ImGui_EndDragAndDropTarget()
    {
        ImGui::EndDragDropTarget();
    }

    static void ImGui_Separator()
    {
        ImGui::Separator();
    }

    static void ImGui_SameLine(float offsetFromStart = 0.0f, float spacing = -1.0f)
    {
        ImGui::SameLine(offsetFromStart, spacing);
    }

    static void ImGui_Indent(float indentW = 0.0f)
    {
        ImGui::Indent(indentW);
    }

    static void ImGui_UnIndent()
    {
        ImGui::Unindent();
    }

    static void ImGui_PushStyleColor(int32_t idx, float r, float g, float b, float a)
    {
        ImGui::PushStyleColor(idx, ImVec4(r, g, b, a));
    }

    static void ImGui_PopStyleColor(int32_t count = 1)
    {
        ImGui::PopStyleColor(count);
    }

    static void ImGui_PushStyleVar(int32_t idx, float value)
    {
        ImGui::PushStyleVar(idx, value);
    }

    static void ImGui_PushStyleVarParam2(int32_t idx, float valueX, float valueY)
    {
        ImGui::PushStyleVar(idx, ImVec2(valueX, valueY));
    }

    static void ImGui_PopStyleVar(int32_t count = 1)
    {
        ImGui::PopStyleVar(count);
    }

    static void ImGui_GetContentRegionAvail(float* outX, float *outY)
    {
        const Vector2 regionAvail = Vector2::ToVector2(ImGui::GetContentRegionAvail());

        *outX = regionAvail.x;
        *outY = regionAvail.y;
    }
#pragma endregion

#pragma region Timer
    static void Timer_GetDeltaTime(float* outDeltaTime)
    {
        *outDeltaTime = Timer::GetDeltaTime();
    }
#pragma endregion

#pragma region Math

    // Utils
    static void Math_ApproxEqual(
        float a, float b, bool* outBool
    )
    {
        *outBool = Utils::ApproxEqual(a, b);
    }

    static void Math_ToRadian(
        float degree, float* outRadian
    )
    {
        *outRadian = DirectX::XMConvertToRadians(degree);
    }

    // Vector3
    static void Vector3_Dot(
		float lx, float ly, float lz,
        float rx, float ry, float rz,

        float* out
    )
    {
        Vector3 l = { lx, ly, lz };
        Vector3 r = { rx, ry, rz };

        Vector3 result = DirectX::XMVector3Dot(l.GetXM(), r.GetXM());

        *out = result.x;
    }

    static void Vector3_AngleBetweenNormals(
        float lx, float ly, float lz,
        float rx, float ry, float rz,

        float* out
    )
    {
        Vector3 l = { lx, ly, lz };
        Vector3 r = { rx, ry, rz };

        Vector3 result = DirectX::XMVector3AngleBetweenNormals(l.GetXM(), r.GetXM());

        *out = result.x;
    }

    static void Vector3_Cross(
        float lx, float ly, float lz,
        float rx, float ry, float rz,
        float* outX, float* outY, float* outZ
    )
    {
        Vector3 l = { lx, ly, lz };
        Vector3 r = { rx, ry, rz };

        Vector3 result = DirectX::XMVector3Cross(l.GetXM(), r.GetXM());

        *outX = result.x;
        *outY = result.y;
        *outZ = result.z;
    }

    // Matrix
    static void Matrix_Transpose(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44,

        float* r_11, float* r_12, float* r_13, float* r_14,
        float* r_21, float* r_22, float* r_23, float* r_24,
        float* r_31, float* r_32, float* r_33, float* r_34,
        float* r_41, float* r_42, float* r_43, float* r_44
    )
    {
        const auto m1 = Matrix(
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        );

        const Matrix r = m1.Transpose();

        *r_11 = r._11; *r_12 = r._12; *r_13 = r._13; *r_14 = r._14;
        *r_21 = r._21; *r_22 = r._22; *r_23 = r._23; *r_24 = r._24;
        *r_31 = r._31; *r_32 = r._32; *r_33 = r._33; *r_34 = r._34;
        *r_41 = r._41; *r_42 = r._42; *r_43 = r._43; *r_44 = r._44;
    }

    static void Matrix_Inverse(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44,

        float* r_11, float* r_12, float* r_13, float* r_14,
        float* r_21, float* r_22, float* r_23, float* r_24,
        float* r_31, float* r_32, float* r_33, float* r_34,
        float* r_41, float* r_42, float* r_43, float* r_44)
    {
        const auto m1 = Matrix(
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        );

        const Matrix r = m1.Inverse();

        *r_11 = r._11; *r_12 = r._12; *r_13 = r._13; *r_14 = r._14;
        *r_21 = r._21; *r_22 = r._22; *r_23 = r._23; *r_24 = r._24;
        *r_31 = r._31; *r_32 = r._32; *r_33 = r._33; *r_34 = r._34;
        *r_41 = r._41; *r_42 = r._42; *r_43 = r._43; *r_44 = r._44;
    }

    static void Matrix_Multiply(
        float m1_11, float m1_12, float m1_13, float m1_14,
        float m1_21, float m1_22, float m1_23, float m1_24,
        float m1_31, float m1_32, float m1_33, float m1_34,
        float m1_41, float m1_42, float m1_43, float m1_44,

        float m2_11, float m2_12, float m2_13, float m2_14,
        float m2_21, float m2_22, float m2_23, float m2_24,
        float m2_31, float m2_32, float m2_33, float m2_34,
        float m2_41, float m2_42, float m2_43, float m2_44,

        float* r_11, float* r_12, float* r_13, float* r_14,
        float* r_21, float* r_22, float* r_23, float* r_24,
        float* r_31, float* r_32, float* r_33, float* r_34,
        float* r_41, float* r_42, float* r_43, float* r_44
    )
    {
	    const auto m1 = Matrix(
            m1_11, m1_12, m1_13, m1_14,
            m1_21, m1_22, m1_23, m1_24,
            m1_31, m1_32, m1_33, m1_34,
            m1_41, m1_42, m1_43, m1_44
        );

	    const auto m2 = Matrix(
            m2_11, m2_12, m2_13, m2_14,
            m2_21, m2_22, m2_23, m2_24,
            m2_31, m2_32, m2_33, m2_34,
            m2_41, m2_42, m2_43, m2_44
        );

        Matrix r = DirectX::XMMatrixMultiply(m1.GetXM(), m2.GetXM());

        *r_11 = r._11; *r_12 = r._12; *r_13 = r._13; *r_14 = r._14;
        *r_21 = r._21; *r_22 = r._22; *r_23 = r._23; *r_24 = r._24;
        *r_31 = r._31; *r_32 = r._32; *r_33 = r._33; *r_34 = r._34;
        *r_41 = r._41; *r_42 = r._42; *r_43 = r._43; *r_44 = r._44;
    }

    static void Matrix_RotationAxis(
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44,

        float axisX, float axisY, float axisZ, float radian
    )
    {
        const Vector3 axis = { axisX, axisY, axisZ };

        const Matrix r = DirectX::XMMatrixRotationAxis(axis.GetXM(), radian);

        *_11 = r._11; *_12 = r._12; *_13 = r._13; *_14 = r._14;
        *_21 = r._21; *_22 = r._22; *_23 = r._23; *_24 = r._24;
        *_31 = r._31; *_32 = r._32; *_33 = r._33; *_34 = r._34;
        *_41 = r._41; *_42 = r._42; *_43 = r._43; *_44 = r._44;
    }

    static void Matrix_RotationQuaternion(
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44,

        float qX, float qY, float qZ, float qW
    )
    {
        const Quaternion q = { qX, qY, qZ, qW };
        const Matrix r = DirectX::XMMatrixRotationQuaternion(q.GetXM());

        *_11 = r._11; *_12 = r._12; *_13 = r._13; *_14 = r._14;
        *_21 = r._21; *_22 = r._22; *_23 = r._23; *_24 = r._24;
        *_31 = r._31; *_32 = r._32; *_33 = r._33; *_34 = r._34;
        *_41 = r._41; *_42 = r._42; *_43 = r._43; *_44 = r._44;
    }

    static void Matrix_RotationRollPitchYaw(
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44,

        float roll, float pitch, float yaw
    )
    {
        const Matrix r = DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw);

        *_11 = r._11; *_12 = r._12; *_13 = r._13; *_14 = r._14;
        *_21 = r._21; *_22 = r._22; *_23 = r._23; *_24 = r._24;
        *_31 = r._31; *_32 = r._32; *_33 = r._33; *_34 = r._34;
        *_41 = r._41; *_42 = r._42; *_43 = r._43; *_44 = r._44;
    }

    static void Matrix_Scaling(
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44,

        float x, float y, float z
    )
    {
        const Matrix r = DirectX::XMMatrixScaling(x, y, z);

        *_11 = r._11; *_12 = r._12; *_13 = r._13; *_14 = r._14;
        *_21 = r._21; *_22 = r._22; *_23 = r._23; *_24 = r._24;
        *_31 = r._31; *_32 = r._32; *_33 = r._33; *_34 = r._34;
        *_41 = r._41; *_42 = r._42; *_43 = r._43; *_44 = r._44;
    }
    static void Matrix_TransformCoord(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44,

        float inX, float inY, float inZ,
        float* outX, float* outY, float* outZ
    )
    {
        const auto m1 = Matrix(
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        );

        Vector3 r = { inX, inY, inZ };
        r = m1.TransformCoord(r);

        *outX = r.x;
        *outY = r.y;
        *outZ = r.z;
    }

    static void Matrix_TransformNormal(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44,

        float inX, float inY, float inZ,
        float* outX, float* outY, float* outZ
    )
    {
        const auto m1 = Matrix(
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        );

        Vector3 r = { inX, inY, inZ };
        r = DirectX::XMVector3TransformNormal(r.GetXM(), m1.GetXM());

        *outX = r.x;
        *outY = r.y;
        *outZ = r.z;
    }

    static void Matrix_ObtainEulerAngles(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44,

        float* outX, float* outY, float* outZ
    )
    {
        const auto m1 = Matrix(
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        );

        const Vector3 eulerAngles = m1.ObtainEulerAngles();
        *outX = eulerAngles.x;
        *outY = eulerAngles.y;
        *outZ = eulerAngles.z;
    }

    static void Matrix_ObtainScale(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44,

        float* outX, float* outY, float* outZ
    )
    {
        const auto m1 = Matrix(
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        );

        const Vector3 scale = m1.ObtainScale();
        *outX = scale.x;
        *outY = scale.y;
        *outZ = scale.z;
    }

    // Quaternion
    static void Quaternion_Slerp(
        float q1X, float q1Y, float q1Z, float q1W,
        float q2X, float q2Y, float q2Z, float q2W, float t,

        float* qrX, float* qrY, float* qrZ, float* qrW
    )
    {
        const auto q1 = Quaternion(q1X, q1Y, q1Z, q1W);
        const auto q2 = Quaternion(q2X, q2Y, q2Z, q2W);

        const Quaternion result = DirectX::XMQuaternionSlerp(q1.GetXM(), q2.GetXM(), t);

        *qrX = result.x;
        *qrY = result.y;
        *qrZ = result.z;
        *qrW = result.w;
    }

    static void Quaternion_RotationAxis(
        float axisX, float axisY, float axisZ, float angle,

        float* qrX, float* qrY, float* qrZ, float* qrW
    )
    {
        const Quaternion result = DirectX::XMQuaternionRotationAxis(
            Vector3(axisX, axisY, axisZ).GetXM(), angle
        );

        *qrX = result.x;
        *qrY = result.y;
        *qrZ = result.z;
        *qrW = result.w;
    }

    static void Quaternion_RotationRollPitchYaw(
        float roll, float pitch, float yaw,

        float* qrX, float* qrY, float* qrZ, float* qrW
    )
    {
        const Quaternion result = DirectX::XMQuaternionRotationRollPitchYaw(
            roll, pitch, yaw
        );

        *qrX = result.x;
        *qrY = result.y;
        *qrZ = result.z;
        *qrW = result.w;
    }

    static void Quaternion_ToMatrix(
        float x, float y, float z, float w,

        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44
    )
    {
        const auto q = Quaternion(x, y, z, w);

        const Matrix m = q.ConvertToMatrix();

        *_11 = m._11; *_12 = m._12; *_13 = m._13; *_14 = m._14;
        *_21 = m._21; *_22 = m._22; *_23 = m._23; *_24 = m._24;
        *_31 = m._31; *_32 = m._32; *_33 = m._33; *_34 = m._34;
        *_41 = m._41; *_42 = m._42; *_43 = m._43; *_44 = m._44;
    }

    static void Quaternion_Multiply(
        float q1X, float q1Y, float q1Z, float q1W,
        float q2X, float q2Y, float q2Z, float q2W,

        float* qrX, float* qrY, float* qrZ, float* qrW
    )
    {
        const auto q1 = Quaternion(q1X, q1Y, q1Z, q1W);
        const auto q2 = Quaternion(q2X, q2Y, q2Z, q2W);

        const Quaternion result = DirectX::XMQuaternionMultiply(q1.GetXM(), q2.GetXM());

        *qrX = result.x;
        *qrY = result.y;
        *qrZ = result.z;
        *qrW = result.w;
    }

#pragma endregion Math

#pragma region Physics
    static void Physics_GetGravity(float* x, float* y, float* z)
    {
        const Ref<Scene> activeScene = EditorSceneManager::GetActiveScene();
        DENTY_CHECK_NULL(activeScene)

        const SceneSetting& sceneSetting = activeScene->GetSceneSetting();
        const Vector3& gravity = sceneSetting.physics.gravity;

        *x = gravity.x;
        *y = gravity.y;
        *z = gravity.z;
    }
#pragma endregion Physics 

#pragma region Scene
    static bool Scene_FindGameObjectByID(uint32_t id)
    {
	    const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in Scene_FindGameObjectByID)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);

        return gameObject.has_value();
    }

    static EntityID Scene_FindGameObjectByTag(MonoString* tag)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in Scene_FindGameObjectsByTag)");

        const std::string tagString = Utils::MonoStringToUTF8(tag);

        const std::optional<Ref<GameObject>> gameObject = scene->FindByTag(tagString);
        if (gameObject.has_value())
        {
            return gameObject.value()->GetEntityID();
        }

        return 0u;
    }

    static MonoArray* Scene_FindGameObjectsByTag(MonoString* tag)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in Scene_FindGameObjectsByTag)");

        const std::string tagString = Utils::MonoStringToUTF8(tag);

    	std::list<Ref<GameObject>> gameObjects = scene->FindGameObjectsByTag(tagString);
        if (gameObjects.empty())
            return nullptr;

        std::vector<EntityID> ids = { };
        for (const auto& gameObject : gameObjects)
        {
            ids.emplace_back(gameObject->GetEntityID());
        }

        MonoArray* monoArray = mono_array_new(ScriptEngine::GetAppDomain(), mono_get_uint32_class(), ids.size());
        for (uint32_t index = 0u; index < ids.size(); ++index)
        {
            mono_array_set(monoArray, EntityID, index, ids.at(index));
        }

        return monoArray;
    }

    static MonoArray* Scene_GetGameObjects()
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in Scene_GetGameObjects)");

        const std::list<Ref<GameObject>> gameObjects = scene->GetGameObjects();

    	std::vector<EntityID> ids = {};
        for (const auto& gameObject : gameObjects)
        {
            ids.emplace_back(gameObject->GetEntityID());
        }

        //MonoClass* monoClass = ScriptClass::GetClassFromImage("GameObject", true);
        MonoArray* monoArray = mono_array_new(ScriptEngine::GetAppDomain(), mono_get_uint32_class(), ids.size());

        for (uint32_t index = 0u; index < ids.size(); ++index)
        {
            mono_array_set(monoArray, EntityID, index, ids.at(index));
        }

        return monoArray;
    }

    static void RuntimeSceneManager_ChangeSceneFromIndex(int32_t sceneIndex, bool reloadScene)
    {
        RuntimeSceneManager::ChangeSceneFromIndex(sceneIndex, reloadScene);
    }

    static void RuntimeSceneManager_ChangeSceneFromName(MonoString* sceneName, bool reloadScene)
    {
        const std::string name = Utils::MonoStringToUTF8(sceneName);

        RuntimeSceneManager::ChangeSceneFromName(name, reloadScene);
    }

    static MonoArray* EditorSceneManager_FindGameObjectsOfSceneByName(MonoString* sceneName)
    {
        const std::string name = Utils::MonoStringToUTF8(sceneName);

        const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

        const std::optional<Ref<Scene>> scene = sceneLibrary->FindByName(name);
        if (not scene.has_value())
        {
            DENTY_ERR_CONSOLE_LOG("Scene %s is not found!", name.c_str());

            return nullptr;
        }

        const std::list<Ref<GameObject>> gameObjects = scene.value()->GetGameObjects();

    	std::vector<EntityID> ids = { };
        for (auto&& gameObject : gameObjects)
        {
            ids.emplace_back(gameObject->GetEntityID());
        }

        MonoArray* monoArray = mono_array_new(ScriptEngine::GetAppDomain(), mono_get_uint32_class(), ids.size());
        for (uint32_t index = 0u; index < ids.size(); ++index)
        {
            mono_array_set(monoArray, EntityID, index, ids.at(index));
        }

        return monoArray;
    }

    static MonoArray* EditorSceneManager_FindGameObjectsByTagOfSceneByName(MonoString* sceneName, MonoString* objectTag)
    {
        const std::string name = Utils::MonoStringToUTF8(sceneName);
        const std::string tag = Utils::MonoStringToUTF8(objectTag);

        const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

        const std::optional<Ref<Scene>> scene = sceneLibrary->FindByName(name);
        if (not scene.has_value())
        {
            DENTY_ERR_CONSOLE_LOG("Scene %s is not found!", name.c_str());

            return nullptr;
        }

        const std::list<Ref<GameObject>> gameObjects = scene.value()->FindGameObjectsByTag(tag);

        std::vector<EntityID> ids = { };
        for (auto&& gameObject : gameObjects)
        {
            ids.emplace_back(gameObject->GetEntityID());
        }

        MonoArray* monoArray = mono_array_new(ScriptEngine::GetAppDomain(), mono_get_uint32_class(), ids.size());
        for (uint32_t index = 0u; index < ids.size(); ++index)
        {
            mono_array_set(monoArray, EntityID, index, ids.at(index));
        }

        return monoArray;
    }

    static MonoString* EditorSceneManager_GetActiveSceneName()
    {
        if (EditorSceneManager::HasActiveScene())
        {
            const std::string name = EditorSceneManager::GetActiveScene()->GetName();

            return Utils::CreateMonoString(ScriptEngine::GetAppDomain(), name.c_str());
        }

        return Utils::CreateMonoString(ScriptEngine::GetAppDomain(), "NoActiveScene");
    }
#pragma endregion

#pragma region GameObject
    static MonoString* GameObject_GetName(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameObject_GetName)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameObject_GetName)");

        const std::string& name = gameObject.value()->GetName();
    	MonoString* monoString = Utils::CreateMonoString(ScriptEngine::GetAppDomain(), name.c_str());

    	return monoString;
    }

    static void GameObject_SetName(MonoString* string, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameObject_SetName)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameObject_SetName)");

        const std::string name = Utils::MonoStringToUTF8(string);
        gameObject.value()->SetName(name);
    }

    static MonoString* GameObject_GetTag(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameObject_GetTag)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameObject_GetTag)");

        MonoString* monoString = Utils::CreateMonoString(ScriptEngine::GetAppDomain(), gameObject.value()->tag().c_str());
        return monoString;
    }

    static bool GameObject_GetEnabled(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameObject_GetEnabled)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameObject_GetEnabled)");

        return gameObject.value()->Enabled();
    }

    static void GameObject_SetEnabled(bool enabled, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameObject_SetEnabled)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameObject_SetEnabled)");

        gameObject.value()->SetEnabled(enabled);
    }

    static MonoArray* GameObject_GetComponents(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameObject_GetComponents)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameObject_GetComponents)");

        std::vector<MonoString*> componentNames = {};
        for (const auto& component : gameObject.value()->GetComponents())
        {
            if (component->GetName() == "ScriptComponent")
                continue;

            MonoString* componentNameMonoString = Utils::CreateMonoString(ScriptEngine::GetAppDomain(), component->GetName().c_str());

            componentNames.emplace_back(componentNameMonoString);
        }

        if (componentNames.empty())
            return nullptr;

        const auto size = static_cast<int32_t>(componentNames.size());
        MonoArray* monoArray = mono_array_new(ScriptEngine::GetAppDomain(), mono_get_string_class(), size);

        for (int32_t index = 0u; index < componentNames.size(); ++index)
        {
            mono_array_set(monoArray, MonoString*, index, componentNames.at(index));
        }

        return monoArray;
    }

    static bool GameObject_HasComponent(EntityID entityID, MonoReflectionType* componentType)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameObject_HasComponent)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(entityID);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameObject_HasComponent)");

        MonoType* managedType = mono_reflection_type_get_type(componentType);
        const std::string managedTypeString = mono_type_get_name(managedType);
        const std::string convertedName = String(managedTypeString).SubStr(12u).Get();

        return gameObject.value()->HasComponent(convertedName);
    }
#pragma endregion

#pragma region Component
    // All component
    static void Component_SetEnabled(uint32_t id, bool enabled, MonoString* componentType)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in Component_SetEnabled)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in Component_SetEnabled)");

        const std::string convertedName = Utils::MonoStringToUTF8(componentType);

        if (convertedName == std::string("Transform")) gameObject.value()->transform.SetEnabled(enabled);
        if (convertedName == std::string("DirectionalLight")) gameObject.value()->GetComponent<DirectionalLight>()->SetEnabled(enabled);
        if (convertedName == std::string("PointLight")) gameObject.value()->GetComponent<PointLight>()->SetEnabled(enabled);
        if (convertedName == std::string("SpotLight")) gameObject.value()->GetComponent<SpotLight>()->SetEnabled(enabled);
        if (convertedName == std::string("MeshRenderer")) gameObject.value()->GetComponent<MeshRenderer>()->SetEnabled(enabled);
        if (convertedName == std::string("SkinnedMeshRenderer")) gameObject.value()->GetComponent<SkinnedMeshRenderer>()->SetEnabled(enabled);
        if (convertedName == std::string("BoxCollider")) gameObject.value()->GetComponent<BoxCollider>()->SetEnabled(enabled);
        if (convertedName == std::string("SphereCollider")) gameObject.value()->GetComponent<SphereCollider>()->SetEnabled(enabled);
        if (convertedName == std::string("CapsuleCollider")) gameObject.value()->GetComponent<CapsuleCollider>()->SetEnabled(enabled);
        if (convertedName == std::string("MeshCollider")) gameObject.value()->GetComponent<MeshCollider>()->SetEnabled(enabled);
        if (convertedName == std::string("ConvexMeshCollider")) gameObject.value()->GetComponent<ConvexMeshCollider>()->SetEnabled(enabled);
        if (convertedName == std::string("ApproximationMeshCollider")) gameObject.value()->GetComponent<ApproximationMeshCollider>()->SetEnabled(enabled);
        if (convertedName == std::string("RigidBody")) gameObject.value()->GetComponent<RigidBody>()->SetEnabled(enabled);
        if (convertedName == std::string("DynamicCharacterController")) gameObject.value()->GetComponent<DynamicCharacterController>()->SetEnabled(enabled);
        if (convertedName == std::string("VehicleComponent")) gameObject.value()->GetComponent<VehicleComponent>()->SetEnabled(enabled);
        if (convertedName == std::string("Animator")) gameObject.value()->GetComponent<Animator>()->SetEnabled(enabled);
        if (convertedName == std::string("UIComponent")) gameObject.value()->GetComponent<UIComponent>()->SetEnabled(enabled);
    }

    static bool Component_GetEnabled(uint32_t id, MonoString* componentType)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in Component_SetEnabled)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in Component_SetEnabled)");

        const std::string convertedName = Utils::MonoStringToUTF8(componentType);

        if (convertedName == std::string("Transform")) return gameObject.value()->transform.Enabled();
        if (convertedName == std::string("DirectionalLight")) return gameObject.value()->GetComponent<DirectionalLight>()->Enabled();
        if (convertedName == std::string("PointLight")) return gameObject.value()->GetComponent<PointLight>()->Enabled();
        if (convertedName == std::string("SpotLight")) return gameObject.value()->GetComponent<SpotLight>()->Enabled();
        if (convertedName == std::string("MeshRenderer")) return gameObject.value()->GetComponent<MeshRenderer>()->Enabled();
        if (convertedName == std::string("SkinnedMeshRenderer")) return gameObject.value()->GetComponent<SkinnedMeshRenderer>()->Enabled();
        if (convertedName == std::string("BoxCollider")) return gameObject.value()->GetComponent<BoxCollider>()->Enabled();
        if (convertedName == std::string("SphereCollider")) return gameObject.value()->GetComponent<SphereCollider>()->Enabled();
        if (convertedName == std::string("CapsuleCollider")) return gameObject.value()->GetComponent<CapsuleCollider>()->Enabled();
        if (convertedName == std::string("MeshCollider")) return gameObject.value()->GetComponent<MeshCollider>()->Enabled();
        if (convertedName == std::string("ConvexMeshCollider")) return gameObject.value()->GetComponent<ConvexMeshCollider>()->Enabled();
        if (convertedName == std::string("ApproximationMeshCollider")) return gameObject.value()->GetComponent<ApproximationMeshCollider>()->Enabled();
        if (convertedName == std::string("RigidBody")) return gameObject.value()->GetComponent<RigidBody>()->Enabled();
        if (convertedName == std::string("DynamicCharacterController")) return gameObject.value()->GetComponent<DynamicCharacterController>()->Enabled();
        if (convertedName == std::string("VehicleComponent")) return gameObject.value()->GetComponent<VehicleComponent>()->Enabled();
        if (convertedName == std::string("Animator")) return gameObject.value()->GetComponent<Animator>()->Enabled();
        if (convertedName == std::string("UIComponent")) return gameObject.value()->GetComponent<UIComponent>()->Enabled();

        DENTY_SET_ERR_COLOR
        DENTY_ERR_LOG << "Unknown component type! ( " << convertedName << " )";

        return false;
    }

    // Transform
	static void TransformComponent_GetPosition(uint32_t id, float* x, float* y, float* z)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetPosition)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetPosition)");

        const Vector3& position = gameObject.value()->transform.position;
        *x = position.x;
        *y = position.y;
        *z = position.z;
    }

    static void TransformComponent_SetLocalPosition(uint32_t id, float localX, float localY, float localZ)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_SetLocalPosition)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_SetLocalPosition)");

        gameObject.value()->transform.localPosition = Vector3(localX, localY, localZ);
    }

    static void TransformComponent_GetLocalPosition(uint32_t id, float* lx, float* ly, float* lz)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetLocalPosition)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetLocalPosition)");

        const Vector3& localPosition = gameObject.value()->transform.localPosition;
        *lx = localPosition.x;
        *ly = localPosition.y;
        *lz = localPosition.z;
    }

    static void TransformComponent_SetLocalEulerAngles(uint32_t id, float eulerAngleX, float eulerAngleY, float eulerAngleZ)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_SetLocalEulerAngles)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_SetLocalEulerAngles)");

        gameObject.value()->transform.localEulerAngles = Vector3(eulerAngleX, eulerAngleY, eulerAngleZ);
    }

    static void TransformComponent_GetLocalEulerAngles(uint32_t id, float* outEulerAngleX, float* outEulerAngleY, float* outEulerAngleZ)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetLocalEulerAngles)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetLocalEulerAngles)");

        const Vector3& localEulerAngles = gameObject.value()->transform.localEulerAngles;
        *outEulerAngleX = localEulerAngles.x;
        *outEulerAngleY = localEulerAngles.y;
        *outEulerAngleZ = localEulerAngles.z;
    }

    static void TransformComponent_SetLocalScale(uint32_t id, float localScaleX, float localScaleY, float localScaleZ)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_SetLocalScale)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_SetLocalScale)");

        gameObject.value()->transform.localScale = Vector3(localScaleX, localScaleY, localScaleZ);
    }

    static void TransformComponent_GetLocalScale(uint32_t id, float* outLocalScaleX, float* outLocalScaleY, float* outLocalScaleZ)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetLocalScale)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetLocalScale)");

        const Vector3& localScale = gameObject.value()->transform.localScale;
        *outLocalScaleX = localScale.x;
        *outLocalScaleY = localScale.y;
        *outLocalScaleZ = localScale.z;
    }

    static void TransformComponent_GetRight(uint32_t id, float* outRightX, float* outRightY, float* outRightZ)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetRight)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetRight)");

        const Vector3& right = gameObject.value()->transform.local.GetRightVec3();
        *outRightX = right.x;
        *outRightY = right.y;
        *outRightZ = right.z;
    }

    static void TransformComponent_GetUp(uint32_t id, float* outUpX, float* outUpY, float* outUpZ)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetUp)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetUp)");

        const Vector3& up = gameObject.value()->transform.local.GetUpVec3();
        *outUpX = up.x;
        *outUpY = up.y;
        *outUpZ = up.z;
    }

    static void TransformComponent_GetFront(uint32_t id, float* outFrontX, float* outFrontY, float* outFrontZ)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetFront)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetFront)");

        const Vector3& front = gameObject.value()->transform.local.GetFrontVec3();
        *outFrontX = front.x;
        *outFrontY = front.y;
        *outFrontZ = front.z;
    }

    static void TransformComponent_GetWorld(
        uint32_t id,
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetWorld)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetWorld)");

        const Matrix world = gameObject.value()->transform.world;
        *_11 = world._11, *_12 = world._12, *_13 = world._13, *_14 = world._14;
        *_21 = world._21, *_22 = world._22, *_23 = world._23, *_24 = world._24;
        *_31 = world._31, *_32 = world._32, *_33 = world._33, *_34 = world._34;
        *_41 = world._41, *_42 = world._42, *_43 = world._43, *_44 = world._44;
    }

    static void TransformComponent_SetWorld(
        uint32_t id,
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetWorld)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetWorld)");

        const Matrix world =
        {
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        };

        gameObject.value()->transform.world = world;
    }

     static void TransformComponent_GetLocal(
        uint32_t id,
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetLocal)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetLocal)");

        const Matrix local = gameObject.value()->transform.local;
        *_11 = local._11, *_12 = local._12, *_13 = local._13, *_14 = local._14;
        *_21 = local._21, *_22 = local._22, *_23 = local._23, *_24 = local._24;
        *_31 = local._31, *_32 = local._32, *_33 = local._33, *_34 = local._34;
        *_41 = local._41, *_42 = local._42, *_43 = local._43, *_44 = local._44;
    }

    static void TransformComponent_SetLocal(
        uint32_t id,
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_SetLocal)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_SetLocal)");

        const Matrix local =
        {
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        };

        gameObject.value()->transform.local = local;
    }

    static bool TransformComponent_GetUpdate(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_GetUpdate)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_GetUpdate)");

        return gameObject.value()->transform.GetUpdate();
    }

    static void TransformComponent_SetUpdate(uint32_t id, bool update)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in TransformComponent_SetUpdate)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in TransformComponent_SetUpdate)");

        gameObject.value()->transform.SetUpdate(update);
    }

    // GameCamera
    static void GameCameraComponent_SetFocusPosition(uint32_t id, float x, float y, float z)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameCamera_SetFocusPosition)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameCamera_SetFocusPosition)");
        
        gameObject.value()->GetComponentAsSharedPtr<GameCamera>()->SetFocusPosition({ x, y, z });
    }

    static void GameCameraComponent_SetViewProjection(
        uint32_t id, 
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameCameraComponent_SetViewProjection)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameCameraComponent_SetViewProjection)");

        const Matrix viewProjection =
        {
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        };

        gameObject.value()->GetComponentAsSharedPtr<GameCamera>()->SetViewProjection(viewProjection);
    }

    static void GameCameraComponent_SetView(
        uint32_t id, 
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameCameraComponent_SetView)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameCameraComponent_SetView)");

        const Matrix view =
        {
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        };

        gameObject.value()->GetComponentAsSharedPtr<GameCamera>()->SetView(view);
    }

    static void GameCameraComponent_SetProjection(
        uint32_t id, 
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in GameCameraComponent_SetProjection)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in GameCameraComponent_SetProjection)");

        const Matrix projection =
        {
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        };

        gameObject.value()->GetComponentAsSharedPtr<GameCamera>()->SetProjection(projection);
    }

    static void GameCameraComponent_ComputeView(
            float frontX, float frontY, float frontZ,
            float localPositionX, float localPositionY, float localPositionZ,
            float* _11, float* _12, float* _13, float* _14,
            float* _21, float* _22, float* _23, float* _24,
            float* _31, float* _32, float* _33, float* _34,
            float* _41, float* _42, float* _43, float* _44
    )
    {
        const Vector3 front = Vector3(frontX, frontY, frontZ).Normalize();
        const Vector3 tempRight = Vector3::Right().Normalize();

        const Vector3 up = front.Cross(tempRight);
        const Vector3 right = up.Cross(front);

        const Matrix view =
        {
            right,
            up,
            front,
            Vector3(localPositionX, localPositionY, localPositionZ)
        };

        *_11 = view._11;
        *_12 = view._12;
        *_13 = view._13;
        *_14 = view._14;

        *_21 = view._21;
        *_22 = view._22;
        *_23 = view._23;
        *_24 = view._24;

        *_31 = view._31;
        *_32 = view._32;
        *_33 = view._33;
        *_34 = view._34;

        *_41 = view._41;
        *_42 = view._42;
        *_43 = view._43;
        *_44 = view._44;
    }

    static void GameCameraComponent_ComputePerspectiveProjection(
            float fovAngleY, float aspectRatio, float nearZ, float farZ,
            float* _11, float* _12, float* _13, float* _14,
            float* _21, float* _22, float* _23, float* _24,
            float* _31, float* _32, float* _33, float* _34,
            float* _41, float* _42, float* _43, float* _44
    )
    {
        const Matrix perspectiveProjection = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);

        *_11 = perspectiveProjection._11;
        *_12 = perspectiveProjection._12;
        *_13 = perspectiveProjection._13;
        *_14 = perspectiveProjection._14;

        *_21 = perspectiveProjection._21;
        *_22 = perspectiveProjection._22;
        *_23 = perspectiveProjection._23;
        *_24 = perspectiveProjection._24;

        *_31 = perspectiveProjection._31;
        *_32 = perspectiveProjection._32;
        *_33 = perspectiveProjection._33;
        *_34 = perspectiveProjection._34;

        *_41 = perspectiveProjection._41;
        *_42 = perspectiveProjection._42;
        *_43 = perspectiveProjection._43;
        *_44 = perspectiveProjection._44;
    }

    static void GameCameraComponent_ComputeOrthographicProjection(
            float viewSizeX, float viewSizeY,
            float nearZ, float farZ,
            float* _11, float* _12, float* _13, float* _14,
            float* _21, float* _22, float* _23, float* _24,
            float* _31, float* _32, float* _33, float* _34,
            float* _41, float* _42, float* _43, float* _44
    )
    {
        const Matrix orthographicProjection = DirectX::XMMatrixOrthographicLH(viewSizeX, viewSizeY, nearZ, farZ);

        *_11 = orthographicProjection._11;
        *_12 = orthographicProjection._12;
        *_13 = orthographicProjection._13;
        *_14 = orthographicProjection._14;

        *_21 = orthographicProjection._21;
        *_22 = orthographicProjection._22;
        *_23 = orthographicProjection._23;
        *_24 = orthographicProjection._24;

        *_31 = orthographicProjection._31;
        *_32 = orthographicProjection._32;
        *_33 = orthographicProjection._33;
        *_34 = orthographicProjection._34;

        *_41 = orthographicProjection._41;
        *_42 = orthographicProjection._42;
        *_43 = orthographicProjection._43;
        *_44 = orthographicProjection._44;
    }

    static void GameCameraComponent_UpdateMainCameraConstants(
        float view_11, float view_12, float view_13, float view_14,
        float view_21, float view_22, float view_23, float view_24,
        float view_31, float view_32, float view_33, float view_34,
        float view_41, float view_42, float view_43, float view_44,

        float projection_11, float projection_12, float projection_13, float projection_14,
        float projection_21, float projection_22, float projection_23, float projection_24,
        float projection_31, float projection_32, float projection_33, float projection_34,
        float projection_41, float projection_42, float projection_43, float projection_44,

        float positionX, float positionY, float positionZ, float positionW
    )
    {
        DENTY_CHECK_NULL(GameCamera::GetMainCamera())

        Camera::Constants constants = { };

        const Matrix view =
        {
            view_11, view_12, view_13, view_14,
            view_21, view_22, view_23, view_24,
            view_31, view_32, view_33, view_34,
            view_41, view_42, view_43, view_44,
        };

        const Matrix projection =
        {
            projection_11, projection_12, projection_13, projection_14,
            projection_21, projection_22, projection_23, projection_24,
            projection_31, projection_32, projection_33, projection_34,
            projection_41, projection_42, projection_43, projection_44,
        };

        const Matrix inverseView = view.Inverse();
        const Matrix inverseProjection = projection.Inverse();

        const Vector4 position = { positionX, positionY, positionZ, positionW };

        constants.view = view;
        constants.inverseView = inverseView;
        constants.projection = projection;
        constants.inverseProjection = inverseProjection;
        constants.viewProjection = view * projection;
        constants.cameraPosition = position;

        GameCamera::UpdateMainCameraConstants(constants);
    }

    // MainCamera
    static void GameCamera_MainCamera_GetOrientation(
		float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44
    )
    {
	    const Ref<GameCamera> mainCamera = GameCamera::GetMainCamera();
        DENTY_CHECK_NULL(mainCamera);

        const Matrix world = mainCamera->GetInverseView();
        *_11 = world._11;
        *_12 = world._12;
        *_13 = world._13;
        *_14 = world._14;

        *_21 = world._21;
        *_22 = world._22;
        *_23 = world._23;
        *_24 = world._24;

        *_31 = world._31;
        *_32 = world._32;
        *_33 = world._33;
        *_34 = world._34;

        *_41 = world._41;
        *_42 = world._42;
        *_43 = world._43;
        *_44 = world._44;
    }

    //
    // RigidBody
    //
    static void RigidBodyComponent_SetLinearVelocity(uint32_t id, float x, float y, float z)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in RigidBodyComponent_SetVelocity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in RigidBodyComponent_SetVelocity)");

        const Ref<RigidBody>& component = gameObject.value()->GetComponentAsSharedPtr<RigidBody>();
        component->SetLinearVelocity(Vector3(x, y, z));
    }

    static void RigidBodyComponent_EnableAngularVelocity(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in RigidBodyComponent_EnableAngularVelocity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in RigidBodyComponent_EnableAngularVelocity)");

        const Ref<RigidBody>& component = gameObject.value()->GetComponentAsSharedPtr<RigidBody>();
        component->EnableAngularVelocity();
    }

    static void RigidBodyComponent_DisableAngularVelocity(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in RigidBodyComponent_DisableAngularVelocity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in RigidBodyComponent_DisableAngularVelocity)");

        const Ref<RigidBody>& component = gameObject.value()->GetComponentAsSharedPtr<RigidBody>();
        component->DisableAngularVelocity();
    }

    static void RigidBodyComponent_AddForce(uint32_t id, float x, float y, float z)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in RigidBodyComponent_AddForce)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in RigidBodyComponent_AddForce)");

        const Ref<RigidBody>& component = gameObject.value()->GetComponentAsSharedPtr<RigidBody>();
        component->AddForce(Vector3(x, y, z));
    }

    static void RigidBodyComponent_AddImpulse(uint32_t id, float x, float y, float z)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in RigidBodyComponent_AddImpulse)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in RigidBodyComponent_AddImpulse)");

        const Ref<RigidBody>& component = gameObject.value()->GetComponentAsSharedPtr<RigidBody>();
        component->AddImpulse(Vector3(x, y, z));
    }

    static void RigidBodyComponent_GetLinearVelocity(uint32_t id, float* x, float* y, float* z)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in RigidBodyComponent_GetLinearVelocity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in RigidBodyComponent_GetLinearVelocity)");

        const Ref<RigidBody>& component = gameObject.value()->GetComponentAsSharedPtr<RigidBody>();
        const Vector3& linearVelocity = component->GetLinearVelocity();

        *x = linearVelocity.x;
        *y = linearVelocity.y;
        *z = linearVelocity.z;
    }

    static void RigidBodyComponent_SetGravity(uint32_t id, float x, float y, float z)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in RigidBodyComponent_SetGravity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in RigidBodyComponent_SetGravity)");

        const Ref<RigidBody>& component = gameObject.value()->GetComponentAsSharedPtr<RigidBody>();
        component->SetGravity(Vector3(x, y, z));
    }

    static void RigidBodyComponent_GetRigidBodyWorld(
        uint32_t id,
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in RigidBodyComponent_GetRigidBodyWorld)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in RigidBodyComponent_GetRigidBodyWorld)");

        const auto& rigidBody = gameObject.value()->GetComponentAsSharedPtr<RigidBody>();
        const Matrix& world = rigidBody->GetRigidBodyWorld();

        *_11 = world._11;
        *_12 = world._12;
        *_13 = world._13;
        *_14 = world._14;

        *_21 = world._21;
        *_22 = world._22;
        *_23 = world._23;
        *_24 = world._24;

        *_31 = world._31;
        *_32 = world._32;
        *_33 = world._33;
        *_34 = world._34;

        *_41 = world._41;
        *_42 = world._42;
        *_43 = world._43;
        *_44 = world._44;
    }

    static void RigidBodyComponent_SetPosition(
        uint32_t id,
        float x, float y, float z
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in RigidBodyComponent_SetPosition)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in RigidBodyComponent_SetPosition)");

        const auto& rigidBody = gameObject.value()->GetComponentAsSharedPtr<RigidBody>();
        rigidBody->SetPosition(Vector3(x, y, z));
    }

    //
    // VehicleComponent
    //
    static void VehicleComponent_SetActivate(bool forceActivate, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetLinearVelocity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetLinearVelocity)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().rigidBody->activate(forceActivate);
    }

    static void VehicleComponent_SetVelocity(float x, float y, float z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetVelocity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetVelocity)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().simplifiedCarAction->SetVelocity(Vector3(x, y, z));
    }

    static void VehicleComponent_GetVelocity(float* x, float* y, float* z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_GetVelocity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_GetVelocity)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        const Vector3 velocity = component->GetCar().GetComponents().simplifiedCarAction->GetVelocity();
        *x = velocity.x;
        *y = velocity.y;
        *z = velocity.z;
    }

    static void VehicleComponent_SetLinearVelocity(float x, float y, float z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetLinearVelocity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetLinearVelocity)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().rigidBody->setLinearVelocity(btVector3(x, y, z));
    }

    static void VehicleComponent_GetLinearVelocity(float* x, float* y, float* z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_GetLinearVelocity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_GetLinearVelocity)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();

        const Vector3 linearVelocity = Vector3::ToVector3(component->GetCar().GetComponents().rigidBody->getLinearVelocity());
        *x = linearVelocity.x;
        *y = linearVelocity.y;
        *z = linearVelocity.z;
    }

    static void VehicleComponent_ApplyCentralImpulse(float x, float y, float z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_ApplyCentralImpulse)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_ApplyCentralImpulse)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().rigidBody->applyCentralImpulse(btVector3(x, y, z));
    }

    static void VehicleComponent_ApplyForce(float x, float y, float z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_ApplyForce)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_ApplyForce)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().rigidBody->applyCentralForce(btVector3(x, y, z));
    }

    static void VehicleComponent_SetGravity(float x, float y, float z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetGravity)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetGravity)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().rigidBody->setGravity(btVector3(x, y, z));
    }

    static void VehicleComponent_SetFront(float x, float y, float z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetFront)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetFront)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().SetFront(Vector3(x, y, z));
    }

    static void VehicleComponent_SetOrientation(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        uint32_t id
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetOrientation)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetOrientation)");

        const auto vehicle = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();

        btTransform trans;
        vehicle->GetCar().GetComponents().rigidBody->getMotionState()->getWorldTransform(trans);
        const Vector3 origin = Vector3::ToVector3(trans.getOrigin());

        const Matrix orientation = 
        {
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            origin.x, origin.y, origin.z, 1.0f
        };

        vehicle->GetCar().GetComponents().rigidBody->getMotionState()->setWorldTransform(
            orientation.ToBtTransformWithoutScaling()
        );
    }

    static void VehicleComponent_GetOrientation(
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        uint32_t id
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_GetOrientation)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_GetOrientation)");

        const auto vehicle = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();

        btTransform trans;
        vehicle->GetCar().GetComponents().rigidBody->getMotionState()->getWorldTransform(trans);

        const Matrix orientation = Matrix::ToMatrix(trans.getBasis());

        *_11 = orientation._11;
        *_12 = orientation._12;
        *_13 = orientation._13;
        *_14 = orientation._14;

        *_21 = orientation._21;
        *_22 = orientation._22;
        *_23 = orientation._23;
        *_24 = orientation._24;

        *_31 = orientation._31;
        *_32 = orientation._32;
        *_33 = orientation._33;
        *_34 = orientation._34;
    }

    static void VehicleComponent_SetPosition(
        float x, float y, float z, uint32_t id
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetPosition)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetPosition)");

        const auto vehicle = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();

        vehicle->GetCar().GetComponents().simplifiedCarAction->SetPosition(Vector3(x, y, z));
    }

    static void VehicleComponent_GetPosition(
        float* x, float* y, float* z, uint32_t id
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_GetPosition)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_GetPosition)");

        const auto vehicle = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();

        const Vector3 position = vehicle->GetCar().GetComponents().simplifiedCarAction->GetPosition();

        *x = position.x;
        *y = position.y;
        *z = position.z;
    }

    static void VehicleComponent_SetMotionWorldTransform(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44,
        uint32_t id
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetMotionWorldTransform)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetMotionWorldTransform)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        const auto world = Matrix(
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        );

        component->GetCar().GetComponents().rigidBody->getMotionState()->setWorldTransform(world.ToBtTransformWithoutScaling());
    }

    static void VehicleComponent_GetMotionWorldTransform(
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44,
        uint32_t id
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_GetMotionWorldTransform)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_GetMotionWorldTransform)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();

        btTransform trans;
        component->GetCar().GetComponents().rigidBody->getMotionState()->getWorldTransform(trans);

        const Matrix world = Matrix::ToMatrix(trans);

        *_11 = world._11;
        *_12 = world._12;
        *_13 = world._13;
        *_14 = world._14;

        *_21 = world._21;
        *_22 = world._22;
        *_23 = world._23;
        *_24 = world._24;

        *_31 = world._31;
        *_32 = world._32;
        *_33 = world._33;
        *_34 = world._34;

        *_41 = world._41;
        *_42 = world._42;
        *_43 = world._43;
        *_44 = world._44;
    }

    static void VehicleComponent_SetWorldTransform(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44,
        uint32_t id
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetWorldTransform)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetWorldTransform)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        const auto world = Matrix(
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44
        );

        component->GetCar().GetComponents().rigidBody->setWorldTransform(world.ToBtTransformWithoutScaling());
    }

    static void VehicleComponent_GetWorldTransform(
        float* _11, float* _12, float* _13, float* _14,
        float* _21, float* _22, float* _23, float* _24,
        float* _31, float* _32, float* _33, float* _34,
        float* _41, float* _42, float* _43, float* _44,
        uint32_t id
    )
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_GetWorldTransform)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_GetWorldTransform)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();

        const btTransform trans = component->GetCar().GetComponents().rigidBody->getWorldTransform();
        const Matrix world = Matrix::ToMatrix(trans);

        *_11 = world._11;
        *_12 = world._12;
        *_13 = world._13;
        *_14 = world._14;

        *_21 = world._21;
        *_22 = world._22;
        *_23 = world._23;
        *_24 = world._24;

        *_31 = world._31;
        *_32 = world._32;
        *_33 = world._33;
        *_34 = world._34;

        *_41 = world._41;
        *_42 = world._42;
        *_43 = world._43;
        *_44 = world._44;
    }

    static void VehicleComponent_GetHitWallNormal(float* x, float* y, float* z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_GetHitWallNormal)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_GetHitWallNormal)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        Vector3 normal = component->GetCar().GetComponents().simplifiedCarAction->GetFindWallHit().normal;
        normal = normal.Normalize();

        *x = normal.x;
        *y = normal.y;
        *z = normal.z;
    }

    static void VehicleComponent_Jump(float x, float y, float z, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_Jump)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_Jump)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().simplifiedCarAction->Jump(Vector3(x, y, z));
    }

    static void VehicleComponent_SetJumpFlag(bool flag, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetJumpFlag)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetJumpFlag)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().simplifiedCarAction->SetJump(flag);
    }

    static bool VehicleComponent_IsJump(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_IsJump)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_IsJump)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        return component->GetCar().GetComponents().simplifiedCarAction->IsJump();
    }

    static bool VehicleComponent_IsDrifting(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_IsDrifting)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_IsDrifting)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        return component->GetCar().GetComponents().simplifiedCarAction->IsDrifting();
    }

    static void VehicleComponent_SetDrifting(bool drifting, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_SetDrifting)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_SetDrifting)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().simplifiedCarAction->SetDrifting(drifting);
    }

    static void VehicleComponent_ResetDrift(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_ResetDrift)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_ResetDrift)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        component->GetCar().GetComponents().simplifiedCarAction->ResetDrift();
    }

    static int32_t VehicleComponent_GetDriftDirection(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_GetDriftDirection)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_GetDriftDirection)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        return static_cast<int32_t>(component->GetCar().GetComponents().simplifiedCarAction->GetDriftDirection());
    }

    static float VehicleComponent_GetOnDriftInputLX(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_GetOnDriftInputLX)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_GetOnDriftInputLX)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        return static_cast<int32_t>(component->GetCar().GetComponents().simplifiedCarAction->GetOnDriftInputLX());
    }

    static bool VehicleComponent_OnGround(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_OnGround)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_OnGround)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        return component->GetCar().GetComponents().simplifiedCarAction->GetFindGroundHit().onGround;
    }

    static bool VehicleComponent_OnGrass(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_OnGrass");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_OnGrass)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        return component->GetCar().GetComponents().simplifiedCarAction->GetFindGrassHit().onGrass;
    }

    static bool VehicleComponent_HitWall(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in VehicleComponent_HitWall)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in VehicleComponent_HitWall)");

        const Ref<VehicleComponent> component = gameObject.value()->GetComponentAsSharedPtr<VehicleComponent>();
        return component->GetCar().GetComponents().simplifiedCarAction->GetFindWallHit().hitWall;
    }

    //
    // Game
    //

    //
    // Race Game
    //
    static int32_t CheckPointComponent_GetDesireEntityPassingCheckPointCount(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in CheckPointComponent_GetDesireEntityPassingCheckPointCount)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in CheckPointComponent_GetDesireEntityPassingCheckPointCount)");

        const Ref<CheckPoint> checkPoint = gameObject.value()->GetComponentAsSharedPtr<CheckPoint>();
        return checkPoint->GetSpecification().desireEntityPassingCheckPointCount;
    }

    static int32_t CheckPointComponent_GetNumber(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in CheckPointComponent_GetNumber)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in CheckPointComponent_GetNumber)");

        const Ref<CheckPoint> checkPoint = gameObject.value()->GetComponentAsSharedPtr<CheckPoint>();
        return checkPoint->GetSpecification().number;
    }

    static int32_t CheckPointComponent_GetNextCheckPointNumber(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in CheckPointComponent_GetNextCheckPointNumber)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in CheckPointComponent_GetNextCheckPointNumber)");

        const Ref<CheckPoint> checkPoint = gameObject.value()->GetComponentAsSharedPtr<CheckPoint>();
        return checkPoint->GetSpecification().nextCheckPointNumber;
    }
#pragma endregion

#pragma region Component2D
    static MonoString* UIComponent_GetItemType(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_GetItemType)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_GetItemType)");

        const std::string itemType = gameObject.value()->GetComponent<UIComponent>()->GetItem()->GetItemType();

        return Utils::CreateMonoString(ScriptEngine::GetAppDomain(), itemType.c_str());
    }

    static void UIComponent_GetPosition(float* x, float* y, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_GetPosition)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_GetPosition)");

        const Vector2 position = gameObject.value()->GetComponent<UIComponent>()->GetItem()->transform.position;
        *x = position.x;
        *y = position.y;
    }

    static float UIComponent_GetScale(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_GetScale)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_GetScale)");

        return gameObject.value()->GetComponent<UIComponent>()->GetItem()->transform.scale;
    }

    static float UIComponent_GetRotation(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_GetRotation)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_GetRotation)");

        return gameObject.value()->GetComponent<UIComponent>()->GetItem()->transform.rotation;
    }

    static void UIComponent_SetPosition(float x, float y, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_SetPosition)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_SetPosition)");

        gameObject.value()->GetComponent<UIComponent>()->GetItem()->transform.position = Vector2(x, y);
    }

    static void UIComponent_SetScale(float scale, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_SetScale)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_SetScale)");

        gameObject.value()->GetComponent<UIComponent>()->GetItem()->transform.scale = scale;
    }

    static void UIComponent_SetRotation(float rotation, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_SetRotation)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_SetRotation)");

        gameObject.value()->GetComponent<UIComponent>()->GetItem()->transform.rotation = rotation;
    }

    static bool UIComponent_GetRender(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_GetRender)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_GetRender)");

        return gameObject.value()->GetComponentAsSharedPtr<UIComponent>()->GetItem()->IsRender();
    }

    static void UIComponent_SetRender(bool render, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_SetRender)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_SetRender)");

        return gameObject.value()->GetComponentAsSharedPtr<UIComponent>()->GetItem()->SetRender(render);
    }

    //
    // Text item.
    //
    static MonoString* UIComponent_Text_GetText(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_Text_GetText)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_Text_GetText)");

        const auto textItem = std::static_pointer_cast<UIText>(gameObject.value()->GetComponent<UIComponent>()->GetItem());

        return Utils::CreateMonoString(ScriptEngine::GetAppDomain(), textItem->GetText().c_str());
    }

    static float UIComponent_Text_GetSpacing(uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_Text_GetSpacing)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_Text_GetSpacing)");

        const auto textItem = std::static_pointer_cast<UIText>(gameObject.value()->GetComponent<UIComponent>()->GetItem());

        return textItem->GetSpacing();
    }

    static void UIComponent_Text_GetTextColor(float* r, float* g, float* b, float* a, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_Text_GetTextColor)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_Text_GetTextColor)");

        const auto textItem = std::static_pointer_cast<UIText>(gameObject.value()->GetComponent<UIComponent>()->GetItem());

        const Color color = textItem->GetTextColor();
        *r = color.color.x;
        *g = color.color.y;
        *b = color.color.z;
        *a = color.color.w;
    }

    static void UIComponent_Text_SetText(MonoString* monoText, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_Text_SetText)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_Text_SetText)");

        const std::string text = Utils::MonoStringToUTF8(monoText);

        const auto textItem = std::static_pointer_cast<UIText>(gameObject.value()->GetComponent<UIComponent>()->GetItem());
        textItem->SetText(text);
    }

    static void UIComponent_Text_SetSpacing(float spacing, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_Text_SetSpacing)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_Text_SetSpacing)");

        const auto textItem = std::static_pointer_cast<UIText>(gameObject.value()->GetComponent<UIComponent>()->GetItem());
        textItem->SetLineSpacing(spacing);
    }

    static void UIComponent_Text_SetTextColor(float r, float g, float b, float a, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_Text_SetTextColor)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_Text_SetTextColor)");

        const auto textItem = std::static_pointer_cast<UIText>(gameObject.value()->GetComponent<UIComponent>()->GetItem());
        textItem->SetTextColor(Color(r, g, b, a));
    }

    //
    // Image item.
    //
	static void UIComponent_Image_GetClipRect(int32_t* left, int32_t* top, int32_t* right, int32_t* bottom, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_Image_GetClipRect)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_Image_GetClipRect)");

        const auto uiImage = std::static_pointer_cast<UIImage>(gameObject.value()->GetComponent<UIComponent>()->GetItem());
        const Vector4I clipPosition = uiImage->GetClipPosition();

        *left = clipPosition.x;
        *top = clipPosition.y;
        *right = clipPosition.z;
        *bottom = clipPosition.w;
    }

    static void UIComponent_Image_SetClipRect(int32_t left, int32_t top, int32_t right, int32_t bottom, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_Image_SetClipRect)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_Image_SetClipRect)");

        const auto uiImage = std::static_pointer_cast<UIImage>(gameObject.value()->GetComponent<UIComponent>()->GetItem());
        uiImage->SetClipPosition(left, top, right, bottom);
    }

    static void UIComponent_Image_GetTextureSize(int32_t* x, int32_t* y, uint32_t id)
    {
        const Ref<Scene> scene = ScriptEngine::GetActiveScene();
        DENTY_ASSERT(scene, "Scene is null! (in UIComponent_Image_GetTextureSize)");

        const std::optional<Ref<GameObject>> gameObject = scene->FindByEntityID(id);
        DENTY_ASSERT(gameObject.has_value(), "GameObject is null! (in UIComponent_Image_GetTextureSize)");

        const auto uiImage = std::static_pointer_cast<UIImage>(gameObject.value()->GetComponent<UIComponent>()->GetItem());
        const Vector2 textureSize = uiImage->GetSize();

        *x = static_cast<int32_t>(textureSize.x);
        *y = static_cast<int32_t>(textureSize.y);
    }

#pragma endregion

#pragma region ParticleSystem
    //
    // CPU ParticleSystem
    //
    static void CPUParticleSystem_Emit(
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
    )
    {
        ParticleProps props = { };
        props.beginSize = beginSize;
        props.endSize = endSize;
        props.sizeVariation = sizeVariation;

        props.lifeTime = lifeTime;

        props.rotation = rotation;

        props.position.x = positionX;
        props.position.y = positionY;
        props.position.z = positionZ;

        props.beginPosition.x = beginPositionX;
        props.beginPosition.y = beginPositionY;
        props.beginPosition.z = beginPositionZ;

        props.endPosition.x = endPositionX;
        props.endPosition.y = endPositionY;
        props.endPosition.z = endPositionZ;

        props.initialVelocity.x = initialVelocityX;
        props.initialVelocity.y = initialVelocityY;
        props.initialVelocity.z = initialVelocityZ;

        props.velocityVariation.x = velocityVariationX;
        props.velocityVariation.y = velocityVariationY;
        props.velocityVariation.z = velocityVariationZ;

        props.beginColor = Color(beginColorR, beginColorG, beginColorB, beginColorA);
        props.endColor = Color(endColorR, endColorG, endColorB, endColorA);

        props.applyGravityForce = applyGravityForce;
        props.useBeginAndEndPosition = useBeginAndEndPosition;

        CPUParticleSystem::GetInstance()->Emit(props);
    }

    //
    // GPU ParticleSystem
    //
#pragma endregion

    void ScriptInternalCall::RegisterInternalCalls()
    {
        // Input
        {
            // Keyboard
            DENTY_ADD_INTERNAL_CALL(Input_IsKeyPressed);
            DENTY_ADD_INTERNAL_CALL(Input_IsKeyTriggered);
            DENTY_ADD_INTERNAL_CALL(Input_IsKeyReleased);

            // Mouse
            DENTY_ADD_INTERNAL_CALL(Input_IsMousePressed);
            DENTY_ADD_INTERNAL_CALL(Input_IsMouseTriggered);
            DENTY_ADD_INTERNAL_CALL(Input_IsMouseReleased);
            DENTY_ADD_INTERNAL_CALL(Input_SetMousePosition);
            DENTY_ADD_INTERNAL_CALL(Input_GetMouseWheelSpeed);
            DENTY_ADD_INTERNAL_CALL(Input_GetMousePanX);
            DENTY_ADD_INTERNAL_CALL(Input_GetMousePanY);
            DENTY_ADD_INTERNAL_CALL(Input_GetMousePosition);

            // GamePad
            DENTY_ADD_INTERNAL_CALL(Input_IsPadPressed);
            DENTY_ADD_INTERNAL_CALL(Input_IsPadTriggered);
            DENTY_ADD_INTERNAL_CALL(Input_IsPadReleased);
            DENTY_ADD_INTERNAL_CALL(Input_IsPadLeftTriggerTriggered);
            DENTY_ADD_INTERNAL_CALL(Input_IsPadRightTriggerTriggered);
            DENTY_ADD_INTERNAL_CALL(Input_IsPadConnectedAt);
            DENTY_ADD_INTERNAL_CALL(Input_GetLeftTrigger);
            DENTY_ADD_INTERNAL_CALL(Input_GetRightTrigger);
            DENTY_ADD_INTERNAL_CALL(Input_GetThumbLX);
            DENTY_ADD_INTERNAL_CALL(Input_GetThumbLY);
            DENTY_ADD_INTERNAL_CALL(Input_GetThumbRX);
            DENTY_ADD_INTERNAL_CALL(Input_GetThumbRY);
            DENTY_ADD_INTERNAL_CALL(Input_GetNormalizedThumbLX);
            DENTY_ADD_INTERNAL_CALL(Input_GetNormalizedThumbLY);
            DENTY_ADD_INTERNAL_CALL(Input_GetNormalizedThumbRX);
            DENTY_ADD_INTERNAL_CALL(Input_GetNormalizedThumbRY);
        }

        // Debug
        {
            DENTY_ADD_INTERNAL_CALL(Debug_Log);
        }

        // ImGui
        {
            DENTY_ADD_INTERNAL_CALL(ImGui_CollapsingHeader);
            DENTY_ADD_INTERNAL_CALL(ImGui_Text);
            DENTY_ADD_INTERNAL_CALL(ImGui_InputFloat);
            DENTY_ADD_INTERNAL_CALL(ImGui_InputFloat2);
            DENTY_ADD_INTERNAL_CALL(ImGui_InputFloat3);
            DENTY_ADD_INTERNAL_CALL(ImGui_InputFloat4);
            DENTY_ADD_INTERNAL_CALL(ImGui_InputInt);
            DENTY_ADD_INTERNAL_CALL(ImGui_InputInt2);
            DENTY_ADD_INTERNAL_CALL(ImGui_InputInt3);
            DENTY_ADD_INTERNAL_CALL(ImGui_InputInt4);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragFloat);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragFloat2);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragFloat3);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragFloat4);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragInt);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragInt2);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragInt3);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragInt4);
            DENTY_ADD_INTERNAL_CALL(ImGui_SliderFloat);
            DENTY_ADD_INTERNAL_CALL(ImGui_SliderFloat2);
            DENTY_ADD_INTERNAL_CALL(ImGui_SliderFloat3);
            DENTY_ADD_INTERNAL_CALL(ImGui_SliderFloat4);
            DENTY_ADD_INTERNAL_CALL(ImGui_SliderInt);
            DENTY_ADD_INTERNAL_CALL(ImGui_SliderInt2);
            DENTY_ADD_INTERNAL_CALL(ImGui_SliderInt3);
            DENTY_ADD_INTERNAL_CALL(ImGui_SliderInt4);
            DENTY_ADD_INTERNAL_CALL(ImGui_ColorEdit3);
            DENTY_ADD_INTERNAL_CALL(ImGui_ColorEdit4);
            DENTY_ADD_INTERNAL_CALL(ImGui_InvisibleButton);
            DENTY_ADD_INTERNAL_CALL(ImGui_BeginDragAndDropTarget);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragAndDropTargetID32);
            DENTY_ADD_INTERNAL_CALL(ImGui_DragAndDropTargetID64);
            DENTY_ADD_INTERNAL_CALL(ImGui_EndDragAndDropTarget);
            DENTY_ADD_INTERNAL_CALL(ImGui_Separator);
            DENTY_ADD_INTERNAL_CALL(ImGui_SameLine);
            DENTY_ADD_INTERNAL_CALL(ImGui_Indent);
            DENTY_ADD_INTERNAL_CALL(ImGui_UnIndent);
            DENTY_ADD_INTERNAL_CALL(ImGui_PushStyleColor);
            DENTY_ADD_INTERNAL_CALL(ImGui_PopStyleColor);
            DENTY_ADD_INTERNAL_CALL(ImGui_PushStyleVar);
            DENTY_ADD_INTERNAL_CALL(ImGui_PushStyleVarParam2);
            DENTY_ADD_INTERNAL_CALL(ImGui_PopStyleVar);
            DENTY_ADD_INTERNAL_CALL(ImGui_GetContentRegionAvail);
        }

        // Timer
        {
            DENTY_ADD_INTERNAL_CALL(Timer_GetDeltaTime);
        }

        // Math
        {
            DENTY_ADD_INTERNAL_CALL(Math_ApproxEqual);
            DENTY_ADD_INTERNAL_CALL(Math_ToRadian);

            DENTY_ADD_INTERNAL_CALL(Vector3_Dot);
            DENTY_ADD_INTERNAL_CALL(Vector3_AngleBetweenNormals);
            DENTY_ADD_INTERNAL_CALL(Vector3_Cross);

            DENTY_ADD_INTERNAL_CALL(Matrix_Transpose);
            DENTY_ADD_INTERNAL_CALL(Matrix_Inverse);
            DENTY_ADD_INTERNAL_CALL(Matrix_Multiply);
            DENTY_ADD_INTERNAL_CALL(Matrix_TransformCoord);
            DENTY_ADD_INTERNAL_CALL(Matrix_TransformNormal);
            DENTY_ADD_INTERNAL_CALL(Matrix_ObtainEulerAngles);
            DENTY_ADD_INTERNAL_CALL(Matrix_ObtainScale);
            DENTY_ADD_INTERNAL_CALL(Matrix_RotationAxis);
            DENTY_ADD_INTERNAL_CALL(Matrix_RotationQuaternion);
            DENTY_ADD_INTERNAL_CALL(Matrix_RotationRollPitchYaw);
            DENTY_ADD_INTERNAL_CALL(Matrix_Scaling);

            DENTY_ADD_INTERNAL_CALL(Quaternion_Slerp);
            DENTY_ADD_INTERNAL_CALL(Quaternion_RotationAxis);
            DENTY_ADD_INTERNAL_CALL(Quaternion_RotationRollPitchYaw);
            DENTY_ADD_INTERNAL_CALL(Quaternion_ToMatrix);
            DENTY_ADD_INTERNAL_CALL(Quaternion_Multiply);
        }

        // Physics
        {
            DENTY_ADD_INTERNAL_CALL(Physics_GetGravity);
        }

        // Scene
        {
            DENTY_ADD_INTERNAL_CALL(Scene_FindGameObjectByID);
            DENTY_ADD_INTERNAL_CALL(Scene_FindGameObjectByTag);
            DENTY_ADD_INTERNAL_CALL(Scene_FindGameObjectsByTag);
            DENTY_ADD_INTERNAL_CALL(Scene_GetGameObjects);

            DENTY_ADD_INTERNAL_CALL(RuntimeSceneManager_ChangeSceneFromIndex);
            DENTY_ADD_INTERNAL_CALL(RuntimeSceneManager_ChangeSceneFromName);
            DENTY_ADD_INTERNAL_CALL(EditorSceneManager_FindGameObjectsOfSceneByName);
            DENTY_ADD_INTERNAL_CALL(EditorSceneManager_FindGameObjectsByTagOfSceneByName);
            DENTY_ADD_INTERNAL_CALL(EditorSceneManager_GetActiveSceneName);
        }

        // GameObject
        {
            DENTY_ADD_INTERNAL_CALL(GameObject_GetName);
            DENTY_ADD_INTERNAL_CALL(GameObject_SetName);
            DENTY_ADD_INTERNAL_CALL(GameObject_GetTag);
            DENTY_ADD_INTERNAL_CALL(GameObject_GetEnabled);
            DENTY_ADD_INTERNAL_CALL(GameObject_SetEnabled);

            DENTY_ADD_INTERNAL_CALL(GameObject_GetComponents);
            DENTY_ADD_INTERNAL_CALL(GameObject_HasComponent);
        }

        // Component
        {
            DENTY_ADD_INTERNAL_CALL(Component_SetEnabled);
            DENTY_ADD_INTERNAL_CALL(Component_GetEnabled);

            // Transform
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetPosition);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetLocalPosition);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_SetLocalPosition);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_SetLocalEulerAngles);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetLocalEulerAngles);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_SetLocalScale);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetLocalScale);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetRight);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetUp);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetFront);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetWorld);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_SetWorld);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetLocal);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_SetLocal);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_GetUpdate);
            DENTY_ADD_INTERNAL_CALL(TransformComponent_SetUpdate);

            // GameCamera
            DENTY_ADD_INTERNAL_CALL(GameCameraComponent_SetFocusPosition);
            DENTY_ADD_INTERNAL_CALL(GameCameraComponent_SetViewProjection);
            DENTY_ADD_INTERNAL_CALL(GameCameraComponent_SetView);
            DENTY_ADD_INTERNAL_CALL(GameCameraComponent_SetProjection);
            DENTY_ADD_INTERNAL_CALL(GameCameraComponent_ComputeView);
            DENTY_ADD_INTERNAL_CALL(GameCameraComponent_ComputePerspectiveProjection);
            DENTY_ADD_INTERNAL_CALL(GameCameraComponent_ComputeOrthographicProjection);
            DENTY_ADD_INTERNAL_CALL(GameCameraComponent_UpdateMainCameraConstants);

            DENTY_ADD_INTERNAL_CALL(GameCamera_MainCamera_GetOrientation);

            //
            // RigidBody
            //
            DENTY_ADD_INTERNAL_CALL(RigidBodyComponent_SetLinearVelocity);

            DENTY_ADD_INTERNAL_CALL(RigidBodyComponent_EnableAngularVelocity);
            DENTY_ADD_INTERNAL_CALL(RigidBodyComponent_DisableAngularVelocity);

            DENTY_ADD_INTERNAL_CALL(RigidBodyComponent_AddForce);
            DENTY_ADD_INTERNAL_CALL(RigidBodyComponent_AddImpulse);

            DENTY_ADD_INTERNAL_CALL(RigidBodyComponent_GetLinearVelocity);
            DENTY_ADD_INTERNAL_CALL(RigidBodyComponent_SetGravity);

            DENTY_ADD_INTERNAL_CALL(RigidBodyComponent_GetRigidBodyWorld);

            DENTY_ADD_INTERNAL_CALL(RigidBodyComponent_SetPosition);

            //
            // VehicleComponent
            //

            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetActivate);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetVelocity);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_GetVelocity);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetLinearVelocity);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_GetLinearVelocity);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_ApplyCentralImpulse);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_ApplyForce);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetGravity);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetFront);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetOrientation);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_GetOrientation);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetPosition);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_GetPosition);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetMotionWorldTransform);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_GetMotionWorldTransform);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetWorldTransform);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_GetWorldTransform);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_GetHitWallNormal);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_Jump);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetJumpFlag);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_IsJump);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_IsDrifting);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_SetDrifting);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_ResetDrift);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_GetDriftDirection);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_GetOnDriftInputLX);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_OnGround);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_OnGrass);
            DENTY_ADD_INTERNAL_CALL(VehicleComponent_HitWall);

            //
            // Game
            //

            //
            // RaceGame
            //
            DENTY_ADD_INTERNAL_CALL(CheckPointComponent_GetDesireEntityPassingCheckPointCount);
            DENTY_ADD_INTERNAL_CALL(CheckPointComponent_GetNumber);
            DENTY_ADD_INTERNAL_CALL(CheckPointComponent_GetNextCheckPointNumber);
        }

        //
        // 2D Component
        //
        {
            DENTY_ADD_INTERNAL_CALL(UIComponent_GetItemType);
            DENTY_ADD_INTERNAL_CALL(UIComponent_GetPosition);
            DENTY_ADD_INTERNAL_CALL(UIComponent_GetScale);
            DENTY_ADD_INTERNAL_CALL(UIComponent_GetRotation);
            DENTY_ADD_INTERNAL_CALL(UIComponent_SetPosition);
            DENTY_ADD_INTERNAL_CALL(UIComponent_SetScale);
            DENTY_ADD_INTERNAL_CALL(UIComponent_SetRotation);
            DENTY_ADD_INTERNAL_CALL(UIComponent_GetRender);
            DENTY_ADD_INTERNAL_CALL(UIComponent_SetRender);

        	DENTY_ADD_INTERNAL_CALL(UIComponent_Text_GetText);
            DENTY_ADD_INTERNAL_CALL(UIComponent_Text_GetSpacing);
            DENTY_ADD_INTERNAL_CALL(UIComponent_Text_GetTextColor);
            DENTY_ADD_INTERNAL_CALL(UIComponent_Text_SetText);
            DENTY_ADD_INTERNAL_CALL(UIComponent_Text_SetSpacing);
            DENTY_ADD_INTERNAL_CALL(UIComponent_Text_SetTextColor);

            //
            // Image item.
            //
            DENTY_ADD_INTERNAL_CALL(UIComponent_Image_GetClipRect);
            DENTY_ADD_INTERNAL_CALL(UIComponent_Image_SetClipRect);
            DENTY_ADD_INTERNAL_CALL(UIComponent_Image_GetTextureSize);
        }

        //
        // Particle system
        //
        {
            DENTY_ADD_INTERNAL_CALL(CPUParticleSystem_Emit);
        }
    }
}
