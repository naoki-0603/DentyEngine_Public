#include "stdafx.h"

#include "Setting.h"

namespace DentyEngine
{
    Setting::Setting() :
        name(), _filePathToSerialized()
    {
    }

    Setting::Setting(const Setting& source) :
        name(source.name), _filePathToSerialized(source._filePathToSerialized)
    {
    }

    void Setting::Clone(const Ref<Setting>& setting)
    {
        setting->name = name;
        setting->_filePathToSerialized = _filePathToSerialized;
    }

    void Setting::SetFilePath(const FilePath& filePath)
    {
        _filePathToSerialized = filePath;
    }
}
