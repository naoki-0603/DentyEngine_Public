#include "stdafx.h"

#include "PrefabLibrary.h"

#include "Scene/GameObject.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "System/Events/EventScene.h"

#include "System/Serializer/PrefabSerializer.h"
#include "System/Serializer/SerializeExtensions.h"

#include "Utilities/StringUtilities.h"

namespace DentyEngine
{
	PrefabLibrary::PrefabLibrary() :
		ILibrary(), _caches()
	{
	}

	void PrefabLibrary::Initialize()
	{
		auto prefabFolderPath = EditorContentBrowser::ENTRY_FILE_PATH;
		auto contentSerializer = [this](auto self, FilePath& folderPath) -> void
		{
			for (auto& content : std::filesystem::directory_iterator(folderPath.Get()))
			{
				const auto& entryPath = content.path();
				std::string entry = entryPath.filename().string();

				if (content.is_directory())
				{
					self(self, folderPath /= entry);
					folderPath = folderPath.ParentPath();
				}

				if (IsPrefabFile(entryPath.string()))
				{
					const auto prefab = std::make_shared<Prefab>();
					prefab->Load(entryPath);

					Add(prefab);
				}
			}
		};
		contentSerializer(contentSerializer, prefabFolderPath);
	}

	void PrefabLibrary::Update()
	{
	}

	void PrefabLibrary::OnGui()
	{
	}

	void PrefabLibrary::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnPrefabOverrideButtonClick>(BIND_EVENT_FUNCTION(OnPrefabOverrideButtonClick));
	}

	void PrefabLibrary::Finalize()
	{
		_caches.clear();
	}

	void PrefabLibrary::OnSerialized() const
	{
	}

	void PrefabLibrary::OnDeserialized()
	{
	}

	void PrefabLibrary::Add(const Ref<Prefab>& prefab)
	{
		const auto& filePath = prefab->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in PrefabLibrary::Add)");
		}

		_caches.try_emplace(filePath, prefab);
	}

	void PrefabLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR

			DENTY_INFO_LOG << "Succeed to remove " << filePath << " prefab from library!";

			_caches.erase(it);

			return;

		}

		DENTY_SET_ERR_COLOR

		DENTY_INFO_LOG << "Failed to remove " << filePath << " prefab from library!";
	}

	std::optional<Ref<Prefab>> PrefabLibrary::Find(PrefabID id) const
	{
		for (const auto& prefab : _caches | std::views::values)
		{
			if (prefab->GetPrefabID() != id)
				continue;

			return prefab;
		}

		return std::nullopt;
	}

	std::optional<Ref<Prefab>> PrefabLibrary::Find(const FilePath& filePath) const
	{
		const auto& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			return it->second;
		}

		return std::nullopt;
	}

	bool PrefabLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	bool PrefabLibrary::IsPrefabFile(const String& path) const
	{
		FilePath p = path.CStr();

		return (p.Extension() == PREFAB_FILE_EXTENSION);
	}

	//void PrefabLibrary::Remove(PrefabID prefabID)
	//{
	//	for (auto&& it = _prefabs.begin(); it != _prefabs.end(); ++it)
	//	{
	//		if (it->first.id != prefabID) continue;

	//		_prefabs.erase(it);

	//		break;
	//	}
	//}

	//void PrefabLibrary::Remove(const std::filesystem::path& filePath)
	//{
	//	for (auto&& it = _prefabs.begin(); it != _prefabs.end(); ++it)
	//	{
	//		if (it->first.path != filePath) continue;

	//		_prefabs.erase(it);

	//		break;
	//	}
	//}

	//Ref<Prefab> PrefabLibrary::Find(const std::filesystem::path& filePath) const
	//{
	//	for (const auto& prefab : _prefabs | std::views::values)
	//	{
	//		if (prefab->GetFilePathToSerialized() != filePath) continue;

	//		return prefab;
	//		//return std::make_shared<Prefab>(*prefab);
	//	}

	//	DENTY_SET_ERR_COLOR
	//	DENTY_ERR_LOG << "Prefab not found. (filePath: " << filePath << " )";

	//	return nullptr;
	//}

	//Ref<Prefab> PrefabLibrary::Find(PrefabID id) const
	//{
	//	for (const auto& prefab : _prefabs | std::views::values)
	//	{
	//		if (prefab->GetPrefabID() != id) continue;

	//		return prefab;
	//	}

	//	DENTY_SET_ERR_COLOR
	//	DENTY_ERR_LOG << "Prefab not found. (id: " << id << " )";

	//	return nullptr;
	//}

	void PrefabLibrary::OnPrefabOverrideButtonClick([[maybe_unused]] EventOnPrefabOverrideButtonClick* e)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		//const auto id = e->GetPrefabID();

		//if (const auto prefab = Find(id))
		//{
		//	// Remove from caches.
		//	Remove(prefab->GetPrefabID());

		//	// Get current data.
		//	const auto name = prefab->GetName();

		//	const auto object = e->GetSourceObject();

		//	// If different name, Remove.
		//	if (name != object->GetName())
		//	{
		//		std::filesystem::remove(prefab->GetFilePathToSerialized());
		//	}

		//	// Create prefab from source game object.
		//	// Keep same id.
		//	{
		//		const auto newPrefab = std::make_shared<Prefab>();
		//		const auto directory = DENTY_GET_FILE_PATH_WITHOUT_FILENAME(prefab->GetFilePathToSerialized());

		//		newPrefab->SetPrefabID(id);
		//		newPrefab->Create(object, directory);

		//		// Add to caches.
		//		Add(newPrefab);
		//	}
		//}
	}
}
