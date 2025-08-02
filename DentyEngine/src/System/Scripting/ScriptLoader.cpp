#include "stdafx.h"

#include "ScriptLoader.h"

#include <mono/metadata/mono-debug.h>

namespace DentyEngine
{
    namespace Utils
    {
        char* ReadBytes(const FilePath& filePath, uint32_t* outSize)
        {
            std::ifstream stream(filePath.GetAsString(), std::ios::binary | std::ios::ate);

            if (not stream)
            {
                DENTY_SET_ERR_COLOR
            	DENTY_ERR_LOG << "Failed to open file!";

                *outSize = 0u;
                return nullptr;
            }

            // Calculate file size.
            std::streampos end = stream.tellg();
            stream.seekg(0, std::ios::beg);
            const auto size = static_cast<uint32_t>(end - stream.tellg());

            // File is empty
            if (size == 0u)
            {
                return nullptr;
            }

            // Read file.
            char* buffer = new char[size];
            stream.read((char*)buffer, size);
            stream.close();

            *outSize = size;

            return buffer;
        }
    }

    ScriptLoader::ScriptLoader()
    {
    }

    MonoAssembly* ScriptLoader::LoadCSharpAssembly(const FilePath& assemblyFilePath, _Out_ bool& outIsLoadSuccessfully, bool loadPDB)
    {
        uint32_t fileSize = 0u;
        char* fileData = Utils::ReadBytes(assemblyFilePath, &fileSize);

        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* errorMessage = mono_image_strerror(status);

            DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Failed to open mono image. (ErrorMessage: " << errorMessage << " )";

            outIsLoadSuccessfully = false;

            return nullptr;
        }

        if (loadPDB)
        {
            FilePath pdbFilePath = assemblyFilePath;
            pdbFilePath.ReplaceExtension(".pdb");

            if (pdbFilePath.IsValid())
            {
                uint32_t outSize = 0u;
                const char* buffer = Utils::ReadBytes(pdbFilePath, &outSize);
                mono_debug_open_image_from_memory(image, reinterpret_cast<const mono_byte*>(buffer), static_cast<int32_t>(outSize));

                DENTY_SET_INFO_COLOR
                DENTY_INFO_LOG << "PDB file loaded! (Path: " << pdbFilePath << ")";

                delete[] buffer;
            }
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyFilePath.GetAsString().c_str(), &status, 0);
        mono_image_close(image);

        delete[] fileData;

        outIsLoadSuccessfully = true;

        return assembly;
    }
}