#include "stdafx.h"

#include "Asset.h"

#include "Graphics/Renderer/Texture2D.h"

namespace DentyEngine
{
	Asset::Asset() :
		_previewImage(), _previewImageFilePath(), _filePath(), _filePathToSerialized(),
	    _referenceObjects()
	{
	}

	Asset::Asset(const Asset& source) :
		_previewImageFilePath(source._previewImageFilePath),
		_filePath(source._filePath),
		_filePathToSerialized(source._filePathToSerialized),
	    _referenceObjects(source._referenceObjects)
	{
		if (source._previewImage)
		{
			_previewImage = std::make_shared<Texture2D>(*source._previewImage);
		}
	}

    void Asset::ReGenerateID()
    {
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)
    }

	void Asset::SetPreviewImage(const Ref<Texture2D>& previewImage)
	{
		_previewImage = previewImage;
		_previewImageFilePath = _previewImage->GetFilePathToSerialized();
	}
}
