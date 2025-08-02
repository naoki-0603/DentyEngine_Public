#include "stdafx.h"

#include "Scene.h"

#include "Component/Graphics/Renderer/DirectionalLight.h"
#include "Component/Graphics/Renderer/HemisphereLight.h"
#include "Component/Graphics/Renderer/GameCamera.h"

#include "Component/Physics/RigidBody.h"
#include "Component/Physics/Colliders/Collider.h"

#include "Graphics/Skybox.h"

#include "System/Events/EventScene.h"
#include "System/Events/EventSystem.h"
#include "System/Events/EventEditor.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/PrefabLibrary.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "System/Serializer/SceneSerializer.h"

#include "Graphics/Renderer/Texture2D.h"
#include "System/Editor/EditorSceneManager.h"
#include "System/Graphics/Renderer/RenderManager.h"
#include "System/Resource/Library/SceneLibrary.h"
#include "System/Resource/Library/SkyboxLibrary.h"

#include "Utilities/StringUtilities.h"

namespace DentyEngine
{
	Ref<Texture2D> Scene::_sceneAssetTexture = nullptr;

	Scene::Scene(std::string_view name) :
		IScene(), Object(), _sceneID(), _gameObjects(), _skybox(), _skyboxFilePath(), _reserveRemoveGameObjects()
	{
		_name = name;

		CreateDefaultObjects();

		// Load texture from library.
		{
		    if (not _sceneAssetTexture)
		    {
                const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
				_sceneAssetTexture = textureLibrary->Find<Texture2D>(DENTY_FILE_PATH("Assets/Textures/Editor/ContentBrowser/Scene.Texture")).value();
		    }
		}
	}

	void Scene::OnUpdate(float deltaTime)
	{
		// Delete game objects scheduled for deletion.
		for (const auto& gameObject : _reserveRemoveGameObjects)
		{
			RemoveGameObject(gameObject);
		}
		_reserveRemoveGameObjects.clear();

		for (const auto& gameObject : _reserveAddGameObjects)
		{
			AddGameObject(gameObject);
		}
		_reserveAddGameObjects.clear();

		for (const auto& gameObject : _gameObjects)
		{
			gameObject->OnUpdate(deltaTime);
		}
	}

	void Scene::OnLateUpdate(float deltaTime)
	{
		for (auto&& gameObject : _gameObjects)
		{
			gameObject->OnLateUpdate(deltaTime);
		}
	}

	void Scene::OnFixedUpdate()
	{
		for (const auto& gameObject : _gameObjects)
		{
			gameObject->OnFixedUpdate();
		}
	}

	void Scene::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnSceneActivated>(BIND_EVENT_FUNCTION(OnSceneActivated));

		dispatcher.Dispatch<EventOnApplicationStateChangedToEdit>(BIND_EVENT_FUNCTION(OnApplicationStateChangedToEdit));
		dispatcher.Dispatch<EventOnApplicationStateChangedToPlay>(BIND_EVENT_FUNCTION(OnApplicationStateChangedToPlay));

		dispatcher.Dispatch<EventOnCreatedGameObject>(BIND_EVENT_FUNCTION(OnCreatedGameObject));
		dispatcher.Dispatch<EventOnPrefabOverrideButtonClick>(BIND_EVENT_FUNCTION(OnPrefabOverrideButtonClick));

		dispatcher.Dispatch<EventOnPrefabEditorClosed>(BIND_EVENT_FUNCTION(OnPrefabEditorClosed));

