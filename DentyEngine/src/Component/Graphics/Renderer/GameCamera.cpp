#include "stdafx.h"

#include "GameCamera.h"

#include "System/Core/Window.h"
#include "Component/Controller/DefaultCameraController.h"

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Sprite/Sprite3D.h"

#include "System/Events/EventRender.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Scene/Scene.h"

#include "Scene/GameObject.h"
#include "System/Core/Assertion.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

namespace DentyEngine
{
	Ref<GameCamera> GameCamera::_mainCamera = nullptr;

	GameCamera::GameCamera() :
		Camera(GraphicsManager::GetInstance().GetDevice()), Behavior(), _gizmoSprite(), _oldProjectionType()
	{
		_name = "GameCamera";

		_multipleAllowed = false;
	}

	GameCamera::GameCamera(const GameCamera& source) :
		Camera(source), Behavior(source), _oldProjectionType(source._oldProjectionType)
	{
		CreateGizmoSprite();
	}

	void GameCamera::Awake()
	{
		// Create perspective.
		{
			_perspective.aspectRatio = Window::GetInfo().GetSize<Vector2>().x / Window::GetInfo().GetSize<Vector2>().y;
			_perspective.distance = 10.0f;
			_perspective.fovAngleY = 50.0f;
			_perspective.nearZ = 0.1f;
			_perspective.farZ = 5000.0f;
			_perspective.up = Vector3::Up();
		}
		
		// Create orthographic.
		{
			_orthographic.distance = 10.0f;
			_orthographic.nearZ = 0.1f;
			_orthographic.farZ = 5000.0f;
			_orthographic.viewSize = Vector2(256, 256);
			_orthographic.up = Vector3::Up();
			_orthographic.aspectRatio = _orthographic.viewSize.x / _orthographic.viewSize.y;
		}

		// Create a camera with perspective projection.
		{
			Transform& transform = GetParent()->transform;

			CalculatePerspectiveProjection(_perspective);

			// Calculate camera and focus position.
			{
				transform.position += (Vector3::Front() * (-_perspective.distance));

				_focusPosition = Vector3::Front() * 2.0f;
			}

			CalculateView(transform.world, transform.position, {}, _perspective.up);

			_viewProjection = _view * _projection;
		}

		// Set main camera.
		{
			if (not _mainCamera)
			{
				if (GetParent()->tag() == "MainCamera")
				{
					_mainCamera = std::static_pointer_cast<GameCamera>(shared_from_this());
				}
			}
		}
	}

	void GameCamera::Start()
	{
		CreateGizmoSprite();
	}

	void GameCamera::OnUpdate([[maybe_unused]] const float deltaTime)
	{
	    {
			Transform& transform = GetParent()->transform;

			CalculateView(transform.local, transform.localPosition, _focusPosition, { 0, 1, 0 });

			switch (_projectionType)
			{
			case ProjectionType::Perspective:
				CalculatePerspectiveProjection(_perspective);

				break;
			case ProjectionType::Orthographic:
				CalculateOrthographicProjection(_orthographic);

				break;
			default:
				DENTY_ASSERT(false, "Unknown projection type.");
				break;
			}

			_viewProjection = _view * _projection;

			// Update constants.
			{
				_constants.view = _view;
				_constants.projection = _projection;
				_constants.viewProjection = _viewProjection;
				_constants.cameraPosition = Vector4(transform.localPosition, 1.0f);
			}
		}

	    {
		    if (_gizmoSprite)
		    {
				_gizmoSprite->Update(GetParent()->transform.world, Color::WHITE, GetParent()->GetEntityID());
		    }
	    }
	}

	void GameCamera::OnFixedUpdate()
	{
		Behavior::OnFixedUpdate();
	}

