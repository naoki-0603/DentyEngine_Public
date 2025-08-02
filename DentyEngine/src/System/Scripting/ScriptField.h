#pragma once

#include "System/Core/DString.h"

#include "Math/VectorI.h"

extern "C"
{
    typedef struct _MonoClassField MonoClassField;
    typedef struct _MonoProperty MonoProperty;
    typedef struct _MonoType MonoType;
    typedef struct _MonoObject MonoObject;
}

namespace DentyEngine
{
    enum class Accessibility : uint8_t
    {
        None = 0x00,
        Private = BIT(0),
        Internal = BIT(1),
        Protected = BIT(2),
        Public = BIT(3),
        Serializable = BIT(4)
    };

    enum class ScriptFieldType
    {
        None = 0,
        Bool, Float, Double,
        SByte, Byte,
        Short, Int, Long,
        UShort, UInt, ULong,
        String,
        Vector2, Vector3, Vector4,
        Quaternion, Matrix,
        Vector2I, Vector3I, Vector4I,
        VehicleMoveStatus,
    };

    namespace Utils
    {
        [[nodiscard]]
        static uint8_t GetFieldAccessibility(MonoClassField* field);

        [[nodiscard]]
        static uint8_t GetPropertyAccessibility(MonoProperty* property);

        [[nodiscard]]
        static const char* ScriptFieldTypeToString(ScriptFieldType fieldType);

        [[nodiscard]]
        static ScriptFieldType ScriptFieldTypeFromString(const String& fieldType);
    }

    struct SerializableScriptFieldValues
    {
    public:
        SerializableScriptFieldValues() :
    		valueBool(), valueFloat(), valueDouble(),
    		valueInt8(), valueInt16(), valueInt32(), valueInt64(),
    		valueUInt8(), valueUInt16(), valueUInt32(), valueUInt64(),
    		valueString(), valueVector2(), valueVector3(), valueVector4(),
    		valueQuaternion(), valueMatrix(),
    		valueVector2I(), valueVector3I(), valueVector4I() {}

    public:
        bool valueBool;

        float valueFloat;
        double valueDouble;

    	int8_t valueInt8;
        int16_t valueInt16;
        int32_t valueInt32;
        int64_t valueInt64;
        uint8_t valueUInt8;
        uint16_t valueUInt16;
        uint32_t valueUInt32;
        uint64_t valueUInt64;

        std::string valueString;

        Vector2 valueVector2;
        Vector3 valueVector3;
        Vector4 valueVector4;

    	Quaternion valueQuaternion;
        Matrix valueMatrix;