		for (const auto& gameObject : _gameObjects)
		{
			gameObject->OnEvent(e);
		}
	}

	void Scene::OnSerialized() const
	{
	}

	void Scene::OnDeserialized()
	{
		// Skybox
		{
			RestoreSkybox();
		}
	}

	void Scene::Destroy()
	{
		Object::Destroy();

		for (const auto& gameObject : _gameObjects)
		{
			gameObject->OnDestroy();
		}

		_gameObjects.clear();
	}

	void Scene::OnDestroy()
	{
		Object::OnDestroy();
	}

	void Scene::Clone(const Ref<Asset>& asset)
	{
		DENTY_ASSERT(asset->GetAssetType() == AssetType::Scene, "Asset type must be scene. (in Scene::Clone)");

		// target
		auto&& target = std::static_pointer_cast<Scene>(asset);

		target->_name = _name;
		target->_uuid = _uuid;

		target->_filePath = _filePath;
		target->_filePathToSerialized = _filePathToSerialized;

		// Set values.
		target->_sceneID = _sceneID;
		target->_skyboxFilePath = _skyboxFilePath;
		target->_skybox = _skybox;
		target->_setting = _setting;

		// Copy
		for (const auto& gameObject : _gameObjects)
		{
			auto copyGameObject = std::make_shared<GameObject>();
			gameObject->Clone(copyGameObject);

			target->_gameObjects.emplace_back(copyGameObject);
		}

		// Set parent.
		for (const auto& gameObject : target->_gameObjects)
		{
			gameObject->transform.SetParent(gameObject);

			for (const auto& component : gameObject->GetComponents())
			{
				component->SetParent(gameObject);
			}
		}

		// Rebuilding child-parent relation ships.
		for (const auto& gameObject : target->_gameObjects)
		{
			// Set child parent.
			for (const auto id : gameObject->GetChildEntityIDs())
			{
				const auto& child = FindByEntityID(id);
				if (!child.has_value())
				{
					DENTY_SET_ERR_COLOR
					DENTY_ERR_LOG << "GameObject " << id << " not found!";

					DENTY_ASSERT(false, "");
				}

				if (gameObject->GetEntityID() == child.value()->GetParentEntityID())
				{
					child.value()->SetParent(gameObject.get());
					gameObject->AddChild(child.value().get());
				}
			}

			gameObject->scene = shared_from_this().get();
		}
	}

	void Scene::Save(bool isUpdate) const
	{
		// If failed
		if (!SceneSerializer::SerializeAsBinary(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " scene!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " scene!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " scene!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " scene!";
		}
	}

	void Scene::Load(const std::filesystem::path& filePath)
	{
		auto scene = std::make_shared<Scene>();

		// If failed
		if (!SceneSerializer::DeserializeFromBinary(filePath, scene))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " scene!";

			return;
		}

		scene->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " scene!";
	}

	void Scene::Rename(std::string_view newName, bool deleteOldFile)
	{
		_name = newName;

		if (deleteOldFile)
		{
			_filePathToSerialized.RemoveFile();
		}

		_filePath.ReplaceFilename(_name.Get());
		_filePathToSerialized.ReplaceFilename(_name.Get());
	}
	
	void Scene::OnReload()
	{
		// nothing to do.
	}

	void Scene::SaveAs(std::string_view newName)
	{
		// Get scene library.
		SceneLibrary* sceneLibrary = LibraryManager::GetInstance().GetSceneLibrary();

	    // Create new scene.
		const auto newScene = std::make_shared<Scene>();

		// Clone scene.
		this->Clone(newScene);

		// Rename new scene.
		newScene->Rename(newName, false);

		// Check is duplicate scene id.
		if (sceneLibrary->IsDuplicateID(newScene->GetSceneID()))
		{
			newScene->ReGenerateID();
		}

		newScene->Save();

		// Add to library
		sceneLibrary->Add(newScene);

		// Reserve change scene.
		EditorSceneManager::SetReservedSceneFilePath(newScene->GetFilePathToSerialized(), false, false, false);
	}

	void Scene::OnGuiInfo()
	{
		ImGui::Text("SceneName: %s", _name.CStr());
		ImGui::Text("SceneID: %u", _sceneID);
		ImGui::Text("FilePath: %s", _filePathToSerialized.GetAsString().c_str());

		ImGui::Separator();

		ImGui::Text("Skybox: %s", _skyboxFilePath.IsEmpty() ? "None" : _skyboxFilePath.GetAsString().c_str());
	}

	void Scene::BeforeSave()
	{
		for (auto&& gameObject : _gameObjects)
		{
			gameObject->OnBeforeSave();
		}

		_setting.OnBeforeSave();
	}

    void Scene::ReGenerateID()
    {
		_sceneID.ReCalculate();
    }

    void Scene::OnSkyboxItemReceived(Ref<Skybox> skybox)
	{
		_skybox = skybox;
		_skyboxFilePath = _skybox->GetFilePathToSerialized();

		RenderManager::GetInstance().OnSceneSkyboxChanged(_skybox);
	}

	void Scene::OnCreatedGameObject(EventOnCreatedGameObject* e)
	{
		AddGameObject(e->GetGameObject());
	}

	void Scene::OnPrefabOverrideButtonClick([[maybe_unused]] EventOnPrefabOverrideButtonClick* e)
	{
	}

	void Scene::OnPrefabEditorClosed(EventOnPrefabEditorClosed* e)
	{
		for (auto&& gameObject : _gameObjects)
		{
			if (not gameObject->Prefab())
				continue;

			if (gameObject->GetPrefabFilePath() != e->filePath)
				continue;

			gameObject->OnPrefabChanged();
		}
	}

	void Scene::AddGameObject(const Ref<GameObject>& gameObject)
	{
		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "GameObject created. (EntityID : " << gameObject->GetEntityID() << ")";

		_gameObjects.emplace_back(gameObject);
	}

	std::optional<Ref<GameObject>> Scene::FindByName(std::string_view name) const
	{
		for (const auto& object : _gameObjects)
		{
			if (object->GetName() != name) continue;

			return object;
		}

		return std::nullopt;
	}

	std::optional<Ref<GameObject>> Scene::FindByEntityID(EntityID id) const
	{
		for (auto&& object : _gameObjects)
		{
			if (object->GetEntityID() != id) continue;

			return object;
		}

		return std::nullopt;
	}

	std::optional<Ref<GameObject>> Scene::FindByTag(std::string tag) const
	{
		for (auto&& object : _gameObjects)
		{
			if (object->tag() != tag)
				continue;

			return object;
		}

		return std::nullopt;
	}

	std::list<Ref<GameObject>> Scene::FindGameObjectsByTag(std::string_view tag) const
	{
		std::list<Ref<GameObject>> gameObjects = { };

		for (auto&& object : _gameObjects)
		{
			if (object->tag() != std::string(tag))
				continue;

			gameObjects.emplace_back(object);
		}

		return gameObjects;
	}

	std::string Scene::ObtainProcessedName(const std::string& name) const
	{
		const int result = IsDuplicate(name);

		// No duplicated.
		if (result == -1) return name;

		const auto processedName = std::string(name + std::to_string(result + 1));
		return processedName;
	}

	bool Scene::ExistsCanvas() const
	{
		const std::optional<Ref<GameObject>> canvas = FindByTag("Canvas");

		return (canvas.has_value());
	}

    bool Scene::IsDuplicate(EntityID id) const
    {
		for (auto&& gameObject : _gameObjects)
		{
		    if (gameObject->GetEntityID() == id)
				return true;

		}

		return false;
    }

    void Scene::LoadAssetTexture()
    {
		DENTY_ASSERT(not _sceneAssetTexture, "Scene asset texture already loaded!");

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		_sceneAssetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::COMMON_SCENE_FILE_PATH).value();

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Scene asset texture successfully loaded!";
    }

    void Scene::CreateDefaultObjects()
	{
		// Reset game camera.
		GameCamera::ResetMainCamera();

		// Create main camera.
		{
		    const auto mainCamera = std::make_shared<GameObject>();
		    mainCamera->Create("MainCamera", "MainCamera", this);
		    mainCamera->tag = Tag::TAGS.at(1);

		    mainCamera->AddComponent<GameCamera>();

		    AddGameObject(mainCamera);
		}
		
		// Create directional light.
		{
		    const auto directionalLight = std::make_shared<GameObject>();
		    directionalLight->Create("DirectionalLight", "UnTagged", this);

			directionalLight->AddComponent<DirectionalLight>();

		    AddGameObject(directionalLight);
		}

		// Create hemisphere light.
		{
			const auto hemisphereLight = std::make_shared<GameObject>();
			hemisphereLight->Create("HemisphereLight", "UnTagged", this);

			hemisphereLight->AddComponent<HemisphereLight>();

			AddGameObject(hemisphereLight);
		}
	}

	void Scene::RemoveGameObject(const Ref<GameObject>& gameObject)
	{
		if (not gameObject) return;

		if (gameObject->GetParentEntityID())
		{
			const auto parent = FindByEntityID(gameObject->GetParentEntityID());
			if (!parent.has_value())
			{
				DENTY_SET_ERR_COLOR
				DENTY_ERR_LOG << "GameObject " << gameObject->GetParentEntityID() << " not found!";

				return;
			}

			parent.value()->RemoveChild(gameObject.get());
		}

		for (const auto childEntityID : gameObject->GetChildEntityIDs())
		{
			const auto child = FindByEntityID(childEntityID);
			if (!child.has_value())
			{
				DENTY_SET_ERR_COLOR
				DENTY_ERR_LOG << "GameObject " << childEntityID << " not found!";

				return;
			}

			child.value()->SetParent(gameObject->GetParent());
		}

		_gameObjects.erase(std::ranges::find(_gameObjects, gameObject));

		gameObject->OnDestroy();
	}

	void Scene::RestoreSkybox()
	{
		if (!_skyboxFilePath.IsEmpty())
		{
			const SkyboxLibrary* skyboxLibrary = LibraryManager::GetInstance().GetSkyboxLibrary();

			const auto skybox = skyboxLibrary->Find(_skyboxFilePath);
			if (!skybox.has_value())
			{
				DENTY_ASSERT(false, "Invalid skybox path! (in Scene::RestoreSkybox)");
			}

			_skybox = skybox.value();
		}
	}

	int32_t Scene::IsDuplicate(const std::string& targetName) const
	{
		int32_t result = -1;

		for (const auto& gameObject : _gameObjects)
		{
			bool isDuplicate = StringUtils::StringContains(gameObject->GetName(), targetName);

			if (isDuplicate)
			{
				++result;
			}
		}

		return result;
	}

	void Scene::OnSceneActivated([[maybe_unused]] EventOnSceneActivated* e)
	{
		for (auto&& gameObject : _gameObjects)
		{
			EntityID parentID = gameObject->GetParentEntityID();
			if (parentID > 0)
			{
				const auto parent = FindByEntityID(parentID);
				if (!parent.has_value())
				{
					DENTY_SET_ERR_COLOR
					DENTY_ERR_LOG << "GameObject " << parentID << " not found!";

					DENTY_ASSERT(false, "");
				}

				gameObject->SetParent(parent.value().get());
			}

			gameObject->ClearChildren();
			if (gameObject->GetChildEntityIDs().size() > 0)
			{
				for (const auto childEntityID : gameObject->GetChildEntityIDs())
				{
					const auto child = FindByEntityID(childEntityID);
					if (!child.has_value())
					{
						DENTY_SET_ERR_COLOR
						DENTY_ERR_LOG << "GameObject " << childEntityID << " not found!";
						DENTY_ASSERT(false, "");
					}

					gameObject->AddChild(child.value().get(), true);
				}
			}

			gameObject->scene = this;
		}

		RestoreSkybox();

		// Set skybox if exists.
		if (_skybox)
		{
			RenderManager::GetInstance().SetSkybox(_skybox);
		}
	}

	void Scene::OnApplicationStateChangedToEdit([[maybe_unused]] EventOnApplicationStateChangedToEdit* e)
	{
		for (auto&& gameObject : _gameObjects)
		{
			if (gameObject->HasComponent<Collider>())
			{
				const auto collider = gameObject->GetComponentAsSharedPtr<Collider>();

				collider->OnApplicationStateChangedToEdit();
			}

			if (gameObject->HasComponent<RigidBody>())
			{
				gameObject->transform.SetUpdateEnabled();
			}
		}
	}

	void Scene::OnApplicationStateChangedToPlay([[maybe_unused]] EventOnApplicationStateChangedToPlay* e)
	{
		for (auto&& gameObject : _gameObjects)
		{
			if (gameObject->HasComponent<Collider>())
			{
				const auto collider = gameObject->GetComponentAsSharedPtr<Collider>();

				collider->OnApplicationStateChangedToPlay();
			}

			if (gameObject->HasComponent<RigidBody>())
			{
				const auto rigidBody = gameObject->GetComponentAsSharedPtr<RigidBody>();

				bool isKinematic = rigidBody->IsKinematic();
				if (!isKinematic)
				{
					gameObject->transform.SetUpdateDisabled();
				}

				rigidBody->OnApplicationStateChangedToPlay();
			}
		}
	}
}
