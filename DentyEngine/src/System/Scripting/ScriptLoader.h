#pragma once

#include "System/Core/DFilePath.h"

#include <../mono/metadata/assembly.h>

namespace DentyEngine
{
    namespace Utils
    {
        static char* ReadBytes(const FilePath& filePath, uint32_t* outSize);
    }

    class ScriptLoader final
    {
    public:
        ScriptLoader();
        ~ScriptLoader() = default;

        static MonoAssembly* LoadCSharpAssembly(const FilePath& assemblyFilePath, _Out_ bool& outIsLoadSuccessfully, bool loadPDB = false);
    };
}
