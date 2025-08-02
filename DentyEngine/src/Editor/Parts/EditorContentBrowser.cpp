#include "stdafx.h"

#include "EditorContentBrowser.h"

#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/SceneTree.h"
#include "Graphics/Mesh/Skeleton.h"
#include "Graphics/Renderer/Texture2D.h"

#include "System/Events/EventEditor.h"
#include "System/Events/EventScene.h"

#include "System/Resource/Prefab.h"
#include "System/Resource/Animations/AnimatorController.h"

#include "System/Resource/Library/AnimatorControllerLibrary.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MaterialLibrary.h"
#include "System/Resource/Library/MeshLibrary.h"
#include "System/Resource/Library/PrefabLibrary.h"
#include "System/Resource/Library/ShaderLibrary.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "System/Scene/Scene.h"

#include "System/Serializer/SerializeExtensions.h"

#include "System/Editor/EditorSceneManager.h"

#include "System/Resource/Library/AnimationClipLibrary.h"

#include "Scene/GameObject.h"

#include "Utilities/StringUtilities.h"

#include "System/Profiler/FunctionState.h"

#include "System/Resource/Library/SceneTreeLibrary.h"
#include "System/Resource/Library/SkeletonLibrary.h"

#include "Editor/RuntimeAssetCreator.h"

#include "Graphics/Skybox.h"

#include "Physics/ApproximationCollisionMesh.h"
#include "Physics/CollisionMesh.h"

#include "System/Core/ID/EntityID.h"
#include "System/Core/ID/PrefabID.h"
#include "System/Core/ID/SceneID.h"

#include "System/Editor/DragAndDrop/DragAndDropSource.h"
#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Layer/ApplicationLayer.h"

#include "System/Resource/Script.h"
#include "System/Resource/Library/SceneLibrary.h"
#include "System/Resource/Library/ScriptLibrary.h"
#include "System/Resource/Library/SkyboxLibrary.h"

// Reference
// https://github.com/TheCherno/Hazel/blob/master/Hazelnut/src/Panels/ContentBrowserPanel.cpp

namespace DentyEngine
{
	//
	// Constants.
	//
	const FilePath EditorContentBrowser::ENTRY_FILE_PATH = "Assets";

	const Vector2 EditorContentBrowser::BACK_ARROW_IMAGE_SIZE = { 16.0f, 16.0f };
	const Vector2 EditorContentBrowser::THUMBNAIL_SIZE = { 64.0f, 64.0f };
	const Vector2 EditorContentBrowser::PADDING = { 32.0f, 32.0f };
	const Vector2 EditorContentBrowser::CEIL_SIZE = PADDING + THUMBNAIL_SIZE;

	const Vector2 EditorContentBrowser::DOWN_SAMPLING_THUMBNAIL_SIZE = { 128.0f, 128.0f };

	namespace Utils
	{
        Asset::AssetType ObtainAssetTypeFromFilePath(const FilePath& filePath, bool& hasOriginalFormat)
        {
			using AssetType = Asset::AssetType;

			String filename = filePath.Stem().GetAsString();
			const auto extension = filePath.Extension();

			hasOriginalFormat = true;

			// Scene
            {
                if (extension == SCENE_FILE_EXTENSION)
					return AssetType::Scene;
            }

			// Mesh
			{
			    if (extension == MESH_FILE_EXTENSION)
					return AssetType::Mesh;
			}

			// Shader
			{
				if (extension == SHADER_FILE_EXTENSION)
					return AssetType::Shader;
			}

			// Material
			{
				if (extension == MATERIAL_FILE_EXTENSION)
					return AssetType::Material;
			}

			// CubeMaterial
			{
				if (extension == CUBE_MATERIAL_FILE_EXTENSION)
					return AssetType::CubeMaterial;
			}

			// Texture
			{
				if (extension == TEXTURE_FILE_EXTENSION)
					return AssetType::Texture;
			}

			// Avatar
			{
				if (extension == AVATAR_FILE_EXTENSION)
					return AssetType::Avatar;
			}

			// SceneTree
			{
				if (extension == SCENE_TREE_FILE_EXTENSION)
					return AssetType::SceneTree;
			}

        	// Collision mesh
			{
				if (extension == COLLISION_MESH_FILE_EXTENSION)
					return AssetType::CollisionMesh;
			}

			// Approximation collision mesh
			{
				if (extension == APPROXIMATION_COLLISION_MESH_FILE_EXTENSION)
					return AssetType::ApproximationCollisionMesh;
			}

			// Animation clip
			{
			    if (extension == ANIMATION_FILE_EXTENSION)
					return AssetType::Animation;
			}

			// Animator controller
			{
				if (extension == ANIMATOR_CONTROLLER_FILE_EXTENSION)
					return AssetType::Controller;
			}

			// Prefab
			{
				if (extension == PREFAB_FILE_EXTENSION)
					return AssetType::Prefab;
			}

			// Skybox
			{
				if (extension == SKYBOX_FILE_EXTENSION)
					return AssetType::Skybox;
			}

			// Script
			{
				if (extension == SCRIPT_FILE_EXTENSION)
					return AssetType::Script;
			}

			hasOriginalFormat = false;

			return AssetType::None;
        }
	}