	void GameCamera::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventGameBeginRenderingProcess>(BIND_EVENT_FUNCTION(OnGameBeginRenderingProcess));
		dispatcher.Dispatch<EventGameEndRenderingProcess>(BIND_EVENT_FUNCTION(OnGameEndRenderingProcess));
		dispatcher.Dispatch<EventOnGameViewSizeChanged>(BIND_EVENT_FUNCTION(OnViewSizeChanged));
	}

	void GameCamera::OnGui()
	{
		_oldProjectionType = _projectionType;

		ImGui::RadioButton("Perspective", reinterpret_cast<int*>(&_projectionType), static_cast<int>(ProjectionType::Perspective));
		ImGui::SameLine();
		ImGui::RadioButton("Orthographic", reinterpret_cast<int*>(&_projectionType), static_cast<int>(ProjectionType::Orthographic));

		// When the projection method is changed.
		if (_oldProjectionType != _projectionType)
		{
			OnProjectionChanged(_projectionType);
		}

		switch (_projectionType)
		{
		case ProjectionType::Perspective:
		{
			ImGui::DragFloat("Fov", &_perspective.fovAngleY, 1, 45.0f, 120.0f);
			ImGui::DragFloat("Distance", &_perspective.distance, 1, 0.1f, 20.0f);
			ImGui::DragFloat("NearZ", &_perspective.nearZ, 1, 0.1f, 5000.0f);
			ImGui::DragFloat("FarZ", &_perspective.farZ,1, 0.1f, 5000.0f);
			ImGui::InputFloat3("Up", &_perspective.up.x, "%f", ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat("AspectRatio:", &_perspective.aspectRatio, 0.0f, 0.0f, "%f", ImGuiInputTextFlags_ReadOnly);
		}
		break;
		case ProjectionType::Orthographic:
		{
			ImGui::DragFloat("ViewWidth", &_orthographic.viewSize.x, 1, 0.1f, 2048.0f);
			ImGui::DragFloat("ViewHeight", &_orthographic.viewSize.y, 1, 0.1f, 2048.0f);
			ImGui::DragFloat("NearZ", &_orthographic.nearZ, 1, 0.1f, 5000.0f);
			ImGui::DragFloat("FarZ", &_orthographic.farZ, 1, 0.1f, 5000.0f);
			ImGui::InputFloat3("Up", &_orthographic.up.x, "%f", ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat("AspectRatio:", &_orthographic.aspectRatio, 0.0f, 0.0f, "%f", ImGuiInputTextFlags_ReadOnly);
		}
		break;
		default:
			DENTY_ASSERT(false, L"Unknown projection type. (in Camera::OnGui)");
			break;
		}
	}

	void GameCamera::OnGizmoRender()
	{
		if (_gizmoSprite)
		{
			if (!GetParent()->HasComponent("DirectionalLight"))
			{
				_gizmoSprite->Render();
			}
		}
	}

	void GameCamera::OnSerialized() const
	{
	}

	void GameCamera::OnDeserialized()
	{
	}

	void GameCamera::OnDestroy()
	{
		Behavior::OnDestroy();

		if (GetParent()->tag() == "MainCamera")
		{
			_mainCamera = nullptr;
		}
	}

	void GameCamera::OnGameObjectDeserialized()
	{
	}

	Ref<Object> GameCamera::MakeCopy() const
	{
		return std::make_shared<GameCamera>(*shared_from_this());
	}

	void GameCamera::OnGameBeginRenderingProcess(EventGameBeginRenderingProcess* e)
	{
		if (_mainCamera)
		{
		    _mainCamera->_constantBuffer->UpdateSubresource(e->_context, &_mainCamera->_constants);
		    _mainCamera->_constantBuffer->BindAll(e->_context, static_cast<UINT>(ConstantBuffer::ReservedBindSlot::Camera));
		}
	}

	void GameCamera::OnGameEndRenderingProcess([[maybe_unused]] EventGameEndRenderingProcess* e)
	{
	}

	void GameCamera::OnViewSizeChanged(EventOnGameViewSizeChanged* e)
	{
		// Call base class function.
		OnResize(e->GetSize());
	}

    void GameCamera::SetMainCamera(Ref<GameCamera> camera)
    {
		DENTY_CHECK_NULL(camera)

		if (_mainCamera)
		{
		    DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Main camera already set!";

			return;
		}

		_mainCamera = camera;
    }

	void GameCamera::ResetMainCamera()
	{
		_mainCamera = nullptr;
	}

	void GameCamera::UpdateMainCameraConstants(const Constants& constants)
	{
		_mainCamera->_constants.view = constants.view;
		_mainCamera->_constants.inverseView = constants.inverseView;
		_mainCamera->_constants.projection = constants.projection;
		_mainCamera->_constants.inverseProjection = constants.inverseProjection;
		_mainCamera->_constants.viewProjection = constants.viewProjection;
		_mainCamera->_constants.cameraPosition = constants.cameraPosition;
	}

	void GameCamera::CreateGizmoSprite()
	{
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const std::optional<Ref<Texture2D>> texture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::CAMERA_GIZMO_FILE_PATH);
		DENTY_CHECK_CONDITION(texture.has_value());

		_gizmoSprite = std::make_shared<Sprite3D>();
		_gizmoSprite->Create(texture.value());
	}
}
