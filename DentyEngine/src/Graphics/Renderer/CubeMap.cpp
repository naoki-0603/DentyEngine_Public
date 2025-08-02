#include "stdafx.h"

#include "CubeMap.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"
#include "System/Serializer/SerializeExtensions.h"
#include "System/Serializer/TextureSerializer.h"

#include "Utilities/StringUtilities.h"

namespace DentyEngine
{
	CubeMap::CubeMap() :
		Texture()
	{
		_type = Type::CubeMap;
	}

	CubeMap& CubeMap::operator=(const CubeMap& source)
	{
		Texture::operator=(source);

		return *this;
	}

	void CubeMap::Load(ID3D11Device* device, const std::filesystem::path& filePath)
	{
		ComRef<ID3D11Resource> textureResource = {};
		HRESULT hr = S_OK;

		_name = filePath.stem().string();

		// Create texture
		{
			const std::wstring path = StringUtils::StringToWString(filePath.string());

			hr = DirectX::CreateDDSTextureFromFile(
				device, path.c_str(), textureResource.ReleaseAndGetAddressOf(),
				_shaderResourceView.GetAddressOf()
			);

			if (FAILED(hr))
			{
				// TODO Add error console log.

				return;
			}
		}

		// Extract data
		{
			D3D11_TEXTURE2D_DESC texture2dDesc = {};
			ComRef<ID3D11Texture2D> texture2d;

			hr = textureResource->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

			texture2d->GetDesc(&texture2dDesc);

			this->_size = Vector2(static_cast<float>(texture2dDesc.Width), static_cast<float>(texture2dDesc.Height));
			this->_format = texture2dDesc.Format;
		}

		{
			_filePath = filePath;
			_filePathToSerialized = FilePath(filePath).ReplaceExtension(TEXTURE_FILE_EXTENSION);
		}

		// Save
		{
			Save();
		}

		// Add to library.
		{
			TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

			textureLibrary->Add(shared_from_this());
		}
	}

	void CubeMap::Save(bool isUpdate) const
	{
		// If failed.
		if (!TextureSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " texture!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " texture!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " texture!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " texture!";
		}
	}

	void CubeMap::Load(const std::filesystem::path& filePath)
	{
		auto cubeMap = std::make_shared<CubeMap>();

		// If failed.
		if (!TextureSerializer::DeserializeFromJson(filePath, cubeMap))
		{
			DENTY_SET_ERR_COLOR

			DENTY_ERR_LOG << "Failed to load " << filePath << " texture!";

			return;
		}

		cubeMap->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " texture!";
	}

	void CubeMap::Rename(std::string_view newName, bool deleteOldFile)
	{
		Texture::Rename(newName, deleteOldFile);
	}

	void CubeMap::Clone(const Ref<Asset>& asset)
	{
		Texture::Clone(asset);
	}

	void CubeMap::OnSerialized() const
	{
		Texture::OnSerialized();
	}

	void CubeMap::OnDeserialized()
	{
		Texture::OnDeserialized();

		if (not _filePath.IsEmpty())
		{
			if (not _shaderResourceView)
			{
				ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

				Load(device, _filePath.Get());
			}
		}
	}
};