	EditorContentBrowserTextures::EditorContentBrowserTextures() :
		folderEmpty(), folderFileExists(), backArrow()
	{
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		folderEmpty = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::CONTENT_BROWSER_FOLDER_EMPTY_FILE_PATH).value();
		folderFileExists = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::CONTENT_BROWSER_FOLDER_FILE_PATH).value();
		backArrow = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::CONTENT_BROWSER_BACK_FILE_PATH).value();
	}

	//
	// Member functions.
	//
	EditorContentBrowser::EditorContentBrowser([[maybe_unused]] ID3D11Device* device) :
		IEditor(), _previewPhotos(), _editorContentBrowserTextures(),
		_currentDirectory(), _previousDirectory(), _isSceneClicked()
	{
		_currentDirectory = ENTRY_FILE_PATH;
		_previousDirectory = _currentDirectory;
	}

	EditorContentBrowser::~EditorContentBrowser()
	{
	}

	void EditorContentBrowser::OnEvent([[maybe_unused]] Event* e)
	{
	}
	
	void EditorContentBrowser::OnGui(Ref<Scene> activeScene)
	{
		const FunctionStateSpecification specification =
		{
			"EditorContentBrowser::OnGui"
		};
		FunctionState functionState(specification);

		ImGuiWindowFlags windowFlags = {};
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBackground;

		static constexpr ImVec2 LOCAL_BACK_ARROW_IMAGE_SIZE = { 16.0f, 16.0f };

		// Constants for image.
		const ImTextureID emptyDirectoryImage = *_editorContentBrowserTextures.folderEmpty->GetResource();
		const ImTextureID fileExistsDirectoryImage = *_editorContentBrowserTextures.folderFileExists->GetResource();
		const ImTextureID backArrowImage = *_editorContentBrowserTextures.backArrow->GetResource();

		static bool open = true;

		// Open window.
		if (ImGui::Begin("ContentBrowser##Editor", &open, windowFlags))
		{
			if (ApplicationLayer::IsPlay())
            {
                ImGui::End();

                return;
            }

			// Calculate column count.
			const float panelWidth = ImGui::GetContentRegionAvail().x;

			int columnCount = static_cast<int>(panelWidth / CEIL_SIZE.x);
			{
				if (columnCount < 1)
				{
					columnCount = 1;
				}
			}

			// Display current directory name.
			{
				const std::string currentDirectory = _currentDirectory.GetAsString();

				ImGui::Text(currentDirectory.c_str());
			    ImGui::NewLine();
			}

			//{
			//	// Display reload button.
			//	{
			//		const float x = ImGui::GetContentRegionAvail().x;
			//		ImGui::SetCursorPosX(x - LOCAL_RELOAD_IMAGE_SIZE.x * 3.0f);

			//		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));

			//		/*if (ImGui::ImageButton(reloadImage, LOCAL_RELOAD_IMAGE_SIZE))
			//		{
			//			Reload();
			//		}*/

			//		ImGui::PopStyleColor();
			//	}

			//	//ImGui::SameLine();

			//	// Display reload all button.
			//	{
			//		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));

			//		/*if (ImGui::ImageButton(reloadImage, LOCAL_RELOAD_IMAGE_SIZE))
			//		{
			//			ReloadAll();
			//		}*/

			//		ImGui::PopStyleColor();
			//	}
			//}
			
			// Display back arrow.
			{
				// Current directory is not entry path.
				if (_currentDirectory != FilePath(ENTRY_FILE_PATH))
				{
					// Display back arrow button.
					if (ImGui::ImageButton(backArrowImage, LOCAL_BACK_ARROW_IMAGE_SIZE))
					{
						// Back to previous directory.
						if (_currentDirectory != ENTRY_FILE_PATH)
						{
							_currentDirectory = _currentDirectory.ParentPath();
						}
					}
				}
			}

			ImGui::Columns(columnCount, nullptr, false);

			// Get current directory path.
			const std::string currentDirectoryPath = _currentDirectory.GetAsString();

			// Explore the current directory.
			for (auto& content : std::filesystem::directory_iterator(_currentDirectory.Get()))
			{
				const auto& entryPath = content.path();
				String entry = entryPath.filename().string();

				ImGui::PushID(entry.CStr());

				const bool isDirectory = content.is_directory();

				// Select an image that fits the state
				{
					if (isDirectory)
					{
						bool isEmpty = true;
						for ([[maybe_unused]] auto&& contents : std::filesystem::directory_iterator(content))
						{
							// Once you get here, the file exists

							isEmpty = false;
							break;
						}

						if (isEmpty)
						{
							ImGui::ImageButton(emptyDirectoryImage, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y));
						}
						else
						{
							ImGui::ImageButton(fileExistsDirectoryImage, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y));
						}

						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
						{
							_currentDirectory /= String::ToPath(entry);
						}
					}
				}

				bool isDisplay = isDirectory;

				// File type.
				const auto existsFileType = ObtainExistsFileType(entry, isDisplay);
				
				// Call the function that was in the 
				{
					// Animation clip.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::AnimationClip))
					{
						if (IsAnimationClip(entry))
						{
						    AnimationAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Animator controller.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::AnimatorController))
					{
						if (IsAnimatorController(entry))
						{
							AnimatorControllerAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Material.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::Material))
					{
						if (IsMaterial(entry))
						{
						    MaterialAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Scene.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::Scene))
					{
						if (IsScene(entry))
						{
						    SceneAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Prefab.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::Prefab))
					{
						if (IsPrefab(entry))
						{
					    	PrefabAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Shader.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::Shader))
					{
						if (IsShader(entry))
						{
					    	ShaderAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Mesh.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::Mesh))
					{
						if (IsMesh(entry))
						{
					    	MeshAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Collision mesh.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::CollisionMesh))
					{
						if (IsCollisionMesh(entry))
						{
							CollisionMeshAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Approximation collision mesh.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::ApproximationCollisionMesh))
					{
						if (IsApproximationCollisionMesh(entry))
						{
							ApproximationCollisionMeshAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Mesh scene tree.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::MeshSceneTree))
					{
						if (IsMeshSceneTree(entry))
						{
							MeshSceneTreeAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Avatar.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::Avatar))
					{
						if (IsAvatar(entry))
						{
							AvatarAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Texture.
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::Texture))
					{
						if (IsTexture(entry))
						{
					    	TextureAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Skybox
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::Skybox))
					{
						if (IsSkybox(entry))
						{
							SkyboxAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}

					// Script
					if (static_cast<bool>(existsFileType & EditorContentBrowserExistsFileType::Script))
					{
						if (IsScript(entry))
						{
							ScriptAsset(entryPath.string(), entry, isDirectory, isDisplay);
						}
					}
				}

				if (isDisplay)
				{
					String text = {};
					if (isDirectory)
					{
						text = entry;
					}
					else
					{
						bool hasOriginalFormat = false;
						[[maybe_unused]] auto type = Utils::ObtainAssetTypeFromFilePath(FilePath(entryPath), hasOriginalFormat);

						text = ObtainDisplayText(entry, hasOriginalFormat);
					}

					ImGui::TextWrapped(text.CStr());
					ImGui::NextColumn();
				}

				ImGui::PopID();
			}

			// Drag and drop area
			{
				const ImVec2 size = ImGui::GetContentRegionAvail();
				ImGui::Button("##DragAndDropArea", size);
			}

			{
				UpdatePrefabAssets();
			}

			// Scene change.
			if (_isSceneClicked)
			{
				// confirmation.
				SceneChangeModalWindow(_currentDirectory.GetAsString());
			}

			CreateAssetOnContentBrowser();
			ModifyAssetOnContentBrowser();

			ImGui::End();
		}

		// Check is directory changed.
		if (_previousDirectory != _currentDirectory)
		{
			OnDirectoryChanged();
		}

		_previousDirectory = _currentDirectory;

	}

    std::optional<std::string> EditorContentBrowser::MakeUniqueNameInDirectory(const FilePath& directory, std::string_view name, Asset::AssetType assetType, const uint32_t maxTrialCount)
    {
		for (uint32_t trialCount = 0u; trialCount < maxTrialCount; ++trialCount)
		{
			if (trialCount == 0u)
			{
				const String uniqueName = String(name);

				if (!IsNameExists(directory, uniqueName.Get(), assetType))
				{
					return uniqueName.Get();
				}
			}
			else
			{
				const String uniqueName = String(name) + "_" + std::to_string(trialCount);
				if (!IsNameExists(directory, uniqueName.Get(), assetType))
				{
					return uniqueName.Get();
				}
			}
		}

		return std::nullopt;
    }

    bool EditorContentBrowser::IsNameExists(const FilePath& directory, std::string_view name, Asset::AssetType assetType)
    {
		for (auto& content : std::filesystem::directory_iterator(directory.Get()))
		{
			const auto& path = FilePath(content.path());

			bool hasOriginalFormat = false;
			const Asset::AssetType type = Utils::ObtainAssetTypeFromFilePath(path, hasOriginalFormat);

			if (type == assetType)
			{
				if (path.Stem() == name)
				{
			        return true;
				}
			}
		}

		return false;
    }

    //
	// Static functions.
	//
	bool EditorContentBrowser::NameAlreadyExists(std::string_view name, std::string_view directoryFilePath, std::string_view targetExtension)
	{
		const std::string directoryPath = std::string(directoryFilePath) + "\\";

		// target extension file count.
		int targetExtensionFileCount = 0;

		std::vector<std::string> targetExtensionFileNames = { };

		// Find target extension count and file names.
		for (auto&& file : std::filesystem::directory_iterator(directoryPath))
		{
			auto path = file.path();
			auto extension = path.extension().string();
			auto fileName = path.filename().stem().string();

			if (not StringUtils::StringContains(extension, targetExtension)) continue;

			targetExtensionFileNames.emplace_back(fileName);

			++targetExtensionFileCount;
		}

		// Check is same file name exists.
		for (int index = 0; index < targetExtensionFileCount; ++index)
		{
			const std::string target = targetExtensionFileNames.at(index);

			if (target == name)
			{
				return true;
			}
		}

		// Not duplicated.
		return false;
	}

	std::optional<std::string> EditorContentBrowser::MakeUniqueName(std::string_view name, std::string_view directoryFilePath,
		std::string_view targetExtension, const int numberOfTrials)
	{
		for (int trialCount = 0; trialCount < numberOfTrials; ++trialCount)
		{
			const bool alreadyExists = NameAlreadyExists(std::string(name) + std::to_string(trialCount), directoryFilePath, targetExtension);

			// Not Duplicated
			if (not alreadyExists)
			{
				// Make unique name and return.
				const std::string uniqueName = std::string(name) + std::to_string(trialCount);

				return uniqueName;
			}
		}

		return std::nullopt;
	}

	//
	// Member functions (private)
	// 

	void EditorContentBrowser::SceneAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

		const auto scene = sceneLibrary->Find(filePath);
		if (!scene.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Scene " << filePath << " not found!";

			return;
		}

		{
			if (ImGui::ImageButton(fileName.CStr(), Scene::GetAssetTexture()->GetResourceAsComPtr().Get(), ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
			{
				_clickedSceneName = fileName.Get();
				_isSceneClicked = true;
			}
		}

		// Source drag and drop.
		{
			auto id = scene.value()->GetSceneID();
			const DragAndDropSpecification specification =
			{
				"SceneItem", sizeof(SceneID),
				& id, DragAndDropDataType::Scene,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(SceneAssetDragAndDropCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropSource dragAndDropSource(specification);
		}
	}

	void EditorContentBrowser::MaterialAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

	    const MaterialLibrary* materialLibrary = LibraryManager::GetInstance().GetMaterialLibrary();

		const auto material = materialLibrary->Find(filePath);

		if (!material.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Material " << filePath << " not found!";

			return;
		}

		{
			const ImTextureID texture = (material.value()->HasPreviewImage() ? 
				                         material.value()->GetPreviewImage()->GetResourceAsComPtr().Get() :
										 Material::GetDefaultThumbNail()->GetResourceAsComPtr().Get()
			);

			String displayFileName = fileName;
			displayFileName.Append("##Material");

			if (ImGui::ImageButton(displayFileName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
			{
				// Notify event.
				EventOnMaterialEditorOpened eventOnMaterialEditorOpened(material.value());
				NOTIFY_EVENT(eventOnMaterialEditorOpened);
			}
		}
	}

	void EditorContentBrowser::CubeMaterialAsset(const FilePath& filePath, const String& fileName, bool isDirectory, bool isDisplay)
	{

	}

	void EditorContentBrowser::MeshAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

		const auto mesh = meshLibrary->Find<Mesh>(filePath);

		if (!mesh.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Mesh " << filePath << " not found!";

			return;
		}

		// Open mesh editor.
		{
		    const ImTextureID texture = (mesh.value()->HasPreviewImage() ? 
		                                 mesh.value()->GetAssetTexture()->GetResourceAsComPtr().Get() : 
		                                 Mesh::GetAssetTexture()->GetResourceAsComPtr().Get());

			if (ImGui::ImageButton(fileName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
		    {
			    EventOnMeshEditorOpened opened(mesh.value());
			    NOTIFY_EVENT(opened);
		    }
		}

	    // Drag and drop source.
		{
			Utils::DragAndDropMeshData dragAndDropMeshData = {};
			dragAndDropMeshData.Set(mesh.value());

			const DragAndDropSpecification specification =
			{
				"MeshItem", sizeof(Utils::DragAndDropMeshData),
				&dragAndDropMeshData, DragAndDropDataType::Mesh,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(MeshAssetDragAndDropCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropSource dragAndDropSource(specification);
		}
	}

	void EditorContentBrowser::CollisionMeshAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

		const auto collisionMesh = meshLibrary->Find<CollisionMesh>(filePath);

		if (!collisionMesh.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Collision mesh " << filePath << " not found!";

			return;
		}

		// Open collision mesh window if clicked.
		{
			const ImTextureID texture = CollisionMesh::GetAssetTexture()->GetResourceAsComPtr().Get();

			if (ImGui::ImageButton(fileName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
			{
				EventOnCollisionMeshWindowOpened eventOnCollisionMeshWindowOpened(collisionMesh.value());
				NOTIFY_EVENT(eventOnCollisionMeshWindowOpened);
			}
		}

		// Drag and drop source.
		{
			Utils::DragAndDropCollisionMeshData dragAndDropCollisionMeshData = {};
			dragAndDropCollisionMeshData.Set(collisionMesh.value());

			const DragAndDropSpecification specification =
			{
				"CollisionMeshItem", sizeof(Utils::DragAndDropCollisionMeshData),
				&dragAndDropCollisionMeshData, DragAndDropDataType::CollisionMesh,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(CollisionMeshAssetDragAndDropCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropSource dragAndDropSource(specification);
		}
	}

	void EditorContentBrowser::ApproximationCollisionMeshAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

		const auto approximationCollisionMesh = meshLibrary->Find<ApproximationCollisionMesh>(filePath);

		if (!approximationCollisionMesh.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Approximation collision mesh " << filePath << " not found!";

			return;
		}

		// Open collision mesh window if clicked.
		{
			const ImTextureID texture = ApproximationCollisionMesh::GetAssetTexture()->GetResourceAsComPtr().Get();

			if (ImGui::ImageButton(fileName.CStr(), texture, THUMBNAIL_SIZE.ToImVec2()))
			{
				// Nothing to do...
			}
		}

		// Drag and drop source.
		{
			Utils::DragAndDropApproximationCollisionMeshData dragAndDropApproximationCollisionMeshData = {};
			dragAndDropApproximationCollisionMeshData.Set(approximationCollisionMesh.value());

			const DragAndDropSpecification specification =
			{
				"ApproximationCollisionMeshItem", sizeof(Utils::DragAndDropApproximationCollisionMeshData),
				&dragAndDropApproximationCollisionMeshData, DragAndDropDataType::ApproximationCollisionMesh,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(ApproximationCollisionMeshAssetDragAndDropCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropSource dragAndDropSource(specification);
		}
	}

	void EditorContentBrowser::MeshSceneTreeAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const SceneTreeLibrary* sceneTreeLibrary = LibraryManager::GetInstance().GetSceneTreeLibrary();
		const auto sceneTree = sceneTreeLibrary->Find(filePath);

		if (!sceneTree.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Scene tree " << filePath << " not found!";

			return;
		}

		// Open scene tree viewer.
		{
			const ImTextureID texture = (sceneTree.value()->HasPreviewImage() ?
				sceneTree.value()->GetAssetTexture()->GetResourceAsComPtr().Get() :
				MeshResource::SceneTree::GetAssetTexture()->GetResourceAsComPtr().Get());

			if (ImGui::ImageButton(fileName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
			{
				
			}
		}
	}

	void EditorContentBrowser::AvatarAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const SkeletonLibrary* skeletonLibrary = LibraryManager::GetInstance().GetSkeletonLibrary();
		const auto avatar = skeletonLibrary->Find(filePath);

		if (!avatar.has_value())
		{
			DENTY_SET_ERR_COLOR
		    DENTY_ERR_LOG << "Avatar " << filePath << " not found!";

			return;
		}

		// Open avatar viewer.
		{
			const ImTextureID texture = (avatar.value()->HasPreviewImage() ?
				avatar.value()->GetAssetTexture()->GetResourceAsComPtr().Get() :
				MeshResource::Skeleton::GetAssetTexture()->GetResourceAsComPtr().Get());

			if (ImGui::ImageButton(fileName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
			{

			}
		}
	}

	void EditorContentBrowser::AnimationAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const AnimationClipLibrary* clipLibrary = LibraryManager::GetInstance().GetAnimationClipLibrary();

		const auto clip = clipLibrary->Find(filePath);

		if (!clip.has_value())
		{
			DENTY_SET_ERR_COLOR

			DENTY_ERR_LOG << "Animation clip " << filePath << " not found!";

			return;
		}

		{
			const ImTextureID texture = (clip.value()->HasPreviewImage() ?
				                         clip.value()->GetPreviewImage()->GetResourceAsComPtr().Get() : 
				                         MeshResource::AnimationClip::GetAssetTexture()->GetResourceAsComPtr().Get());

		    if (ImGui::ImageButton(fileName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
		    {

		    }
		}

		// Drag and drop source.
		{
			Utils::DragAndDropAnimationClipData clipData = { };
			clipData.Set(clip.value());

			const DragAndDropSpecification specification =
			{
				"AnimationItem", sizeof(Utils::DragAndDropAnimationClipData),
				&clipData, DragAndDropDataType::AnimationClip,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(AnimationClipAssetDragAndDropCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropSource dragAndDropSource(specification);
		}
	}

	void EditorContentBrowser::AnimatorControllerAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const AnimatorControllerLibrary* animatorControllerLibrary = LibraryManager::GetInstance().GetAnimatorControllerLibrary();

		const auto controller = animatorControllerLibrary->Find(filePath);

		if (!controller.has_value())
		{
			DENTY_SET_ERR_COLOR

			DENTY_ERR_LOG << "Animation controller " << filePath << " not found!";

			return;
		}

		{
			const ImTextureID texture = (controller.value()->HasPreviewImage() ?
				                         controller.value()->GetPreviewImage()->GetResourceAsComPtr().Get() : 
				                         AnimatorController::GetAssetTexture()->GetResourceAsComPtr().Get());

		    if (ImGui::ImageButton(fileName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
		    {
				EventOnAnimatorWindowOpened eventOnAnimatorWindowOpened(controller.value());
				NOTIFY_EVENT(eventOnAnimatorWindowOpened);
		    }
		}
	}

	void EditorContentBrowser::TextureAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		const auto texture = textureLibrary->Find<Texture>(filePath);

		if (!texture.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Texture " << filePath << " not found!";

			return;
		}

		if (texture.value()->GetType() != Texture::Type::CubeMap)
		{
			if (ImGui::ImageButton(fileName.CStr(), texture.value()->GetResourceAsComPtr().Get(), ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
			{
				EventOnTextureEditorOpened eventOnTextureEditorOpened(texture.value());
				NOTIFY_EVENT(eventOnTextureEditorOpened);
			}
		}
		else
		{
			String displayName = texture.value()->GetName();
			displayName.Append("##CubeMap");

			if (ImGui::Button(displayName.CStr(), THUMBNAIL_SIZE.ToImVec2()))
			{
				
			}
		}

		// Drag and drop source.
		{
			TextureID id = texture.value()->GetTextureID();
			const DragAndDropSpecification specification =
			{
				"TextureItem", sizeof(TextureID),
				&id, DragAndDropDataType::Texture,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(TextureAssetDragAndDropCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()

			};
			DragAndDropSource dragAndDropSource(specification);
		}
	}

	void EditorContentBrowser::PrefabAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const PrefabLibrary* prefabLibrary = LibraryManager::GetInstance().GetPrefabLibrary();
		const auto prefab = prefabLibrary->Find(filePath);

		if (!prefab.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Prefab " << filePath << " not found!";

			return;
		}

        {
			const ImTextureID texture = (prefab.value()->HasPreviewImage() ? 
				prefab.value()->GetPreviewImage()->GetResourceAsComPtr().Get() : 
				Prefab::GetAssetTexture()->GetResourceAsComPtr().Get());

            if (ImGui::ImageButton(fileName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
            {
                EventOnPrefabEditorOpened eventOnPrefabEditorOpened(prefab.value());
                NOTIFY_EVENT(eventOnPrefabEditorOpened);
            }
        }

		// Drag and drop source.
		{
			PrefabID id = prefab.value()->GetPrefabID();
			const DragAndDropSpecification specification =
			{
				"PrefabItem", sizeof(PrefabID),
				&id, DragAndDropDataType::Prefab,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(PrefabAssetDragAndDropCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()

			};
			DragAndDropSource dragAndDropSource(specification);
		}
	}

	void EditorContentBrowser::ShaderAsset(const FilePath& filePath, const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();
		const auto shader = shaderLibrary->Find(filePath);

		if (!shader.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Shader " << filePath << " not found!";

			return;
		}

		{
			const ImTextureID texture = (shader.value()->HasPreviewImage() ?
				shader.value()->GetPreviewImage()->GetResourceAsComPtr().Get() :
				Shader::GetAssetTexture()->GetResourceAsComPtr().Get());

            if (ImGui::ImageButton(fileName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
            {
                // Notify event.
                EventOnShaderEditorOpened eventOnShaderEditorOpened(shader.value());
                NOTIFY_EVENT(eventOnShaderEditorOpened);
            }
        }
	}

	void EditorContentBrowser::SkyboxAsset(const FilePath& filePath, [[maybe_unused]] const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const SkyboxLibrary* skyboxLibrary = LibraryManager::GetInstance().GetSkyboxLibrary();
		const auto skybox = skyboxLibrary->Find(filePath);

		if (!skybox.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Skybox " << filePath << " not found!";

			return;
		}

		{
			const ImTextureID texture = (skybox.value()->HasPreviewImage() ?
				skybox.value()->GetPreviewImage()->GetResourceAsComPtr().Get() :
				Skybox::GetAssetTexture()->GetResourceAsComPtr().Get());

			String displayName = skybox.value()->GetName();
			displayName.Append("##Skybox");

			if (ImGui::ImageButton(displayName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
			{
			}
		}

		// Drag and drop
		{
			Utils::DragAndDropSkyboxData dragAndDropSkyboxData = {};
			dragAndDropSkyboxData.Set(skybox.value());

			const DragAndDropSpecification specification =
			{
				"SkyboxItem", sizeof(Utils::DragAndDropSkyboxData),
				& dragAndDropSkyboxData, DragAndDropDataType::Skybox,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(SkyboxAssetDragAndDropCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()

			};
			DragAndDropSource dragAndDropSource(specification);
		}
	}

	void EditorContentBrowser::ScriptAsset(const FilePath& filePath, [[maybe_unused]] const String& fileName, bool isDirectory, [[maybe_unused]] bool isDisplay)
	{
		if (isDirectory) return;

		const ScriptLibrary* scriptLibrary = LibraryManager::GetInstance().GetScriptLibrary();
		const auto script = scriptLibrary->Find(filePath);

		if (!script.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Script " << filePath << " not found!";

			return;
		}

		{
			const ImTextureID texture = (script.value()->HasPreviewImage() ?
				script.value()->GetPreviewImage()->GetResourceAsComPtr().Get() :
				Script::GetAssetTexture()->GetResourceAsComPtr().Get());

			String displayName = script.value()->GetName();
			displayName.Append("##Script");

			if (ImGui::ImageButton(displayName.CStr(), texture, ImVec2(THUMBNAIL_SIZE.x, THUMBNAIL_SIZE.y)))
			{
			}
		}

		// Drag and drop
		{
			Utils::DragAndDropScriptData dragAndDropScriptData = {};
			dragAndDropScriptData.Set(script.value());

			const DragAndDropSpecification specification =
			{
				"ScriptItem", sizeof(Utils::DragAndDropScriptData),
				&dragAndDropScriptData, DragAndDropDataType::Script,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(ScriptAssetDragAndDropCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()

			};
			DragAndDropSource dragAndDropSource(specification);
		}
	}

	void EditorContentBrowser::UpdateSceneAssets()
	{
	}

	void EditorContentBrowser::UpdateMaterialAssets()
	{
	}

	void EditorContentBrowser::UpdateMeshAssets()
	{
	}

	void EditorContentBrowser::UpdateAnimationAssets()
	{
	}

	void EditorContentBrowser::UpdateTextureAssets()
	{
	}

	void EditorContentBrowser::UpdatePrefabAssets()
	{
		// Receive "GameObjectList" drag and drop source
		{
			const DragAndDropSpecification specification =
			{
				"GameObjectList", sizeof(EntityID),
				nullptr, DragAndDropDataType::GameObject,
				DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(PrefabAssetDragAndDropTargetCallback),
				DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
			};
			DragAndDropTarget dragAndDropTarget(specification);
		}
	}


	void EditorContentBrowser::UpdateShaderAssets()
	{
	}

	void EditorContentBrowser::CreateAssetOnContentBrowser()
	{
		FilePath directory = _currentDirectory;
		directory += std::string("\\");

		if (ImGui::BeginPopupContextWindow("##EditorContentBrowser"))
		{
			if (ImGui::BeginMenu("Graphics##EditorContentBrowser"))
			{
				const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();
				MaterialLibrary* materialLibrary = LibraryManager::GetInstance().GetMaterialLibrary();

				if (ImGui::MenuItem("StaticMeshMaterial##Graphics"))
				{
					const auto staticMeshShader = shaderLibrary->Find(ShaderLibrary::STATIC_MESH_SHADER_PATH);
					if (staticMeshShader.has_value())
					{
					    const auto material = RuntimeAssetCreator::CreateMaterial(directory, staticMeshShader.value());
						if (material.has_value())
						{
							material.value()->Save();
						    materialLibrary->Add(material.value());
						}
					}
				}

				if (ImGui::MenuItem("SkinnedMeshMaterial##Graphics"))
				{
					const auto skinnedMeshShader = shaderLibrary->Find(ShaderLibrary::SKINNED_MESH_SHADER_PATH);
					if (skinnedMeshShader.has_value())
					{
					    const auto material = RuntimeAssetCreator::CreateMaterial(directory, skinnedMeshShader.value());
						if (material.has_value())
						{
							material.value()->Save();
						    materialLibrary->Add(material.value());
						}
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Animations##EditorContentBrowser"))
			{
				AnimatorControllerLibrary* animatorControllerLibrary = LibraryManager::GetInstance().GetAnimatorControllerLibrary();

				if (ImGui::MenuItem("AnimatorController##Animations"))
				{
					const auto controller = RuntimeAssetCreator::CreateAnimatorController(directory);
					if (controller.has_value())
					{
					    controller.value()->Save();
						animatorControllerLibrary->Add(controller.value());
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Scene##EditorContentBrowser"))
			{
				
			}

			if (ImGui::MenuItem("Script##EditorContentBrowser"))
			{
				RuntimeAssetCreator::CreateScript(directory);
			}

			ImGui::EndPopup();
		}

		if (RuntimeAssetCreator::IsInputFileName())
		{
			RuntimeAssetCreator::OnInputFileName();
		}
	}

    void EditorContentBrowser::ModifyAssetOnContentBrowser()
    {

    }

    void EditorContentBrowser::SceneChangeModalWindow(std::string_view filePath)
	{
		ImGui::OpenPopup("SceneChange");

		ImVec2 centerPos = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(centerPos, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("SceneChange", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure?");
			ImGui::Text("(Unsaved changes are discarded.)");

			// Notify scene change event.
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				// Make file path.
				const std::string path = DENTY_FILE_PATH(std::string(filePath) + "\\" + _clickedSceneName);

				// Create event.
				EventRequestedSceneLoad eventRequestedSceneLoad(path);
				NOTIFY_EVENT(eventRequestedSceneLoad);

				_isSceneClicked = false;

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				_isSceneClicked = false;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void EditorContentBrowser::SceneAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::Scene, "Data type must be scene!");

		const auto id = (*static_cast<SceneID*>(data));

		const SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

		const auto scene = sceneLibrary->Find(id);
		if (scene.has_value())
		{
			ImGui::SetDragDropPayload("SceneItem", &id, sizeof(SceneID));
			ImGui::Text("%s", scene.value()->GetName().c_str());
		}
	}

	void EditorContentBrowser::MeshAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::Mesh, "Data type must be mesh!");

		const auto dragAndDropMeshData = (static_cast<Utils::DragAndDropMeshData*>(data));
		const auto mesh = dragAndDropMeshData->Get();

		if (mesh)
		{
			Utils::DragAndDropMeshData dragAndDropMesh = { };
			dragAndDropMesh.Set(mesh);

			ImGui::SetDragDropPayload("MeshItem", &dragAndDropMesh, sizeof(Utils::DragAndDropMeshData));
			ImGui::Text("%s", mesh->GetName().c_str());
		}
	}

	void EditorContentBrowser::CollisionMeshAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::CollisionMesh, "Data type must be collision mesh!");

		const auto dragAndDropCollisionMeshData = (static_cast<Utils::DragAndDropCollisionMeshData*>(data));
		const auto collisionMesh = dragAndDropCollisionMeshData->Get();

		if (collisionMesh)
		{
			Utils::DragAndDropCollisionMeshData collisionMeshData = { };
			collisionMeshData.Set(collisionMesh);

			ImGui::SetDragDropPayload("CollisionMeshItem", &collisionMeshData, sizeof(Utils::DragAndDropCollisionMeshData));
			ImGui::Text("%s", collisionMesh->GetName().c_str());
		}
	}

	void EditorContentBrowser::ApproximationCollisionMeshAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::ApproximationCollisionMesh, "Data type must be approximation collision mesh!");

		const auto dragAndDropApproximationCollisionMeshData = (static_cast<Utils::DragAndDropApproximationCollisionMeshData*>(data));
		const auto approximationCollisionMesh = dragAndDropApproximationCollisionMeshData->Get();

		if (approximationCollisionMesh)
		{
			Utils::DragAndDropApproximationCollisionMeshData approximationCollisionMeshData = { };
			approximationCollisionMeshData.Set(approximationCollisionMesh);

			ImGui::SetDragDropPayload("ApproximationCollisionMeshItem", &approximationCollisionMeshData, sizeof(Utils::DragAndDropApproximationCollisionMeshData));
			ImGui::Text("%s", approximationCollisionMesh->GetName().c_str());
		}
	}

	void EditorContentBrowser::MaterialAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::Material, "Data type must be material!");

		const MaterialLibrary* materialLibrary = LibraryManager::GetInstance().GetMaterialLibrary();
		const auto materialFilePath = (*static_cast<FilePath*>(data));

		const auto material = materialLibrary->Find(materialFilePath);

		if (material.has_value())
		{
			ImGui::SetDragDropPayload("MaterialItem", &materialFilePath, sizeof(FilePath));
			ImGui::Text("%s", material.value()->GetName().c_str());
		}
	}

	void EditorContentBrowser::AnimationClipAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::AnimationClip, "Data type must be animation clip!");

		const auto clipData = static_cast<Utils::DragAndDropAnimationClipData*>(data);

		if (clipData->Get())
		{
			ImGui::SetDragDropPayload("AnimationClip", clipData, sizeof(Utils::DragAndDropAnimationClipData));
			ImGui::Text("%s", clipData->Get()->GetName().c_str());
		}
	}

	void EditorContentBrowser::AnimationControllerAssetDragAndDropCallback([[maybe_unused]] void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		DENTY_ASSERT(dataType == DragAndDropDataType::AnimatorController, "Data type must be animator controller!");
	}

	void EditorContentBrowser::TextureAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_CHECK_CONDITION(dataType == DragAndDropDataType::Texture)

		const auto id = (*static_cast<TextureID*>(data));

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const std::optional<Ref<Texture2D>> texture2d = textureLibrary->Find<Texture2D>(id);
		if (texture2d.has_value())
		{
			ImGui::SetDragDropPayload("TextureItem", &id, sizeof(TextureID));
			ImGui::Text("%s", texture2d.value()->GetName().c_str());
		}
		else
		{
			
		}
	}

	void EditorContentBrowser::PrefabAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::Prefab, "Data type must be prefab!");

		const PrefabLibrary* prefabLibrary = LibraryManager::GetInstance().GetPrefabLibrary();
		const auto id = (*static_cast<PrefabID*>(data));

		const std::optional<Ref<Prefab>> prefab = prefabLibrary->Find(id);

		if (prefab.has_value())
		{
			ImGui::SetDragDropPayload("PrefabItem", &id, sizeof(PrefabID));
			ImGui::Text("%s", prefab.value()->GetName().c_str());
		}
	}

	void EditorContentBrowser::SkyboxAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::Skybox, "Data type must be skybox!");

		const auto skyboxDragAndDropData = (*static_cast<Utils::DragAndDropSkyboxData*>(data));

		const auto skybox = skyboxDragAndDropData.Get();

		if (skybox)
		{
			Utils::DragAndDropSkyboxData skyboxData = { };
			skyboxData.Set(skybox);

			ImGui::SetDragDropPayload("SkyboxItem", &skyboxData, sizeof(Utils::DragAndDropSkyboxData));
			ImGui::Text("%s", skybox->GetName().c_str());
		}
	}

    void EditorContentBrowser::ScriptAssetDragAndDropCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
    {
		DENTY_ASSERT(dataType == DragAndDropDataType::Script, "Data type must be script!");

		const auto scriptDragAndDropData = (*static_cast<Utils::DragAndDropScriptData*>(data));

	    const Ref<Script> script = scriptDragAndDropData.Get();
		if (script)
		{
			Utils::DragAndDropScriptData scriptData = { };
			scriptData.Set(script);

			ImGui::SetDragDropPayload("ScriptItem", &scriptData, sizeof(Utils::DragAndDropScriptData));
			ImGui::Text("%s", script->GetName().c_str());
		}
    }

    void EditorContentBrowser::PrefabAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::GameObject, "Data type must be game object.");

		const auto activeScene = EditorSceneManager::GetActiveScene();
		const auto sourceEntityId = (*static_cast<const EntityID*>(data));

		const auto gameObject = activeScene->FindByEntityID(sourceEntityId);

		if (not gameObject.has_value())
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << sourceEntityId << " entity id is invalid!";

			return;
		}

		// Not supported currently yet.
		if (gameObject.value()->HasChild())
		{
			DENTY_ERR_CONSOLE_LOG("Parent gameobject not supported currently yet!");

			return;
		}

		FilePath filePath = {};

		// Create file path.
		{
			const std::string filePathString = std::string("Assets\\Prefabs\\")
				+ std::string(gameObject.value()->GetName())
				+ PREFAB_FILE_EXTENSION;

			filePath = FilePath(filePathString);
		}

		// Create prefab.
		{
			PrefabLibrary* prefabLibrary = LibraryManager::GetInstance().GetPrefabLibrary();

			const auto prefab = std::make_shared<Prefab>();
			prefab->Create(gameObject.value(), filePath);

			// Tell them that it has been prefabricated.
			gameObject.value()->OnPrefabricated(prefab->GetFilePathToSerialized());

			prefabLibrary->Add(prefab);
		}
	}

    void EditorContentBrowser::Reload()
	{
		[[maybe_unused]] auto& libraryManager = LibraryManager::GetInstance();

		for (auto entry : std::filesystem::directory_iterator(_currentDirectory.Get()))
		{
			if (entry.is_directory())
				continue;

			const auto& path = entry.path();
			const auto extension = path.extension().string();

			if (extension == ".jpeg")
				continue;

			if (extension == ".Json")
				continue;

			/*if (extension == SCENE_FILE_EXTENSION)
			{
				EditorSceneManager::GetInstance().GetActiveScene()->OnReload();
			}
			else if (extension == MESH_FILE_EXTENSION)
			{
				const MeshLibrary* meshLibrary = libraryManager.GetMeshLibrary();

				const auto mesh = meshLibrary->Find(path.string());
				mesh->OnReload();
			}
			else if (extension == MATERIAL_FILE_EXTENSION)
			{
				const MaterialLibrary* materialLibrary = libraryManager.GetMaterialLibrary();

				const auto material = materialLibrary->Find(path.string());
				material->OnReload();
			}
			else if (extension == PREFAB_FILE_EXTENSION)
			{
				const PrefabLibrary* prefabLibrary = libraryManager.GetPrefabLibrary();

				const auto prefab = prefabLibrary->Find(path.string());
				prefab->OnReload();
			}
			else if (extension == SHADER_FILE_EXTENSION)
			{
				const ShaderLibrary* shaderLibrary = libraryManager.GetShaderLibrary();

				const auto shader = shaderLibrary->Find(path.string());
				shader->OnReload();
			}
			else if (extension == TEXTURE_FILE_EXTENSION)
			{
			}
			else if (extension == ANIMATION_FILE_EXTENSION)
			{
			}
			else if (extension == ANIMATOR_CONTROLLER_FILE_EXTENSION)
			{
			}*/
		}
	}

	void EditorContentBrowser::ReloadAll()
	{
	}

	String EditorContentBrowser::ObtainDisplayText(const String& entry, bool hasOriginalFormat) const
	{
		auto text = entry;
		text.RemoveExtension();

		if (hasOriginalFormat)
			return text;

		// if exists "_" remove.
		const uint32_t index = text.FindLastOf("_");
		if (index != UINT32_MAX)
		{
			text = entry.ClipUntil(index)->Get();
		}

		return text;
	}

	EditorContentBrowserExistsFileType EditorContentBrowser::ObtainExistsFileType(const String& entry, bool& isDisplay) const
	{
		using FileType = EditorContentBrowserExistsFileType;

		auto fileType = static_cast<FileType>(0u);

		// Animation file extension.
		if (IsAnimationClip(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::AnimationClip;
			isDisplay = true;
		}
		// Animator controller file extension.
		else if (IsAnimatorController(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::AnimatorController;
			isDisplay = true;
		}
		// Texture file extension.
		else if (IsTexture(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::Texture;
			isDisplay = true;
		}
		// Scene file extension
		else if (IsScene(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::Scene;
			isDisplay = true;
		}
		// Mesh file extension
		else if (IsMesh(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::Mesh;
			isDisplay = true;
		}
		// Collision mesh file extension
		else if (IsCollisionMesh(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::CollisionMesh;
			isDisplay = true;
		}
		// Approximation collision mesh file extension.
		else if (IsApproximationCollisionMesh(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::ApproximationCollisionMesh;
			isDisplay = true;
		}
		// Mesh scene tree extension
		else if (IsMeshSceneTree(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::MeshSceneTree;
			isDisplay = true;
		}
		// Avatar extension
		else if (IsAvatar(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::Avatar;
			isDisplay = true;
		}
		// Material file extension
		else if (IsMaterial(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::Material;
			isDisplay = true;
		}
		// Prefab file extension
		else if (IsPrefab(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::Prefab;
			isDisplay = true;
		}
		// Shader file extension
		else if (IsShader(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::Shader;
			isDisplay = true;
		}
		// Skybox file extension
		else if (IsSkybox(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::Skybox;
			isDisplay = true;
		}
		// Script file extension
		else if (IsScript(entry))
		{
			fileType = fileType | EditorContentBrowserExistsFileType::Script;
			isDisplay = true;
		}

		return fileType;
	}

	bool EditorContentBrowser::IsAnimationClip(const String& entry) const
	{
		return entry.Contains(ANIMATION_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsAnimatorController(const String& entry) const
	{
		return entry.Contains(ANIMATOR_CONTROLLER_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsTexture(const String& entry) const
	{
		return entry.Contains(TEXTURE_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsScene(const String& entry) const
	{
		const FilePath path = entry.CStr();

		return (path.Extension() == SCENE_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsMesh(const String& entry) const
	{
		const uint32_t index = entry.FindLastOf("_");

		if (index == UINT32_MAX)
			return false;

		String fileName = entry;
		fileName.RemoveUntil(index);
		fileName.RemoveExtension();

		return (entry.Contains(MESH_FILE_EXTENSION));
	}

	bool EditorContentBrowser::IsCollisionMesh(const String& entry) const
	{
		return (entry.Contains(COLLISION_MESH_FILE_EXTENSION));
	}

	bool EditorContentBrowser::IsApproximationCollisionMesh(const String& entry) const
	{
		return (entry.Contains(APPROXIMATION_COLLISION_MESH_FILE_EXTENSION));
	}

	bool EditorContentBrowser::IsMeshSceneTree(const String& entry) const
	{
		const FilePath path = entry.CStr();
		return (path.Extension() == SCENE_TREE_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsAvatar(const String& entry) const
	{
		return entry.Contains(AVATAR_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsMaterial(const String& entry) const
	{
		return entry.Contains(MATERIAL_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsPrefab(const String& entry) const
	{
		return entry.Contains(PREFAB_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsShader(const String& entry) const
	{
		return entry.Contains(SHADER_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsSkybox(const String& entry) const
	{
		return entry.Contains(SKYBOX_FILE_EXTENSION);
	}

	bool EditorContentBrowser::IsScript(const String& entry) const
	{
		return entry.Contains(SCRIPT_FILE_EXTENSION);
	}

	void EditorContentBrowser::OnDirectoryChanged()
	{
		
	}
};