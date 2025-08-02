#include "stdafx.h"

//#include "System/Events/EventEditor.h"
//
//#include "EditorPostEffectWindow.h"
//
//#include "Graphics/Buffer/ColorBuffer.h"
//#include "Graphics/Buffer/ConstantBuffer.h"
//
//#include "Graphics/Mesh/Mesh.h"
//#include "Graphics/PostEffect/PostEffect.h"
//
//#include "Graphics/Renderer/Camera.h"
//
//#include "System/Graphics/GraphicsManager.h"
//#include "System/Graphics/PostEffect/PostEffectGenerator.h"
//
//#include "System/Resource/Library/LibraryManager.h"
//#include "System/Resource/Library/MeshLibrary.h"
//#include "System/Resource/Library/ShaderLibrary.h"
//#include "Utilities/ImGuiUtilities.h"
//
//namespace DentyEngine
//{
//	const int EditorPostEffectWindow::MAKE_UNIQUE_NAME_TRIAL_COUNT_MAX = 100;
//
//	EditorPostEffectWindow::EditorPostEffectWindow() :
//		EditorWindow(), IEditor(), _previewWindowInfo(), _parameterWindowInfo(), _pressedAddPostEffectButton(), _generatedScene()
//	{
//
//	}
//
//	void EditorPostEffectWindow::OnOpened(Event* e)
//	{
//		_open = true;
//
//		GenerateScene();
//	}
//
//	void EditorPostEffectWindow::OnClosed(Event* e)
//	{
//		_open = false;
//		_generatedScene = false;
//	}
//
//	void EditorPostEffectWindow::GenerateScene()
//	{
//		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();
//
//		// Create scene buffer.
//		{
//			_previewWindowInfo.sceneBuffer = std::make_shared<FrameBuffer>();
//			_previewWindowInfo.sceneBuffer->Create(device, _previewWindowInfo.screenSize);
//		}
//
//		// Find mesh and replace shader from "Mesh.Shader" to "PostEffectMesh.Shader".
//		{
//			const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
//
//			_previewWindowInfo.planeMesh = meshLibrary->Find<StaticMesh>(MeshLibrary::PLANE_FILE_PATH).value();
//			_previewWindowInfo.sphereMesh = meshLibrary->Find(MeshLibrary::SPHERE_FILE_PATH).value();
//
//			// Replace
//			{
//				const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();
//
//				const auto shader = shaderLibrary->Find(ShaderLibrary::POST_EFFECT_MESH_SHADER_PATH).value();
//
//				_previewWindowInfo.planeMesh->GetMainMaterial()->shader = shader;
//				_previewWindowInfo.sphereMesh->GetMainMaterial()->shader = shader;
//			}
//		}
//
//		// Create camera.
//		{
//			//ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();
//
//			//_previewWindowInfo.camera = std::make_unique<Camera>(device);
//
//			//// Perspective.
//			//Camera::Perspective perspective = {};
//			//perspective.fovAngleY = 45.0f;
//			//perspective.aspectRatio = _previewWindowInfo.screenSize.x / _previewWindowInfo.screenSize.y;
//			//perspective.nearZ = 0.01f;
//			//perspective.farZ = 2000.0f;
//			//perspective.distance = 5.0f;
//
//			//_previewWindowInfo.camera->CalculatePerspectiveProjection(
//			//	perspective
//			//);
//
//			//_previewWindowInfo.camera->CalculateView(
//			//	Vector3(0, 5.0f, -perspective.distance),
//			//	Vector3::Zero(),
//			//	Vector3::Up()
//			//);
//
//			//Camera::Constants constants = { };
//			//constants.view = _previewWindowInfo.camera->GetView();
//			//constants.projection = _previewWindowInfo.camera->GetProjection();
//			//constants.viewProjection = _previewWindowInfo.camera->GetView() * _previewWindowInfo.camera->GetProjection();
//			//constants.cameraPosition = _previewWindowInfo.camera->transform.world.GetOffset();
//
//			//_previewWindowInfo.camera->GetConstantBuffer()->UpdateSubresource(context, &constants);
//			//_previewWindowInfo.camera->GetConstantBuffer()->BindAll(context, static_cast<UINT>(ConstantBuffer::ReservedBindSlot::PostEffectCamera));
//		}
//
//		_generatedScene = true;
//	}
//
//	void EditorPostEffectWindow::Update()
//	{
//		const int postEffectSize = _parameterWindowInfo.postEffects.size();
//
//		for (int index = 0; index < postEffectSize; ++index)
//		{
//			auto&& postEffect = _parameterWindowInfo.postEffects.at(index);
//
//			postEffect->Process();
//
//			if (postEffectSize <= 1)
//				break;
//
//			// Is end of container.
//			const bool isEnd = (index + 1 >= postEffectSize);
//
//			// Set next post effect base buffer use processed buffer.
//			if (not isEnd)
//			{
//				auto&& nextPostEffect = _parameterWindowInfo.postEffects.at(index + 1);
//				nextPostEffect->SetBaseBuffer(postEffect->GetResultBuffer());
//			}
//		}
//	}
//
//	void EditorPostEffectWindow::OnEvent(Event* e)
//	{
//		EventDispatcher dispatcher(e);
//		dispatcher.Dispatch<EventOnPostEffectWindowOpened>(BIND_EVENT_FUNCTION(OnOpened));
//		dispatcher.Dispatch<EventOnPostEffectWindowClosed>(BIND_EVENT_FUNCTION(OnClosed));
//	}
//
//	void EditorPostEffectWindow::SceneRender()
//	{
//		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();
//
//		_previewWindowInfo.sceneBuffer->Clear(context, Color::GRAY);
//		_previewWindowInfo.sceneBuffer->Activate(context);
//
//		_previewWindowInfo.planeMesh->OnRender(context, _previewWindowInfo.planeMesh->GetMainMaterial(), _previewWindowInfo.planeMesh->GetMaterials(), nullptr);
//		_previewWindowInfo.sphereMesh->OnRender(context, _previewWindowInfo.sphereMesh->GetMainMaterial(), _previewWindowInfo.sphereMesh->GetMaterials(), nullptr);
//
//		_previewWindowInfo.sceneBuffer->DeActivate(context);
//	}
//
//	void EditorPostEffectWindow::RemoveOnGui()
//	{
//
//	}
//
//	void EditorPostEffectWindow::SwapOnGui()
//	{
//		
//	}
//
//	void EditorPostEffectWindow::AddPostEffectOnGui()
//	{
//		// Set button on center.
//		if (GuiUtils::ButtonCentered("AddPostEffect##EditorPostEffectWindow", MathConverter::ImVec2ToVector2(ImVec2(ImGui::GetContentRegionAvail().x, 28.0f))))
//		{
//			_pressedAddPostEffectButton = true;
//		}
//
//		// Show post effect list.
//		if (_pressedAddPostEffectButton)
//		{
//			std::string postEffectName = "";
//
//			PostEffectGenerator::ShowList(postEffectName, _pressedAddPostEffectButton);
//
//			// Contain name.
//			if (not postEffectName.empty())
//			{
//				// Generate post effect.
//				const auto postEffect = PostEffectGenerator::Generate(postEffectName);
//
//				// Doesn't find.
//				if (not postEffect)
//					return;
//
//				// Make unique name.
//				const auto uniqueName = MakeUniqueName(postEffect->GetName());
//
//				// If empty assert.
//				if (uniqueName.empty())
//				{
//					DENTY_ASSERT(false, "Failed to make unique name. (in EditorPostEffectWindow::AddPostEffectOnGui)");
//				}
//
//				// Set new name.
//				postEffect->SetName(uniqueName);
//
//				// Create
//				postEffect->Create(_previewWindowInfo.sceneBuffer, _previewWindowInfo.screenSize);
//
//				_parameterWindowInfo.postEffects.emplace_back(postEffect);
//			}
//		}
//	}
//
//	std::string EditorPostEffectWindow::MakeUniqueName(std::string_view targetName) const
//	{
//		for (int trialCount = 0; trialCount < MAKE_UNIQUE_NAME_TRIAL_COUNT_MAX; ++trialCount)
//		{
//			std::string name = std::string(targetName) + std::to_string(trialCount);
//
//			bool isDuplicate = false;
//			for (const auto& postEffect : _parameterWindowInfo.postEffects)
//			{
//				if (postEffect->GetName() == name)
//				{
//					isDuplicate = true;
//
//					break;
//				}
//			}
//
//			// doesn't duplicate.
//			if (not isDuplicate)
//				return name;
//		}
//
//		return "";
//	}
//
//	void EditorPostEffectWindow::OnGui(Scene* activeScene)
//	{
//		// If not opened, return.
//		if (not _open) return;
//		if (not _generatedScene) return;
//
//		// Plane mesh.
//		{
//			const Matrix S = DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f);
//			const Matrix R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
//			const Matrix T = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
//
//			const Matrix W = (S * R * T);
//
//			_previewWindowInfo.planeMesh->OnUpdate(W, 0u, _previewWindowInfo.planeMesh->GetMainMaterial(), 0.0f);
//		}
//
//		// Sphere mesh.
//		{
//			const Matrix S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
//			const Matrix R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
//			const Matrix T = DirectX::XMMatrixTranslation(0.0f, 1.5f, -1.0f);
//
//			const Matrix W = (S * R * T);
//
//			_previewWindowInfo.sphereMesh->OnUpdate(W, 0u, _previewWindowInfo.sphereMesh->GetMainMaterial(), 0.0f);
//		}
//
//		SceneRender();
//
//		ImGui::SetNextWindowSize(ImVec2(512.0f, 512.0f), ImGuiCond_Once);
//		if (ImGui::Begin("EditorPostEffectWindow##EditorWindow", &_open))
//		{
//			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();
//
//			ImGuiWindowFlags childWindowFlags = ImGuiWindowFlags_NoDocking;
//
//			// Parameter
//			if (ImGui::BeginChild("Parameter##EditorPostEffectWindow", ImVec2(ImGui::GetContentRegionAvail().x * 0.35f, 0.0f), childWindowFlags))
//			{
//				// Show post effect gui.
//				for (auto&& it = _parameterWindowInfo.postEffects.begin(); it != _parameterWindowInfo.postEffects.end();)
//				{
//					auto&& postEffect = (*it);
//
//					const auto label = postEffect->GetName();
//
//					bool removeButtonPressed = false;
//					if (ImGui::TreeNodeEx(label.c_str()))
//					{
//						// Remove button.
//						const auto removeButtonLabel = std::string("Remove") + "##" + label;
//
//						ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 64.0f);
//						if (ImGui::Button(removeButtonLabel.c_str()))
//						{
//							removeButtonPressed = true;
//						}
//
//						postEffect->OnGui();
//
//						ImGui::TreePop();
//					}
//
//					// Erase from vector.
//					if (removeButtonPressed)
//					{
//						it = _parameterWindowInfo.postEffects.erase(it);
//					}
//					else
//					{
//						++it;
//					}
//				}
//
//				// Add post effect button.
//				AddPostEffectOnGui();
//
//				ImGui::EndChild();
//			}
//
//			ImGui::SameLine();
//
//			// Preview
//			if (ImGui::BeginChild("Preview##EditorPostEffectWindow", ImVec2(), childWindowFlags))
//			{
//				{
//					_previewWindowInfo.previousScreenSize = _previewWindowInfo.screenSize;
//					_previewWindowInfo.screenSize = MathConverter::ImVec2ToVector2(ImGui::GetContentRegionAvail());
//
//					// If child window size changed, resize buffer and camera.
//					if (_previewWindowInfo.screenSize != _previewWindowInfo.previousScreenSize)
//					{
//						// Check is minus.
//						if (_previewWindowInfo.screenSize.x > 0.0f && _previewWindowInfo.screenSize.y > 0.0f)
//						{
//							ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();
//
//							_previewWindowInfo.sceneBuffer->OnResize(device, _previewWindowInfo.screenSize);
//							_previewWindowInfo.camera->OnResize(_previewWindowInfo.screenSize);
//
//							Camera::Constants constants = { };
//							constants.view = _previewWindowInfo.camera->GetView();
//							constants.projection = _previewWindowInfo.camera->GetProjection();
//							constants.viewProjection = _previewWindowInfo.camera->GetViewProjection();
//							//constants.cameraPosition = _previewWindowInfo.camera->transform.world.GetOffset();
//
//							// Update constant buffer.
//							_previewWindowInfo.camera->GetConstantBuffer()->UpdateSubresource(context, &constants);
//							_previewWindowInfo.camera->GetConstantBuffer()->BindAll(context, static_cast<UINT>(ConstantBuffer::ReservedBindSlot::PostEffectCamera));
//						}
//					}
//				}
//
//				// Preview
//				{
//					const int postEffectSize = _parameterWindowInfo.postEffects.size();
//
//					ID3D11ShaderResourceView* srv = nullptr;
//
//					if (postEffectSize <= 0)
//					{
//						srv = *_previewWindowInfo.sceneBuffer->GetColorBuffer()->GetResource();
//					}
//					else
//					{
//						const auto& finalPass = _parameterWindowInfo.postEffects.at(postEffectSize - 1);
//						srv = *finalPass->GetResultBuffer()->GetColorBuffer()->GetResource();
//					}
//
//					ImGui::Image(srv, ImGui::GetContentRegionAvail());
//				}
//
//				ImGui::EndChild();
//			}
//		}
//
//		ImGui::End();
//
//		// Check is window closed.
//		if (not _open)
//		{
//			EventOnPostEffectWindowClosed eventOnPostEffectWindowClosed;
//			NOTIFY_EVENT(eventOnPostEffectWindowClosed);
//		}
//	}
//}