        Vector2I valueVector2I;
        Vector3I valueVector3I;
        Vector4I valueVector4I;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(valueBool),
                    CEREAL_NVP(valueFloat),
                    CEREAL_NVP(valueDouble),
                    CEREAL_NVP(valueInt8),
                    CEREAL_NVP(valueInt16),
                    CEREAL_NVP(valueInt32),
                    CEREAL_NVP(valueInt64),
                    CEREAL_NVP(valueUInt8),
                    CEREAL_NVP(valueUInt16),
                    CEREAL_NVP(valueUInt32),
                    CEREAL_NVP(valueUInt64),
                    CEREAL_NVP(valueString),
                    CEREAL_NVP(valueVector2),
                    CEREAL_NVP(valueVector3),
                    CEREAL_NVP(valueVector4),
                    CEREAL_NVP(valueQuaternion),
                    CEREAL_NVP(valueMatrix),
                    CEREAL_NVP(valueVector2I),
                    CEREAL_NVP(valueVector3I),
                    CEREAL_NVP(valueVector4I)
                );
            }
            
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(valueBool),
                    CEREAL_NVP(valueFloat),
                    CEREAL_NVP(valueDouble),
                    CEREAL_NVP(valueInt8),
                    CEREAL_NVP(valueInt16),
                    CEREAL_NVP(valueInt32),
                    CEREAL_NVP(valueInt64),
                    CEREAL_NVP(valueUInt8),
                    CEREAL_NVP(valueUInt16),
                    CEREAL_NVP(valueUInt32),
                    CEREAL_NVP(valueUInt64),
                    CEREAL_NVP(valueString),
                    CEREAL_NVP(valueVector2),
                    CEREAL_NVP(valueVector3),
                    CEREAL_NVP(valueVector4),
                    CEREAL_NVP(valueQuaternion),
                    CEREAL_NVP(valueMatrix),
                    CEREAL_NVP(valueVector2I),
                    CEREAL_NVP(valueVector3I),
                    CEREAL_NVP(valueVector4I)
                );
            }
        }
    };

    class ScriptField final
    {
    public:
        ScriptField();
        ~ScriptField() = default;

        void Create(const String& name, const String& className, ScriptFieldType type, MonoClassField* monoClassField, MonoObject* classInstance);

        void SetValues(MonoObject* object);
        void GetValues(MonoObject* object);

        //
        // Getter
        //

        //
        // C# Build in types.
        //
        [[nodiscard]]
        bool GetFieldValueBool() const;

        [[nodiscard]]
        float GetFieldValueFloat() const;

        [[nodiscard]]
        double GetFieldValueDouble() const;

        [[nodiscard]]
        int8_t GetFieldValueInt8() const;

        [[nodiscard]]
        int16_t GetFieldValueInt16() const;

        [[nodiscard]]
        int32_t GetFieldValueInt32() const;

        [[nodiscard]]
        int64_t GetFieldValueInt64() const;

        [[nodiscard]]
        uint8_t GetFieldValueUInt8() const;

        [[nodiscard]]
        uint16_t GetFieldValueUInt16() const;

        [[nodiscard]]
        uint32_t GetFieldValueUInt32() const;

        [[nodiscard]]
        uint64_t GetFieldValueUInt64() const;

        [[nodiscard]]
        const std::string& GetFieldValueString() const;

        //
        // C# DentyEngine types.
        //
        [[nodiscard]]
        const Vector2& GetFieldValueVector2() const;

        [[nodiscard]]
        const Vector3& GetFieldValueVector3() const;

        [[nodiscard]]
        const Vector4& GetFieldValueVector4() const;

        [[nodiscard]]
        const Quaternion& GetFieldValueQuaternion() const;

        [[nodiscard]]
        const Matrix& GetFieldValueMatrix() const;

        [[nodiscard]]
        const Vector2I& GetFieldValueVector2I() const;

        [[nodiscard]]
        const Vector3I& GetFieldValueVector3I() const;

        [[nodiscard]]
        const Vector4I& GetFieldValueVector4I() const;

        [[nodiscard]]
        const String& GetName() const { return _name; }

        [[nodiscard]]
        ScriptFieldType GetScriptFieldType() const;
    public:
        [[nodiscard]]
        static ScriptFieldType ScriptFieldTypeFromMonoType(MonoType* monoType);

    private:
        void OnDeserialized();

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(_name),
                    CEREAL_NVP(_className),
                    CEREAL_NVP(_fieldType),
                    CEREAL_NVP(_fieldValues)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(_name),
                    CEREAL_NVP(_className),
                    CEREAL_NVP(_fieldType),
                    CEREAL_NVP(_fieldValues)
                );
            }

            OnDeserialized();
        }
    private:
        String _name;
        String _className;
        ScriptFieldType _fieldType;
        SerializableScriptFieldValues _fieldValues;

        MonoClassField* _monoClassField;
    private:
        static const std::map<std::string, ScriptFieldType> SCRIPT_FIELD_TYPES;
    };
}

CEREAL_CLASS_VERSION(DentyEngine::SerializableScriptFieldValues, 1u)
CEREAL_CLASS_VERSION(DentyEngine::ScriptField, 1u)