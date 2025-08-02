#include "stdafx.h"

#include "ScriptLibrary.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "System/Resource/Script.h"
#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	ScriptLibrary::ScriptLibrary() :
		ILibrary(), _caches()
	{
	}

	void ScriptLibrary::Initialize()
	{
		auto sceneFolderPath = EditorContentBrowser::ENTRY_FILE_PATH;
		auto contentSerializer = [this](auto self, FilePath& folderPath) -> void
		{
			for (auto& content : std::filesystem::directory_iterator(folderPath.Get()))
			{
				const auto& entryPath = content.path();
				const std::string entry = entryPath.filename().string();

				if (content.is_directory())
				{
					self(self, folderPath /= entry);
					folderPath = folderPath.ParentPath();
				}
				else
				{
					String path = entryPath.string();
					if (IsScriptFile(path))
					{
						const auto script = std::make_shared<Script>();
						script->Load(entryPath);

						Add(script);
					}
				}
			}
		};
		contentSerializer(contentSerializer, sceneFolderPath);
	}

	void ScriptLibrary::Update()
	{
	}

	void ScriptLibrary::OnGui()
	{
	}

	void ScriptLibrary::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void ScriptLibrary::Finalize()
	{
	}

	void ScriptLibrary::OnSerialized() const
	{
	}

	void ScriptLibrary::OnDeserialized()
	{
	}

    void ScriptLibrary::ReloadAt(
		const FilePath& targetScriptFilePathToSerialized,
        const std::map<std::string, ScriptField>& newScriptFields
	)
    {
		auto&& it = _caches.find(targetScriptFilePathToSerialized);
		if (it == _caches.end())
		{
			DENTY_ASSERT(false, "Script not found! (in OnScriptAssemblyReloaded)");
		}

		it->second->OnScriptAssemblyReloaded(newScriptFields);
    }

    void ScriptLibrary::Add(Ref<Script> script)
	{
		const FilePath& filePath = script->GetFilePathToSerialized();

		if (filePath.IsEmpty())
		{
			DENTY_ASSERT(false, "File path is empty! (in ScriptLibrary::Add)");
		}

		_caches.try_emplace(filePath, script);
	}

	void ScriptLibrary::Remove(const FilePath& filePath)
	{
		auto&& it = _caches.find(filePath);

		if (it != _caches.end())
		{
			DENTY_SET_INFO_COLOR

			DENTY_INFO_LOG << "Succeed to remove " << filePath << " script from library!";

			_caches.erase(it);

			return;

		}

		DENTY_SET_ERR_COLOR
		DENTY_INFO_LOG << "Failed to remove " << filePath << " script from library!";
	}

	std::optional<Ref<Script>> ScriptLibrary::Find(const FilePath& filePath) const
	{
		auto&& it = _caches.find(filePath);
		if (it != _caches.end())
		{
			return it->second;
		}

		return std::nullopt;
	}

	bool ScriptLibrary::Reload([[maybe_unused]] const FilePath& path)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return false;
	}

	bool ScriptLibrary::IsScriptFile(const String& path) const
	{
		return path.Contains(SCRIPT_FILE_EXTENSION);
	}

	bool ScriptLibrary::Exists(const FilePath& filePath)
	{
		for (const auto& script : _caches)
		{
			if (script.first != filePath)
				continue;

			return true;
		}

		return false;
	}
}
