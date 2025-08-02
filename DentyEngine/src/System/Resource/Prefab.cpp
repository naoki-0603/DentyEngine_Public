#include "stdafx.h"

#include "Prefab.h"

#include "Library/LibraryManager.h"
#include "Library/TextureLibrary.h"

#include "Graphics/Renderer/Texture2D.h"

#include "Scene/GameObject.h"

#include "System/Serializer/PrefabSerializer.h"
#include "System/Serializer/SerializeExtensions.h"

#include "System/Events/EventEditor.h"

#include "Utilities/StringUtilities.h"

namespace DentyEngine
{
	const String Prefab::ADD_FILENAME_AT_END = "_Prefab";

	Ref<Texture2D> Prefab::_prefabAssetTexture = nullptr;

	PrefabGameObject::PrefabGameObject() :
		components(), transform(), name(), tag(), enabled(), isStatic()
	{

	}

	PrefabSpecification::PrefabSpecification() :
		parent(), children()
	{
	}

	Prefab::Prefab() :
		Asset(), Object(), _specification(), _id()
	{
	}

	Prefab::Prefab(const Prefab& source) :
		Asset(source), Object(source), _specification(source._specification), _id(source._id)
	{
	}

	void Prefab::Create(const Ref<GameObject>& source, const FilePath& filePath)
	{
		// Parent
		{
			_name = source->GetName();

			FetchGameObjectData(source, _specification.parent);
		}

		// TODO Children
		{
		}

		// Store file paths.
		{
			_filePath = filePath;
			_filePathToSerialized = _filePath;
		}

		Save();
	}

	std::vector<Ref<GameObject>> Prefab::CreateGameObjects(const Scene* activeScene, const Vector3& position)
	{
		std::vector<Ref<GameObject>> gameObjects = { };

		// Parent
		{
			Ref<GameObject>& parent = gameObjects.emplace_back();
			parent = std::make_shared<GameObject>();

			FetchPrefabGameObjectData(_specification.parent, parent, activeScene);
			parent->transform.localPosition = position;

			parent->SetPrefab(true);
			parent->SetPrefabFilePath(_filePathToSerialized);
		}

		// TODO Children
		{
		}

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "GameObject successes created from prefab. (PrefabID: " << _id << " )";

		return gameObjects;
	}

	void Prefab::Destroy()
	{
		Object::Destroy();
	}

	void Prefab::OnDestroy()
	{
		Object::OnDestroy();
	}

	void Prefab::OnEvent(Event* e)
	{
		Object::OnEvent(e);
	}

	void Prefab::OnSerialized() const
	{
		Object::OnSerialized();
	}

	void Prefab::OnDeserialized()
	{
		Object::OnDeserialized();
	}

	void Prefab::Save(bool isUpdate) const
	{
		// If failed.
		if (!PrefabSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " prefab!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " prefab!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " prefab!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " prefab!";
		}
	}

	void Prefab::Load(const std::filesystem::path& filePath)
	{
		auto prefab = std::make_shared<Prefab>();

		// If failed.
		if (!PrefabSerializer::DeserializeFromJson(filePath, prefab))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " prefab!";

			return;
		}

		prefab->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " prefab!";
	}

	void Prefab::Rename(std::string_view newName, [[maybe_unused]] bool deleteOldFile)
	{
		_name = newName;
	}

	void Prefab::OnReload()
	{
	}

	void Prefab::Clone(const Ref<Asset>& asset)
	{
		DENTY_CHECK_CONDITION(asset->GetAssetType() == AssetType::Prefab)

		Ref<Prefab>&& target = std::static_pointer_cast<Prefab>(asset);

		// Object class
		{
			target->_name = _name;
			target->_uuid = _uuid;
		}

		// Asset class
		{
			target->_previewImage = _previewImage;
			target->_previewImageFilePath = _previewImageFilePath;

			target->_filePath = _filePath;
			target->_filePathToSerialized = _filePathToSerialized;

			target->_referenceObjects = _referenceObjects;
		}

		// This class.
		{
			target->_id = _id;
			target->_specification = _specification;
		}
	}

    void Prefab::LoadAssetTexture()
    {
		DENTY_ASSERT(not _prefabAssetTexture, "Prefab asset texture already loaded!");

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		_prefabAssetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::PREFAB_DEFAULT_THUMBNAIL_FILE_PATH).value();

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Prefab asset texture successfully loaded!";
    }

    void Prefab::FetchGameObjectData(const Ref<GameObject>& source, PrefabGameObject& prefabGameObject)
    {
		prefabGameObject.name = source->GetName();
		prefabGameObject.tag = source->tag;
		prefabGameObject.transform = source->transform;
		prefabGameObject.enabled = source->Enabled();
		prefabGameObject.isStatic = source->Static();

		for (const auto& component : source->GetComponents())
		{
			Ref<Component>& comp = prefabGameObject.components.emplace_back();
			comp = std::static_pointer_cast<Component>(component->MakeCopy());
		}
    }

	void Prefab::FetchPrefabGameObjectData(const PrefabGameObject& source, Ref<GameObject>& gameObject, const Scene* activeScene)
	{
		gameObject->CreatePrefab(source.name, source.tag(), source.transform, activeScene);

		gameObject->SetEnabled(source.enabled);
		gameObject->SetStatic(source.isStatic);

		for (const auto& component : source.components)
		{
			component->SetParent(gameObject);

			const auto comp = std::static_pointer_cast<Component>(component->MakeCopy());

			component->SetParent(nullptr);
			comp->SetParent(nullptr);

			gameObject->SetComponent(comp);
		}
	}
}
