#include "stdafx.h"

#include "CubeMaterial.h"

#include "CubeMap.h"
#include "Shader.h"

#include "System/Resource/Library/TextureLibrary.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MaterialLibrary.h"
#include "System/Resource/Library/ShaderLibrary.h"

#include "System/Serializer/MaterialSerializer.h"
#include "System/Serializer/SerializeExtensions.h"

namespace DentyEngine
{
	CubeMaterialInitData::CubeMaterialInitData() noexcept :
		name(), filePath(), textureFilePath(), shader()
	{
	}

	CubeMaterial::CubeMaterial() noexcept :
		Object(), Asset(), _texture(), _textureFilePath(),
		_shader(),  _shaderFilePath()
	{
	}

	CubeMaterial::CubeMaterial(const CubeMaterial& source) noexcept :
		Object(source), Asset(source), _texture(source._texture),
		_textureFilePath(source._textureFilePath),
		_shader(source._shader), _shaderFilePath(source._shaderFilePath)
	{
	}

	CubeMaterial::~CubeMaterial() noexcept
	{
	}

	void CubeMaterial::Create(const CubeMaterialInitData& data)
	{
		DENTY_CHECK_CONDITION(data.shader);
		DENTY_CHECK_CONDITION(!data.filePath.IsEmpty());
		DENTY_CHECK_CONDITION(!data.textureFilePath.IsEmpty());

		const FilePath path = FilePath::MakePath(data.filePath.ObtainDirectory(), data.name, CUBE_MATERIAL_FILE_EXTENSION);

		_name = data.name;
		_filePath = path;
		_filePathToSerialized = path;

		_textureFilePath = data.textureFilePath;

		_shader = data.shader;
		_shaderFilePath = data.shader->GetFilePathToSerialized();

		Save();

		MaterialLibrary* materialLibrary = LibraryManager::GetInstance().GetMaterialLibrary();
		materialLibrary->Add(shared_from_this());
	}

	void CubeMaterial::RemoveTexture() noexcept
	{
		_texture = nullptr;
	}

	void CubeMaterial::ReplaceTexture(Ref<CubeMap> cubeMap)
	{
		if (HasTexture())
		{
			RemoveTexture();
		}

		SetTexture(cubeMap);
	}

	void CubeMaterial::SetTexture(Ref<CubeMap> cubeMap)
	{
		_texture = cubeMap;
		_textureFilePath = cubeMap->GetFilePathToSerialized();
	}

	Ref<Shader> CubeMaterial::GetShader() const noexcept
	{
		return _shader;
	}

	Ref<CubeMap> CubeMaterial::GetTexture() const noexcept
	{
		return _texture;
	}

	bool CubeMaterial::HasTexture() const noexcept
	{
		return (_texture != nullptr);
	}

	//
	// Object
	//

	void CubeMaterial::OnSerialized() const
	{
		// Nothing to do...
	}

	void CubeMaterial::OnDeserialized()
	{
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

		// Restore texture.
		{
			const std::optional<Ref<CubeMap>> cubeMap = textureLibrary->Find<CubeMap>(_textureFilePath);
			DENTY_CHECK_CONDITION(cubeMap.has_value());

			_texture = cubeMap.value();
		}

		// Restore shader.
		{
			const std::optional<Ref<Shader>> shader = shaderLibrary->Find(_shaderFilePath);
			DENTY_CHECK_CONDITION(shader.has_value());

			_shader = shader.value();
		}
	}

	//
	// Asset
	//

	void CubeMaterial::Save(bool isUpdate) const
	{
		// If failed.
		if (!MaterialSerializer::CubeSerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " cube material!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " cube material!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR
		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " material!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " material!";
		}
	}

	void CubeMaterial::Load(const std::filesystem::path& filePath)
	{
		auto material = std::make_shared<CubeMaterial>();

		// If failed.
		if (!MaterialSerializer::CubeDeserializeFromJson(filePath, material))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " cube material!";

			return;
		}

		material->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " cube material!";
	}

	void CubeMaterial::Rename([[maybe_unused]] std::string_view newName, [[maybe_unused]] bool deleteOldFile)
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__);
	}

	void CubeMaterial::OnReload()
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__);
	}

	void CubeMaterial::Clone(const Ref<Asset>& asset)
	{
		DENTY_CHECK_CONDITION(asset->GetAssetType() == Asset::AssetType::CubeMaterial);

		auto&& target = std::static_pointer_cast<CubeMaterial>(asset);

		// Object class
		{
			target->_name = _name;
		}

		// Asset class.
		{
			target->_previewImageFilePath = _previewImageFilePath;
			target->_filePath = _filePath;
			target->_filePathToSerialized = _filePathToSerialized;
		}

		// This class.
		{
			target->_texture = _texture;
			target->_textureFilePath = _textureFilePath;
			target->_shader = _shader;
			target->_shaderFilePath = _shaderFilePath;
		}
	}

	Asset::AssetType CubeMaterial::GetAssetType() const noexcept
	{
		return AssetType::CubeMaterial;
	}

	void CubeMaterial::Setup()
	{
		// Get default thumbnail from library.
		{
			const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

			const std::optional<Ref<Texture2D>> thumbnail = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::CUBE_MATERIAL_DEFAULT_THUMBNAIL_FILE_PATH);
			DENTY_CHECK_CONDITION(thumbnail.has_value());

			_defaultThumbnail = thumbnail.value();
		}
	}

	Ref<Texture2D> CubeMaterial::GetDefaultThumbnail() noexcept
	{
		return _defaultThumbnail;
	}

	//
	// Static variables.
	//
	Ref<Texture2D> CubeMaterial::_defaultThumbnail = nullptr;
}
