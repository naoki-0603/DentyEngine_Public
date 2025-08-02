#pragma once

// Macros
#define DENTY_USE_BULLET_PHYSICS (1)
#define DENTY_USE_ASSIMP		 (1)
#define DENTY_USE_FBX_SDK		 (0)
#define DENTY_USE_IMGUI			 (1)

// Standard library headers and windows headers
#pragma warning(push, 0)
#include <Windows.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <tchar.h>
#include <chrono>
#include <unordered_map>
#include <array>
#include <list>
#include <stack>
#include <deque>
#include <vector>
#include <memory>
#include <string>
#include <comdef.h>
#include <filesystem>
#include <fstream>
#include <stdarg.h>
#include <ranges>
#include <functional>

// DirectX11
#include <d3d11.h>
#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

// Externals

// Texture loaders.
#include <directxtk/WICTextureLoader.h>
#include <directxtk/DDSTextureLoader.h>
#include <directxtk/ScreenGrab.h>
#include <wincodec.h>

// ImGui
#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <imgui_ja_gryph_ranges.h>

// ImGuizmo
#include <ImGuizmo.h>

// Nameof
#include <nameof.hpp>

// Cereal
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/specialize.hpp>

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/stack.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#pragma warning(pop)

// BulletPhysics
// Right hand-side
#pragma warning(push)
#pragma warning(disable:26495)
#pragma warning(disable:26451)
#pragma warning(disable:4127)
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#pragma warning(pop)

#pragma warning(push, 0)
// XInput
#include <Xinput.h>

// XAudio
#include <xaudio2.h>

// P-log
#include <plog/Log.h>

// Assimp
#include <assimp/types.h>

#pragma warning(pop)

// DentyEngine
#include "Utilities/Utilities.h"

#include "System/Core/Assertion.h"

#include "Math/Vector.h"
#include "Math/VectorI.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

#include "System/Core/ImGuiConsole.h"
#include "System/Core/Constants.h"
#include "System/Core/ConsoleLogger.h"
#include "System/Input/Input.h"

// Undef
#undef min
#undef max
#undef far
#undef near