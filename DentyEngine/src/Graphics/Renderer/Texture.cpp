#include "stdafx.h"

#include "Texture.h"

#include "System/Graphics/GraphicsManager.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "System/Serializer/TextureSerializer.h"

#include "Utilities/StringUtilities.h"

namespace DentyEngine
{
	const String Texture::ADD_FILENAME_AT_END = "_DentyTexture";

	Texture::Texture() : Asset(), Object(),
		_format(), _textureID(),
		_type(Type::None), _alias(), _size(), _shaderResourceView(), _isDummyTexture()
	{
	}

	Texture& Texture::operator=(const Texture& source)
	{
		Asset::operator=(source);
		Object::operator=(source);

		_format = source._format;
		_textureID = source._textureID;
		_type = source._type;
		_alias = source._alias;
		_size = source._size;
		source._shaderResourceView.CopyTo(_shaderResourceView.ReleaseAndGetAddressOf());

		_isDummyTexture = source._isDummyTexture;

		return *this;
	}

	void Texture::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void Texture::OnSerialized() const
	{
	}

	void Texture::OnDeserialized()
	{
	}

	void Texture::Save([[maybe_unused]] bool isUpdate) const
	{

	}

	void Texture::Load([[maybe_unused]] const std::filesystem::path& filePath)
	{
	}

	void Texture::Rename(std::string_view newName, [[maybe_unused]] bool deleteOldFile)
	{
		_name = newName;

		/*if (deleteOldFile)
		{
			std::filesystem::remove(_filePathToSerialized);
		}

		DENTY_REPLACE_FILENAME(_filePath, newName);
		DENTY_REPLACE_FILENAME(_filePathToSerialized, newName);*/
	}

	void Texture::Clone(const Ref<Asset>& asset)
	{
		DENTY_ASSERT(asset->GetAssetType() == AssetType::Texture, "Asset type must be Texture. (in Texture::Clone)");

		auto&& target = std::static_pointer_cast<Texture>(asset);

		target->_name = _name;

		target->_size = _size;
		target->_alias = _alias;
		target->_format = _format;
		target->_textureID = _textureID;
		target->_type = _type;
		target->_isDummyTexture = _isDummyTexture;

		_shaderResourceView.CopyTo(target->_shaderResourceView.ReleaseAndGetAddressOf());

		target->_filePath = _filePath;
		target->_filePathToSerialized = _filePathToSerialized;
	}

	void Texture::OnReload()
	{
		// nothing to do.
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)
	}

	bool Texture::SaveTextureToFile(ID3D11DeviceContext* context, ID3D11Resource* buffer, std::string_view filePath, [[maybe_unused]] const Vector2& size, [[maybe_unused]] DXGI_FORMAT format)
	{
		const auto path = StringUtils::StringToWString(std::string(filePath));

        const HRESULT hr = DirectX::SaveWICTextureToFile(context, buffer,
                                                         GUID_ContainerFormatPng, path.c_str()
        );

		if (SUCCEEDED(hr))
		{
			DENTY_SET_INFO_COLOR
		    DENTY_INFO_LOG << "The png file was successfully exported! (filePath : " << StringUtils::WStringToString(std::wstring(path)) << " )";

			return true;
		}

		DENTY_SET_ERR_COLOR

		DENTY_ERR_LOG << "Failed to output png file. (filePath: " << StringUtils::WStringToString(std::wstring(path)) << " )";

		return false;
	}
};