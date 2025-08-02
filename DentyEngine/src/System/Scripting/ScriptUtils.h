#pragma once

#include <mono/metadata/object.h>

#include "mono/utils/mono-error.h"

namespace DentyEngine
{
    namespace Utils
    {
        static void PrintAssemblyTypes(MonoAssembly* assembly);

        [[nodiscard]]
        static bool CheckMonoError(MonoError& error);

        [[nodiscard]]
        std::string MonoStringToUTF8(MonoString* monoString);

        [[nodiscard]]
        MonoString* CreateMonoString(MonoDomain* domain, const char* string);

        [[nodiscard]]
        void* GetFieldValue(MonoClass* monoClass, const char* memberName);
    }
}