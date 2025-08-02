#include "stdafx.h"

#include "ScriptUtils.h"
#include "ScriptClass.h"

#include <mono/metadata/assembly.h>

namespace DentyEngine
{
    namespace Utils
    {
        void PrintAssemblyTypes(MonoAssembly* assembly)
        {
            MonoImage* image = mono_assembly_get_image(assembly);

            const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
            int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

            for (int32_t i = 0; i < numTypes; ++i)
            {
                uint32_t cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

                const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

                printf("%s.%s\n", nameSpace, name);
            }
        }

        bool CheckMonoError(MonoError& error)
        {
            const bool hasError = (not mono_error_ok(&error));
            if (hasError)
            {
                const uint16_t errorCode = mono_error_get_error_code(&error);
                const char* errorMessage = mono_error_get_message(&error);

                DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Mono Error!";
                DENTY_ERR_LOG << "Error Code: " << errorCode;
                DENTY_ERR_LOG << "Error Message: " << errorMessage;

                mono_error_cleanup(&error);
            }

            return hasError;
        }

        std::string MonoStringToUTF8(MonoString* monoString)
        {
            // Return if mono string is empty.
            if (monoString == nullptr || mono_string_length(monoString) == 0)
                return "";

            MonoError error;
            char* utf8 = mono_string_to_utf8_checked(monoString, &error);

            if (CheckMonoError(error))
                return "";

            std::string result(utf8);
            mono_free(utf8);

            return result;
        }

        MonoString* CreateMonoString(MonoDomain* domain, const char* string)
        {
            MonoString* monoString = mono_string_new(domain, string);
            if (not monoString)
            {
                DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Failed to create mono string!";

                return nullptr;
            }

            return monoString;
        }

        void* GetFieldValue(MonoClass* monoClass, const char* memberName)
        {
            MonoObject* instance = ScriptClass::Instantiate(monoClass);
        	MonoClass* instancedClass = mono_object_get_class(instance);

        	MonoClassField* field = mono_class_get_field_from_name(instancedClass, memberName);

            void* value = nullptr;
            mono_field_get_value(instance, field, value);

            return value;
        }
    }
}
