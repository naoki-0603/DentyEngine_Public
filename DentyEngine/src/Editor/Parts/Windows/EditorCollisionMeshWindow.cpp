#include "stdafx.h"

#include "EditorCollisionMeshWindow.h"

#include "System/Core/Timer.h"

#include "Physics/CollisionMesh.h"

#include "Editor/Graphics/Rendering/EditorCamera.h"

#include "Editor/Objects/CollisionPreviewObject.h"

#include "Graphics/Buffer/ColorBuffer.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Buffer/SceneBuffer.h"

#include "Graphics/Mesh/StaticMesh.h"
#include "Graphics/Renderer/LineRenderer.h"

#include "Physics/ApproximationCollisionMesh.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"
#include "System/Resource/Library/ShaderLibrary.h"

#include "System/Events/Event.h"
#include "System/Events/EventEditor.h"
#include "System/Events/EventRender.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Serializer/SerializeExtensions.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	const Color EditorCollisionMeshWindow::OBJECT_NAME_COLOR_COLLIDER_EXISTS = Color::GREEN;
	const Color EditorCollisionMeshWindow::OBJECT_NAME_COLOR_COLLIDER_DOES_NOT_EXISTS = Color::RED;

	EditorCollisionMeshWindow::EditorCollisionMeshWindow() :
		IEditor(), EditorWindow(), _approximationCollisionMesh(), _collisionMesh(), _collisionPreviewObjects(),
		_staticMesh(), _logs(), _editorCamera(), _sceneBuffer(), _previousWindowSize(),
		_currentWindowSize(), _selectedIndex(UINT32_MAX), _prevFilterType(), _filterType(), _hierarchyOpen(), _inspectorOpen(), _isViewHovered()
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

		// Create editor camera.
		{
			Camera::Perspective perspective = {};

			// Perspective Settings
			{
				perspective.nearZ = 0.1f;
				perspective.farZ = 2000.0f;
				perspective.up = Vector3::Up();
				perspective.fovAngleY = 45.0f;
				perspective.distance = 1.0f;

				constexpr Vector2 size = COLLISION_EDITOR_VIEW_AREA_DEFAULT_SIZE;
				perspective.aspectRatio = size.x / size.y;
			}

			_editorCamera = std::make_unique<EditorCamera>(device, perspective);
		}

		// Create scene buffer.
		{
			_sceneBuffer = std::make_unique<SceneBuffer>();
			_sceneBuffer->Create(device, COLLISION_EDITOR_VIEW_AREA_DEFAULT_SIZE);
		}

		_currentWindowSize = COLLISION_EDITOR_DEFAULT_SIZE;
		_previousWindowSize = _currentWindowSize;
	}

	void EditorCollisionMeshWindow::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnCollisionMeshWindowOpened>(BIND_EVENT_FUNCTION(OnOpened));
		dispatcher.Dispatch<EventOnCollisionMeshWindowClosed>(BIND_EVENT_FUNCTION(OnClosed));
	}

	void EditorCollisionMeshWindow::OnGui(Ref<Scene> activeScene)
	{
		if (not _open) return;
		if (not _collisionMesh) return;

		if (_editorCamera->IsMoving() || _isViewHovered)
		{
			_editorCamera->UpdateInput(Timer::GetDeltaTime());
		}

		_editorCamera->OnUpdate();

		// Gui
		{
			if (ImGui::Begin("EditorCollisionMeshWindow##EditorCollisionMeshWindow", &_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
			{
				_isViewHovered = (ImGui::IsWindowHovered());

				ViewWindow();
				HierarchyWindow();
				InspectorWindow();
				ConsoleWindow();

				// Update mesh.
				{
					for (uint32_t index = 0u; index < _staticMesh->GetSubMeshCount(); ++index)
					{
						auto&& subMesh = _staticMesh->GetSubMeshes().at(index);

						subMesh->OnUpdate(Matrix::Identity(), (index + 1u), subMesh->GetMainMaterial(), Timer::GetDeltaTime());
					}
				}

				// Render scene.
				{
					ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

					// Begin
					{
						EventEditorBeginRenderingProcess eventEditorBeginRenderingProcess(context, static_cast<uint32_t>(ConstantBuffer::ReservedBindSlot::CollisionVisualizerCamera));
						_editorCamera->OnBeginRendering(&eventEditorBeginRenderingProcess);

						_sceneBuffer->Clear(context, Color::BLACK);
						_sceneBuffer->Activate(context);
					}

					// Rendering
					{
						// Render sub meshes.
						{
							for (uint32_t index = 0u; index < _staticMesh->GetSubMeshCount(); ++index)
							{
								const Ref<StaticMesh>& subMesh = std::static_pointer_cast<StaticMesh>(_staticMesh->GetSubMeshes().at(index));

								subMesh->OnRender(context, subMesh->GetMainMaterial(), subMesh->GetMaterials());
							}
						}

						// Render selected sub mesh.
						{
							LineRenderer::Flush(true);
						}
					}

					// End
					{
						_sceneBuffer->DeActivate(context);

						_editorCamera->OnEndRendering(nullptr);
					}
				}
			}
		}
		ImGui::End();

		// If closed.
		if (not _open)
		{
			EventOnCollisionMeshWindowClosed eventOnCollisionMeshWindowClosed(_collisionMesh->GetMeshID());
			NOTIFY_EVENT(eventOnCollisionMeshWindowClosed);
		}
	}

	void EditorCollisionMeshWindow::OnOpened(Event* e)
	{
		// If already opened
		if (_open)
		{
			// Notify closed event.
			EventOnCollisionMeshWindowClosed eventOnCollisionMeshWindowClosed(_collisionMesh->GetMeshID());
			NOTIFY_EVENT(eventOnCollisionMeshWindowClosed);
		}

		_open = true;
		_hierarchyOpen = true;
		_inspectorOpen = true;

		_collisionMesh = static_cast<EventOnCollisionMeshWindowOpened*>(e)->GetCollisionMesh();

		// Create collision preview objects.
		{
			for (uint32_t index = 0u; index < _collisionMesh->GetSubMeshCount(); ++index)
			{
				Ref<CollisionPreviewObject>& collisionPreviewObject = _collisionPreviewObjects.emplace_back();
				collisionPreviewObject = std::make_shared<CollisionPreviewObject>();

				const auto subMesh = std::static_pointer_cast<CollisionMesh>(_collisionMesh->GetSubMeshes().at(index));
				collisionPreviewObject->Create(subMesh, subMesh->GetName(), index, MESH_COLLIDER_MAX_VERTICES_COUNT);
			}
		}

		// Find static mesh.
		{
			const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
			String filePath = _collisionMesh->GetFilePathToSerialized().GetAsString();
			filePath.RemoveExtension();
			filePath.Append("_Static");
			filePath.Append(MESH_FILE_EXTENSION);

			const auto staticMesh = meshLibrary->Find<StaticMesh>(filePath.Get());
			if (!staticMesh.has_value())
			{
				DENTY_ASSERT(false, "Static mesh not found! (in EditorCollisionMeshWindow::OnOpened)");
			}

			_staticMesh = staticMesh.value();
		}

		// Replace from static mesh shader to collision visualizer mesh shader.
		{
			const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

			const std::optional<Ref<Shader>> collisionVisualizerMeshShader = shaderLibrary->Find(ShaderLibrary::COLLISION_VISUALIZER_MESH_SHADER_PATH);
			if (!collisionVisualizerMeshShader.has_value())
			{
				DENTY_ASSERT(false, "Shader not found! (in EditorCollisionMeshWindow::OnOpened)");
			}

			_staticMesh->ReplaceMainMaterialShaders(collisionVisualizerMeshShader.value());
		}

		ImGui::SetNextWindowSize(COLLISION_EDITOR_DEFAULT_SIZE.ToImVec2());
	}

	void EditorCollisionMeshWindow::OnClosed([[maybe_unused]] Event* e)
	{
		// Reset original shader.
		{
			const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

			const std::optional<Ref<Shader>> staticMeshShader = shaderLibrary->Find(ShaderLibrary::STATIC_MESH_SHADER_PATH);
			if (!staticMeshShader.has_value())
			{
				DENTY_ASSERT(false, "Shader not found! (in EditorCollisionMeshWindow::OnClosed)");
			}

			_staticMesh->ReplaceMainMaterialShaders(staticMeshShader.value());
		}

		Reset();
	}

	void EditorCollisionMeshWindow::ViewWindow()
	{
		_previousWindowSize = _currentWindowSize;

		_currentWindowSize = Vector2::ToVector2(ImGui::GetWindowSize());

		// Resize
		if (_sceneBuffer)
		{
			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			if (_previousWindowSize != _currentWindowSize)
			{
				if (!_currentWindowSize.IsContainMinus())
				{
					const Vector2 viewSize = _currentWindowSize * 0.5f;

					_sceneBuffer->OnResize(device, viewSize);
					_editorCamera->OnResize(viewSize);
				}
			}
		}

		const ColorBuffer* colorBuffer = _sceneBuffer->GetColorBuffer();
		if (colorBuffer->IsValid())
		{
			const ImTextureID texture = *colorBuffer->GetResource();

			ImGui::Image(texture, _currentWindowSize.ToImVec2());
		}
	}

	void EditorCollisionMeshWindow::HierarchyWindow()
	{
		if (ImGui::Begin("CollisionMeshWindowHierarchy##EditorCollisionMeshWindow", &_hierarchyOpen, ImGuiWindowFlags_NoMove))
		{
			const ImVec2 regionAvail = ImGui::GetContentRegionAvail();
			if (GuiUtils::ButtonCentered("Export##EditorCollisionMeshWindow", Vector2(regionAvail.x, 32.0f)))
			{
				std::string reason = {};
				if (IsExportable(reason))
				{
					// Save approximation collision mesh asset.
					{
						Copy();
					}

					// Add log
					{
						Log log = {};
						log.color = Color::GREEN;
						log.text = "Succeed to export approximation collision mesh asset!";

						_logs.emplace_back(log);
					}
				}
				else
				{
					// Add log
					{
						Log log = {};
						log.color = Color::RED;
						log.text = reason;

						_logs.emplace_back(log);
					}
				}
			}

			ImGui::Separator();

			// Filter
			if (ImGui::CollapsingHeader("Filter##CollisionMeshWindowHierarchy", ImGuiTreeNodeFlags_DefaultOpen))
			{
				_prevFilterType = _filterType;

				const String none = String(NAMEOF_ENUM(FilterType::None)).CStr();
				const String colliderExists = String(NAMEOF_ENUM(FilterType::ColliderExists)).CStr();
				const String colliderDoesNotExists = String(NAMEOF_ENUM(FilterType::ColliderDoesNotExists)).CStr();

				auto filterType = static_cast<int32_t>(_filterType);
				ImGui::RadioButton(none.CStr(), &filterType, 0);
				ImGui::RadioButton(colliderExists.CStr(), &filterType, 1);
				ImGui::RadioButton(colliderDoesNotExists.CStr(), &filterType, 2);
				_filterType = static_cast<FilterType>(filterType);
			}

			if (ImGui::CollapsingHeader("Objects##CollisionMeshWindowHierarchy", ImGuiTreeNodeFlags_DefaultOpen))
			{
				uint32_t selectedIndex = UINT32_MAX;
				for (auto&& collisionPreviewObject : _collisionPreviewObjects)
				{
					if (collisionPreviewObject->HasCollider())
						ImGui::PushStyleColor(ImGuiCol_Text, OBJECT_NAME_COLOR_COLLIDER_EXISTS.color.ToImVec4());
					else
						ImGui::PushStyleColor(ImGuiCol_Text, OBJECT_NAME_COLOR_COLLIDER_DOES_NOT_EXISTS.color.ToImVec4());

					switch (_filterType)
					{
					case FilterType::None:
						{
							selectedIndex =	collisionPreviewObject->OnGui();
						}
						break;
					case FilterType::ColliderExists:
						{
							if (collisionPreviewObject->HasCollider())
							{
								selectedIndex = collisionPreviewObject->OnGui();
							}
						}
						break;
					case FilterType::ColliderDoesNotExists:
						{
							if (!collisionPreviewObject->HasCollider())
							{
								selectedIndex = collisionPreviewObject->OnGui();
							}
						}
						break;
					}

					ImGui::PopStyleColor();

					if (selectedIndex != UINT32_MAX)
						break;
				}

				// Set selected index and editor camera focus position.
				if (selectedIndex != UINT32_MAX)
				{
					_selectedIndex = selectedIndex;

					// Set focus position.
					{
						auto&& object = _collisionPreviewObjects.at(_selectedIndex);

						_editorCamera->SetFocusPosition(object->GetPositionInWorld());
					}
				}
			}
		}

		ImGui::End();
	}

	void EditorCollisionMeshWindow::InspectorWindow()
	{
		if (ImGui::Begin("CollisionMeshWindowInspector##EditorCollisionMeshWindow", &_inspectorOpen, ImGuiWindowFlags_NoMove))
		{
			if (_selectedIndex != UINT32_MAX)
			{
				auto&& previewObject = _collisionPreviewObjects.at(_selectedIndex);

				previewObject->OnInspectorGui();

				if (previewObject->HasCollider())
				{
					previewObject->Update();
				}
			}
		}

		ImGui::End();
	}

	void EditorCollisionMeshWindow::ConsoleWindow()
	{
		if (ImGui::Begin("EditorCollisionMeshConsole##EditorCollisionMeshWindow"))
		{
			if (_logs.size() > MAX_LOG_COUNT)
				_logs.pop_front();

			for (const auto& log : _logs)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, log.color.color.ToImVec4());

				ImGui::Text("%s", log.text.c_str());

				ImGui::PopStyleColor();
			}
		}

		ImGui::End();
	}

	void EditorCollisionMeshWindow::Copy()
	{
		_approximationCollisionMesh = std::make_shared<ApproximationCollisionMesh>();
		_approximationCollisionMesh->Create({}, ShapeType::None, ApproximationCollisionMeshBulletObjectType::RigidBody, true);

		// Create name.
		{
			_approximationCollisionMesh->SetName(_collisionMesh->GetFilePath().Filename().RemoveExtension().GetAsString() + "_Approximation");
		}

		// Create file path.
		{
			FilePath filePath = _collisionMesh->GetFilePathToSerialized();
			String path = filePath.RemoveExtension().RemoveFileName().GetAsString();
			path.Append(_approximationCollisionMesh->GetName());
			path.Append(APPROXIMATION_COLLISION_MESH_FILE_EXTENSION);

			_approximationCollisionMesh->SetFilePath(path.Get());
			_approximationCollisionMesh->SetFilePathToSerialized(path.Get());
		}

		// Copy from vertices from collision meshes.
		{
			for (uint32_t index = 0u; index < _collisionPreviewObjects.size(); ++index)
			{
				const auto& object = _collisionPreviewObjects.at(index);

				const auto& collisionObject = _collisionMesh->GetSubMeshes().at(object->GetIndex());

				const auto& subMesh = std::static_pointer_cast<CollisionMesh>(collisionObject);

				auto&& approximation = std::make_shared<ApproximationCollisionMesh>();
				approximation->Create(subMesh->GetVertices(), object->GetShapeType(), object->GetBulletObjectType());
				approximation->SetName(_approximationCollisionMesh->GetName());
				approximation->SetFilePath(_approximationCollisionMesh->GetFilePath().GetAsString());
				approximation->SetFilePathToSerialized(_approximationCollisionMesh->GetFilePathToSerialized().GetAsString());

				_approximationCollisionMesh->AddSubMesh(approximation);
			}
		}

		// Save
		{
			_approximationCollisionMesh->Save();
		}

		// Add to mesh library.
		{
			MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
			meshLibrary->Add(_approximationCollisionMesh);
		}
	}

	void EditorCollisionMeshWindow::Reset()
	{
		_open = false;
		_hierarchyOpen = false;
		_inspectorOpen = false;

		_collisionPreviewObjects.clear();
		_collisionMesh = nullptr;
		_staticMesh = nullptr;
		_selectedIndex = 0u;
		_logs.clear();

		_prevFilterType = FilterType::None;
		_filterType = FilterType::None;
	}

	bool EditorCollisionMeshWindow::IsExportable(std::string& reason) const
	{
		// Check all sub mesh has collider.
		{
			for (const auto& collisionObject : _collisionPreviewObjects)
			{
				if (!collisionObject->HasCollider())
				{
					reason = "Must be all sub mesh has collider!";
					return false;
				}
			}
		}

		return true;
	}
}
