#pragma once

#include "Scene/IScene.h"

#include "Scene/GameObject.h"

#include "System/Resource/Asset.h"

#include "System/Core/ID/SceneID.h"

#include "System/Core/Setting/SceneSetting.h"

namespace DentyEngine
{
	class EventOnPrefabEditorClosed;
	//
	// Event forward declarations.
	//
	class EventOnCreatedGameObjectInHierarchy;
	class EventOnCreatedGameObject;
	class EventOnPrefabOverrideButtonClick;

	class EventOnApplicationStateChangedToEdit;
	class EventOnApplicationStateChangedToPlay;

	class Skybox;

	class Scene final : public IScene, public Asset, public Object, public std::enable_shared_from_this<Scene>
	{
	public:
		Scene() = default;
		Scene(std::string_view name);
		~Scene() override = default;

		void OnUpdate(float deltaTime) override;
		void OnLateUpdate(float deltaTime) override;
		void OnFixedUpdate() override;
		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Destroy() override;
		void OnDestroy() override;

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Rename(std::string_view newName, bool deleteOldFile) override;
		void OnReload() override;

		void Clone(const Ref<Asset>& asset) override;

		void SaveAs(std::string_view newName);

		void OnGuiInfo();

		void BeforeSave();

		void ReGenerateID() override;

		//
		// Event functions.
		//

		// Call on EditorSceneView::OnSkyboxItemReceived function.
		void OnSkyboxItemReceived(Ref<Skybox> skybox);

	    void OnCreatedGameObject(EventOnCreatedGameObject* e);

		void OnPrefabOverrideButtonClick(EventOnPrefabOverrideButtonClick* e);
		void OnPrefabEditorClosed(EventOnPrefabEditorClosed* e);

		void AddGameObject(const Ref<GameObject>& gameObject);

		void ReserveAddGameObject(const Ref<GameObject>& gameObject) { _reserveAddGameObjects.emplace_back(gameObject); }
		void ReserveRemoveGameObject(const Ref<GameObject>& gameObject) { _reserveRemoveGameObjects.emplace_back(gameObject); }

		[[nodiscard]]
		std::optional<Ref<GameObject>> FindByName(std::string_view name) const;

		[[nodiscard]]
		std::optional<Ref<GameObject>> FindByEntityID(EntityID id) const;

		[[nodiscard]]
		std::optional<Ref<GameObject>> FindByTag(std::string tag) const;

		[[nodiscard]]
		std::list<Ref<GameObject>> FindGameObjectsByTag(std::string_view tag) const;

		[[nodiscard]]
		std::string ObtainProcessedName(const std::string& name) const;

		//
		// Getter
		//
		[[nodiscard]]
		const SceneSetting& GetSceneSetting() const { return _setting; }

		[[nodiscard]]
		SceneSetting& GetSceneSetting() { return _setting; }

		[[nodiscard]]
		SceneID GetSceneID() const { return _sceneID; }

		[[nodiscard]]
		const std::list<Ref<GameObject>>& GetGameObjects() const { return _gameObjects; }

		[[nodiscard]]
		AssetType GetAssetType() const override { return AssetType::Scene; }

		[[nodiscard]]
		bool HasGameObjects() const { return not _gameObjects.empty(); }

		[[nodiscard]]
		bool ExistsCanvas() const;

		[[nodiscard]]
		bool IsDuplicate(EntityID id) const;

		//
		// Static functions.
		//
		static void LoadAssetTexture();

		[[nodiscard]]
		static Ref<Texture2D> GetAssetTexture() { return _sceneAssetTexture; }
	private:
		void CreateDefaultObjects();
		void RemoveGameObject(const Ref<GameObject>& gameObject);

		void RestoreSkybox();

		// return -1 means can not found.
		// return 0 means found same name.
		// return n > 0 means found name which is target string, and n means count of number.
		[[nodiscard]]
		int32_t IsDuplicate(const std::string& targetName) const;

		//
		// Event functions.
		//
		void OnSceneActivated(EventOnSceneActivated* e);

		void OnApplicationStateChangedToEdit(EventOnApplicationStateChangedToEdit* e);
		void OnApplicationStateChangedToPlay(EventOnApplicationStateChangedToPlay* e);
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 5u)
			{
			    archive(
					cereal::base_class<Asset>(this),
					cereal::base_class<Object>(this),
					CEREAL_NVP(_sceneID),
					CEREAL_NVP(_gameObjects),
					CEREAL_NVP(_skyboxFilePath),
					CEREAL_NVP(_setting)
			    );
			}

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
					cereal::base_class<Asset>(this),
					cereal::base_class<Object>(this),
					CEREAL_NVP(_sceneID),
					CEREAL_NVP(_gameObjects),
					CEREAL_NVP(_skyboxFilePath)
			    );
			}
			else if (version == 2u)
			{
			    archive(
				    cereal::base_class<Asset>(this),
					cereal::base_class<Object>(this),
					CEREAL_NVP(_sceneID),
					CEREAL_NVP(_gameObjects),
					CEREAL_NVP(_skyboxFilePath)
				);
			}
			else if (version == 3u)
			{
				archive(
					cereal::base_class<Asset>(this),
					cereal::base_class<Object>(this),
					CEREAL_NVP(_sceneID),
					CEREAL_NVP(_gameObjects),
					CEREAL_NVP(_skyboxFilePath),
					CEREAL_NVP(_setting)
				);
			}
			else if (version == 4u)
			{
				archive(
					cereal::base_class<Asset>(this),
					cereal::base_class<Object>(this),
					CEREAL_NVP(_sceneID),
					CEREAL_NVP(_gameObjects),
					CEREAL_NVP(_skyboxFilePath)
				);
			}
			else if (version == 5u)
			{
				archive(
					cereal::base_class<Asset>(this),
					cereal::base_class<Object>(this),
					CEREAL_NVP(_sceneID),
					CEREAL_NVP(_gameObjects),
					CEREAL_NVP(_skyboxFilePath),
					CEREAL_NVP(_setting)
				);
			}

			OnDeserialized();
		}

	private:
		SceneID _sceneID;

		std::list<Ref<GameObject>> _gameObjects;

		Ref<Skybox> _skybox;
		FilePath _skyboxFilePath;

		SceneSetting _setting;

		std::vector<Ref<GameObject>> _reserveAddGameObjects;
		std::vector<Ref<GameObject>> _reserveRemoveGameObjects;
	private:
		static Ref<Texture2D> _sceneAssetTexture;
	};
}

CEREAL_REGISTER_TYPE(DentyEngine::Scene)

CEREAL_CLASS_VERSION(DentyEngine::Scene, 5u)