#include "stdafx.h"

#include "ScriptField.h"

#include <mono/metadata/class.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

#include "ScriptEngine.h"

namespace DentyEngine
{
    const std::map<std::string, ScriptFieldType> ScriptField::SCRIPT_FIELD_TYPES =
    {
        // System.
        { "System.Boolean", ScriptFieldType::Bool },
        { "System.Single", ScriptFieldType::Float },
        { "System.Double", ScriptFieldType::Double },
        { "System.SByte", ScriptFieldType::SByte },
        { "System.Int16", ScriptFieldType::Short },
        { "System.Int32", ScriptFieldType::Int },
        { "System.Int64", ScriptFieldType::Long },
        { "System.Byte", ScriptFieldType::Byte },
        { "System.UInt16", ScriptFieldType::UShort },
        { "System.UInt32", ScriptFieldType::UInt },
        { "System.UInt64", ScriptFieldType::ULong },
        { "System.String", ScriptFieldType::String },

        // DentyEngine.
        { "DentyEngine.Vector2", ScriptFieldType::Vector2 },
        { "DentyEngine.Vector3", ScriptFieldType::Vector3 },
        { "DentyEngine.Vector4", ScriptFieldType::Vector4 },
        { "DentyEngine.Quaternion", ScriptFieldType::Quaternion },
        { "DentyEngine.Matrix", ScriptFieldType::Matrix },
        { "DentyEngine.Vector2I", ScriptFieldType::Vector2I },
        { "DentyEngine.Vector3I", ScriptFieldType::Vector3I },
        { "DentyEngine.Vector4I", ScriptFieldType::Vector4I },

        { "DentyEngine.VehicleMoveStatus", ScriptFieldType::VehicleMoveStatus }
    };

    namespace Utils
    {
        uint8_t GetFieldAccessibility(MonoClassField* field)
        {
            auto accessibility = static_cast<uint8_t>(Accessibility::None);
            const uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

            switch (accessFlag)
            {
            case MONO_FIELD_ATTR_PRIVATE:
                accessibility = static_cast<uint8_t>(Accessibility::Private);
                break;
            case MONO_FIELD_ATTR_FAM_AND_ASSEM:
                accessibility |= static_cast<uint8_t>(Accessibility::Protected);
                accessibility |= static_cast<uint8_t>(Accessibility::Internal);
                break;
            case MONO_FIELD_ATTR_ASSEMBLY:
                accessibility = static_cast<uint8_t>(Accessibility::Internal);
                break;
            case MONO_FIELD_ATTR_FAMILY:
                accessibility = static_cast<uint8_t>(Accessibility::Protected);
                break;
            case MONO_FIELD_ATTR_FAM_OR_ASSEM:
                accessibility |= static_cast<uint8_t>(Accessibility::Private);
                accessibility |= static_cast<uint8_t>(Accessibility::Protected);
                break;
            case MONO_FIELD_ATTR_PUBLIC:
                accessibility = static_cast<uint8_t>(Accessibility::Public);
                break;
            }

            return accessibility;
        }

