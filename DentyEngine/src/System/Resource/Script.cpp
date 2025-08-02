#include "stdafx.h"

#include "Script.h"

#include "System/Serializer/ScriptSerializer.h"
#include "System/Serializer/SerializeExtensions.h"

#include "Graphics/Renderer/Texture2D.h"

#include "System/Scripting/ScriptClass.h"
#include "System/Scripting/ScriptEngine.h"

#include "Library/LibraryManager.h"
#include "Library/ScriptLibrary.h"
#include "Library/TextureLibrary.h"
#include "System/Scripting/ScriptObject.h"

namespace DentyEngine
{
	Ref<Texture2D> Script::_assetTexture = nullptr;

	Script::Script() :
		Object(), Asset(), _fields()
	{
	}

	Script::Script(const Script& source) :
		Object(source), Asset(source), _fields(source._fields)
	{
	}

	void Script::Create(const FilePath& filePath)
	{
		// Create file paths.
		{
		    _name = filePath.Filename().GetAsString();
		    _filePath = FilePath(ScriptEngine::SCRIPT_SAVE_FOLDER_PATH.GetAsString() + _name.Get() + ".cs");
		    _filePathToSerialized = FilePath(filePath).ReplaceExtension(SCRIPT_FILE_EXTENSION);
		}

		// Generate C# script file.
		{
		    GenerateCSharpScript();
		}

		// Save
		{
		    Save();
		}
		
		// Add library
		{
		    ScriptLibrary* scriptLibrary = LibraryManager::GetInstance().GetScriptLibrary();
			scriptLibrary->Add(shared_from_this());
		}
	}

    void Script::OnGui()
    {
		ImGui::Text("Name: %s", _name.CStr());
		ImGui::Text("FilePath: %s", _filePath.GetAsString().c_str());
		ImGui::Text("FilePathToSerialized: %s", _filePathToSerialized.GetAsString().c_str());
    }

    void Script::OnEvent(Event* e)
	{
		Object::OnEvent(e);
	}

	void Script::OnSerialized() const
	{
		Object::OnSerialized();
	}

	void Script::OnDeserialized()
	{
		Object::OnDeserialized();
	}

	void Script::OnDestroy()
	{
		Object::OnDestroy();
	}

	void Script::OnReload()
	{
	}

	void Script::Save(bool isUpdate) const
	{
		// If failed
		if (!ScriptSerializer::SerializeAsBinary(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " script!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " script!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " script!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " script!";
		}
	}

	void Script::Load(const std::filesystem::path& filePath)
	{
		auto script = std::make_shared<Script>();

		// If failed
		if (!ScriptSerializer::DeserializeFromBinary(filePath, script))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " script!";

			return;
		}

		script->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " script!";
	}

	void Script::Rename([[maybe_unused]] std::string_view newName, [[maybe_unused]] bool deleteOldFile)
	{
	}

	void Script::Clone(const Ref<Asset>& asset)
	{
		DENTY_ASSERT(asset->GetAssetType() == AssetType::Script, "Asset type must be script! (in Script::Clone)");

		auto&& target = std::static_pointer_cast<Script>(asset);

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

		// This class
		{
			target->_fields = _fields;
		}
	}

    void Script::OnScriptAssemblyReloaded(const std::map<std::string, ScriptField>& newScriptFields)
	{
		_fields.clear();
		_fields = newScriptFields;

		Save(true);
	}

    void Script::LoadAssetTexture()
	{
		DENTY_ASSERT(not _assetTexture, "Script asset texture already loaded!");

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		_assetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::CSHARP_SCRIPT_DEFAULT_THUMBNAIL_FILE_PATH).value();

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Script asset texture successfully loaded!";
	}

    void Script::GenerateCSharpScript()
    {
        const std::string filePath = FilePath(_filePath).RemoveExtension().GetAsString();
		ScriptEngine::GenerateCSharpFile(filePath.c_str());
    }
}
