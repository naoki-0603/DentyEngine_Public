#pragma once

#include "System/Core/ID/PrefabID.h"
#include "System/Core/ID/EntityID.h"

#include "Asset.h"
#include "System/Core/DString.h"

#include "System/Core/Object.h"

#include "Component/Transform.h"

#include "System/Core/Tag.h"

namespace DentyEngine
{
	class Scene;

	struct PrefabGameObject
	{
	public:
		PrefabGameObject();
		~PrefabGameObject() = default;

	public:
		std::vector<Ref<Component>> components;

		Transform transform;
		std::string name;
		Tag tag;

		bool enabled;
		bool isStatic;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1)
			{
				archive(
					CEREAL_NVP(components),
					CEREAL_NVP(transform),
					CEREAL_NVP(name),
					CEREAL_NVP(tag),
					CEREAL_NVP(enabled),
					CEREAL_NVP(isStatic)
				);
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version) 
		{
			if (version <= 1)
			{
				archive(
					CEREAL_NVP(components),
					CEREAL_NVP(transform),
					CEREAL_NVP(name),
					CEREAL_NVP(tag),
					CEREAL_NVP(enabled),
					CEREAL_NVP(isStatic)
				);
			}
		}
	};

	struct PrefabSpecification
	{
	public:
		PrefabSpecification();
		~PrefabSpecification() = default;

		PrefabGameObject parent;
		std::vector<PrefabGameObject> children;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1)
			{
				archive(
					CEREAL_NVP(parent),
					CEREAL_NVP(children)
				);
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1)
			{
				archive(
					CEREAL_NVP(parent),
					CEREAL_NVP(children)
				);
			}
		}
	};

	class Prefab final : public Asset, public Object, public std::enable_shared_from_this<Prefab>
	{
		friend class PrefabEditor;
	public:
		Prefab();
		Prefab(const Prefab& source);
		~Prefab() override = default;

		// Create prefab.
		void Create(const Ref<GameObject>& source, const FilePath& filePath);

		// Create game object from prefab.
		[[nodiscard]]
		std::vector<Ref<GameObject>> CreateGameObjects(const Scene* activeScene, const Vector3& position = Vector3::Zero());

		void Destroy() override;
		void OnDestroy() override;

		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Rename(std::string_view newName, bool deleteOldFile) override;

		void OnReload() override;

		void Clone(const Ref<Asset>& asset) override;

		//
		// Getter
		//
		[[nodiscard]]
		const PrefabSpecification& GetSpecification() const { return _specification; }

		[[nodiscard]]
		AssetType GetAssetType() const override { return AssetType::Prefab; }

		[[nodiscard]]
		PrefabID GetPrefabID() const { return _id; }
	public:
		//
		// Static functions.
		//
		static void LoadAssetTexture();

		[[nodiscard]]
		static Ref<Texture2D> GetAssetTexture() { return _prefabAssetTexture; }
	public:
		//
		// Static Variables.
		//
		static const String ADD_FILENAME_AT_END;
	private:
		void FetchGameObjectData(const Ref<GameObject>& source, PrefabGameObject& prefabGameObject);
		void FetchPrefabGameObjectData(const PrefabGameObject& source, Ref<GameObject>& gameObject, const Scene* activeScene);

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_specification),
					CEREAL_NVP(_id)
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
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_specification),
					CEREAL_NVP(_id)
				);
			}

			OnDeserialized();
		}

	private:
		PrefabSpecification _specification;

		PrefabID _id;
	private:
		static Ref<Texture2D> _prefabAssetTexture;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::PrefabGameObject, 1u)
CEREAL_CLASS_VERSION(DentyEngine::PrefabSpecification, 1u)
CEREAL_CLASS_VERSION(DentyEngine::Prefab, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Prefab)