        uint8_t GetPropertyAccessibility(MonoProperty* property)
        {
            auto accessibility = static_cast<uint8_t>(Accessibility::None);

            // Get a reference to the property's getter method
            MonoMethod* propertyGetter = mono_property_get_get_method(property);
            if (propertyGetter != nullptr)
            {
                // Get getter accessibility flags.
                uint32_t accessFlag = mono_method_get_flags(propertyGetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

                switch (accessFlag)
                {
                case MONO_FIELD_ATTR_PRIVATE:
                    accessibility = static_cast<uint8_t>(Accessibility::Private);
                    break;
                case MONO_FIELD_ATTR_FAM_AND_ASSEM:
                    accessibility |= static_cast<uint8_t>(Accessibility::Protected);
                    accessibility |= static_cast<uint8_t>(Accessibility::Internal);
                    break;
                case MONO_FIELD_ATTR_ASSEMBLY:
                    accessibility = static_cast<uint8_t>(Accessibility::Internal);
                    break;
                case MONO_FIELD_ATTR_FAMILY:
                    accessibility = static_cast<uint8_t>(Accessibility::Protected);
                    break;
                case MONO_FIELD_ATTR_FAM_OR_ASSEM:
                    accessibility |= static_cast<uint8_t>(Accessibility::Private);
                    accessibility |= static_cast<uint8_t>(Accessibility::Protected);
                    break;
                case MONO_FIELD_ATTR_PUBLIC:
                    accessibility = static_cast<uint8_t>(Accessibility::Public);
                    break;
                }
            }

            // Get a reference to the property's setter method
            MonoMethod* propertySetter = mono_property_get_set_method(property);
            if (propertySetter != nullptr)
            {
                // Get setter accessibility flags.
                const uint32_t accessFlag = mono_method_get_flags(propertySetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
                if (accessFlag != MONO_FIELD_ATTR_PUBLIC)
                    accessibility = static_cast<uint8_t>(Accessibility::Private);
            }
            else
            {
                accessibility = static_cast<uint8_t>(Accessibility::Private);
            }

            return accessibility;
        }

        const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
        {
            return std::string(NAMEOF_ENUM(fieldType)).c_str();
        }

        ScriptFieldType ScriptFieldTypeFromString(const String& fieldType)
        {
            if (fieldType.Get() == "None") return ScriptFieldType::None;
            if (fieldType.Get() == "Bool") return ScriptFieldType::Bool;
            if (fieldType.Get() == "Float") return ScriptFieldType::Float;
            if (fieldType.Get() == "Double") return ScriptFieldType::Double;
            if (fieldType.Get() == "SByte") return ScriptFieldType::SByte;
            if (fieldType.Get() == "Byte") return ScriptFieldType::Byte;
            if (fieldType.Get() == "Short") return ScriptFieldType::Short;
            if (fieldType.Get() == "Int") return ScriptFieldType::Int;
            if (fieldType.Get() == "Long") return ScriptFieldType::Long;
            if (fieldType.Get() == "UShort") return ScriptFieldType::UShort;
            if (fieldType.Get() == "UInt") return ScriptFieldType::UInt;
            if (fieldType.Get() == "ULong") return ScriptFieldType::ULong;
            if (fieldType.Get() == "String") return ScriptFieldType::String;
            if (fieldType.Get() == "Vector2") return ScriptFieldType::Vector2;
            if (fieldType.Get() == "Vector3") return ScriptFieldType::Vector3;
            if (fieldType.Get() == "Vector4") return ScriptFieldType::Vector4;
            if (fieldType.Get() == "Quaternion") return ScriptFieldType::Quaternion;
            if (fieldType.Get() == "Matrix") return ScriptFieldType::Matrix;
            if (fieldType.Get() == "Vector2I") return ScriptFieldType::Vector2I;
            if (fieldType.Get() == "Vector3I") return ScriptFieldType::Vector3I;
            if (fieldType.Get() == "Vector4I") return ScriptFieldType::Vector4I;
            if (fieldType.Get() == "VehicleMoveStatus") return ScriptFieldType::VehicleMoveStatus;

            DENTY_ASSERT(false, "Unknown script field type! (in ScriptFieldTypeFromString)");
            return ScriptFieldType::None;
        }
    }

    ScriptField::ScriptField() :
        _name(), _className(), _fieldType(), _fieldValues(), _monoClassField()
    {
    }

    void ScriptField::Create(const String& name, const String& className, ScriptFieldType type, MonoClassField* monoClassField, MonoObject* classInstance)
    {
        _name = name;
        _className = className;
        _fieldType = type;
        _monoClassField = monoClassField;

        GetValues(classInstance);
    }

    void ScriptField::SetValues(MonoObject* object)
    {
        switch (_fieldType)
        {
        case ScriptFieldType::Bool:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueBool);
            break;
        case ScriptFieldType::Float:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueFloat);
            break;
        case ScriptFieldType::Double:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueDouble);
            break;
        case ScriptFieldType::SByte:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueInt8);
            break;
        case ScriptFieldType::Byte:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueUInt8);
            break;
        case ScriptFieldType::Short:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueInt16);
            break;
        case ScriptFieldType::Int:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueInt32);
            break;
        case ScriptFieldType::Long:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueInt64);
            break;
        case ScriptFieldType::UShort:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueUInt16);
            break;
        case ScriptFieldType::UInt:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueUInt32);
            break;
        case ScriptFieldType::ULong:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueUInt64);
            break;
        case ScriptFieldType::String:
            mono_field_set_value(object, _monoClassField, &_fieldValues.valueString);
            break;
        case ScriptFieldType::Quaternion:

            mono_field_set_value(object, _monoClassField, &_fieldValues.valueQuaternion);

            break;
        case ScriptFieldType::Matrix:

            mono_field_set_value(object, _monoClassField, &_fieldValues.valueMatrix);

            break;
        case ScriptFieldType::Vector2:

            mono_field_set_value(object, _monoClassField, &_fieldValues.valueVector2);

            break;
        case ScriptFieldType::Vector3:

            mono_field_set_value(object, _monoClassField, &_fieldValues.valueVector3);

            break;
        case ScriptFieldType::Vector4:

            mono_field_set_value(object, _monoClassField, &_fieldValues.valueVector4);

            break;
        case ScriptFieldType::Vector2I:

            mono_field_set_value(object, _monoClassField, &_fieldValues.valueVector2I);

            break;
        case ScriptFieldType::Vector3I:

            mono_field_set_value(object, _monoClassField, &_fieldValues.valueVector3I);

            break;
        case ScriptFieldType::Vector4I:

            mono_field_set_value(object, _monoClassField, &_fieldValues.valueVector4I);

            break;
        }
    }

    void ScriptField::GetValues(MonoObject* object)
    {
        switch (_fieldType)
        {
        case ScriptFieldType::Bool:
            {
                bool value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueBool = value;
            }
            break;
        case ScriptFieldType::Float:
            {
                float value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueFloat = value;
            }
            break;
        case ScriptFieldType::Double:
            {
                double value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueDouble = value;
            }
            break;
        case ScriptFieldType::SByte:
            {
                int8_t value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueInt8 = value;
            }
            break;
        case ScriptFieldType::Byte:
            {
                uint8_t value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueUInt8 = value;
            }
            break;
        case ScriptFieldType::Short:
            {
                int16_t value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueInt16 = value;
            }
            break;
        case ScriptFieldType::Int:
            {
                int32_t value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueInt32 = value;
            }
            break;
        case ScriptFieldType::Long:
            {
                int64_t value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueInt64 = value;
            }
            break;
        case ScriptFieldType::UShort:
            {
                uint16_t value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueUInt16 = value;
            }
            break;
        case ScriptFieldType::UInt:
            {
                uint32_t value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueUInt32 = value;
            }
            break;
        case ScriptFieldType::ULong:
            {
                uint64_t value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueUInt64 = value;
            }
            break;
        case ScriptFieldType::String:
            {
                std::string value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueString = value;
            }
            break;
        case ScriptFieldType::Vector2:
            {
                Vector2 value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueVector2 = value;
            }
            break;
        case ScriptFieldType::Vector3:
            {
                Vector3 value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueVector3 = value;
            }
            break;
        case ScriptFieldType::Vector4:
            {
                Vector4 value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueVector4 = value;
            }
            break;
        case ScriptFieldType::Quaternion:
            {
                Quaternion value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueQuaternion = value;
            }
            break;
        case ScriptFieldType::Matrix:
            {
                Matrix value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueMatrix = value;
            }
            break;
        case ScriptFieldType::Vector2I:
            {
                Vector2I value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueVector2I = value;
            }
            break;
        case ScriptFieldType::Vector3I:
            {
                Vector3I value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueVector3I = value;
            }
            break;
        case ScriptFieldType::Vector4I:
            {
                Vector4I value;
                mono_field_get_value(object, _monoClassField, &value);

                _fieldValues.valueVector4I = value;
            }
            break;
        }
    }

    bool ScriptField::GetFieldValueBool() const
    {
        if (_fieldType != ScriptFieldType::Bool)
            DENTY_ASSERT(false, "Field type must be Bool! (in ScriptField::GetFieldValueBool)");

        // It should be exists.
        return _fieldValues.valueBool;
    }

    float ScriptField::GetFieldValueFloat() const
    {
        if (_fieldType != ScriptFieldType::Float)
            DENTY_ASSERT(false, "Field type must be float! (in ScriptField::GetFieldValueFloat)");

        // It should be exists.
        return _fieldValues.valueFloat;
    }

    double ScriptField::GetFieldValueDouble() const
    {
        if (_fieldType != ScriptFieldType::Double)
            DENTY_ASSERT(false, "Field type must be double! (in ScriptField::GetFieldValueDouble)");

        // It should be exists.
        return _fieldValues.valueDouble;
    }

    int8_t ScriptField::GetFieldValueInt8() const
    {
        if (_fieldType != ScriptFieldType::SByte)
            DENTY_ASSERT(false, "Field type must be sbyte! (in ScripField::GetFieldValueInt8)");

        return _fieldValues.valueInt8;
    }

    int16_t ScriptField::GetFieldValueInt16() const
    {
        if (_fieldType != ScriptFieldType::Short)
            DENTY_ASSERT(false, "Field type must be short! (in ScripField::GetFieldValueInt16)");

        return _fieldValues.valueInt16;
    }

    int32_t ScriptField::GetFieldValueInt32() const
    {
        if (_fieldType != ScriptFieldType::Int)
            DENTY_ASSERT(false, "Field type must be int! (in ScripField::GetFieldValueInt32)");

        return _fieldValues.valueInt32;
    }

    int64_t ScriptField::GetFieldValueInt64() const
    {
        if (_fieldType != ScriptFieldType::Long)
            DENTY_ASSERT(false, "Field type must be long! (in ScripField::GetFieldValueInt64)");

        return _fieldValues.valueInt64;
    }

    uint8_t ScriptField::GetFieldValueUInt8() const
    {
        if (_fieldType != ScriptFieldType::Byte)
            DENTY_ASSERT(false, "Field type must be byte! (in ScripField::GetFieldValueUInt8)");

        return _fieldValues.valueUInt8;
    }

    uint16_t ScriptField::GetFieldValueUInt16() const
    {
        if (_fieldType != ScriptFieldType::UShort)
            DENTY_ASSERT(false, "Field type must be ushort! (in ScripField::GetFieldValueUInt16)");

        return _fieldValues.valueUInt16;
    }

    uint32_t ScriptField::GetFieldValueUInt32() const
    {
        if (_fieldType != ScriptFieldType::UInt)
            DENTY_ASSERT(false, "Field type must be uint! (in ScripField::GetFieldValueUInt32)");

        return _fieldValues.valueUInt32;
    }

    uint64_t ScriptField::GetFieldValueUInt64() const
    {
        if (_fieldType != ScriptFieldType::ULong)
            DENTY_ASSERT(false, "Field type must be ulong! (in ScriptField::GetFieldValueUInt64)");

        return _fieldValues.valueUInt64;
    }

    const std::string& ScriptField::GetFieldValueString() const
    {
        if (_fieldType != ScriptFieldType::String)
            DENTY_ASSERT(false, "Field type must be string! (in ScriptField::GetFieldValueString)");

        return _fieldValues.valueString;
    }

    const Vector2& ScriptField::GetFieldValueVector2() const
    {
        if (_fieldType != ScriptFieldType::Vector2)
            DENTY_ASSERT(false, "Field type must be vector2! (in ScriptField::GetFieldValueVector2)");

        return _fieldValues.valueVector2;
    }

    const Vector3& ScriptField::GetFieldValueVector3() const
    {
        if (_fieldType != ScriptFieldType::Vector3)
            DENTY_ASSERT(false, "Field type must be vector3! (in ScriptField::GetFieldValueVector3)");

        return _fieldValues.valueVector3;
    }

    const Vector4& ScriptField::GetFieldValueVector4() const
    {
        if (_fieldType != ScriptFieldType::Vector4)
            DENTY_ASSERT(false, "Field type must be vector4! (in ScriptField::GetFieldValueVector4)");

        return _fieldValues.valueVector4;
    }

    const Quaternion& ScriptField::GetFieldValueQuaternion() const
    {
        if (_fieldType != ScriptFieldType::Quaternion)
            DENTY_ASSERT(false, "Field type must be quaternion! (in ScriptField::GetFieldValueQuaternion)");

        return _fieldValues.valueQuaternion;
    }

    const Matrix& ScriptField::GetFieldValueMatrix() const
    {
        if (_fieldType != ScriptFieldType::Matrix)
            DENTY_ASSERT(false, "Field type must be matrix! (in ScriptField::GetFieldValueMatrix)");

        return _fieldValues.valueMatrix;
    }

    const Vector2I& ScriptField::GetFieldValueVector2I() const
    {
        if (_fieldType != ScriptFieldType::Vector2I)
            DENTY_ASSERT(false, "Field type must be vector2I! (in ScriptField::GetFieldValueVector2I)");

        return _fieldValues.valueVector2I;
    }

    const Vector3I& ScriptField::GetFieldValueVector3I() const
    {
        if (_fieldType != ScriptFieldType::Vector3I)
            DENTY_ASSERT(false, "Field type must be vector3I! (in ScriptField::GetFieldValueVector3I)");

        return _fieldValues.valueVector3I;
    }

    const Vector4I& ScriptField::GetFieldValueVector4I() const
    {
        if (_fieldType != ScriptFieldType::Vector4I)
            DENTY_ASSERT(false, "Field type must be vector4I! (in ScriptField::GetFieldValueVector4I)");

        return _fieldValues.valueVector4I;
    }

    ScriptFieldType ScriptField::GetScriptFieldType() const
    {
        return _fieldType;
    }

    ScriptFieldType ScriptField::ScriptFieldTypeFromMonoType(MonoType* monoType)
    {
        const std::string typeName = mono_type_get_name(monoType);

        const auto& it = SCRIPT_FIELD_TYPES.find(typeName);
        if (it == SCRIPT_FIELD_TYPES.end())
        {
            DENTY_SET_ERR_COLOR
            DENTY_ERR_LOG << "Unknown type";

            return ScriptFieldType::None;
        }

        return it->second;
    }

    void ScriptField::OnDeserialized()
    {
        MonoClass* monoClass = mono_class_from_name(ScriptEngine::GetAppAssemblyImage(), "DentyEngine", _className.CStr());

        _monoClassField = mono_class_get_field_from_name(monoClass, _name.CStr());
    }
}
