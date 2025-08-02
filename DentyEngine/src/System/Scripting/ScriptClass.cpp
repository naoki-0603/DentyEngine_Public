#include "stdafx.h"

#include "ScriptClass.h"

#include "ScriptEngine.h"

#include <mono/metadata/object.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/reflection.h>

#include "System/Resource/Script.h"

namespace DentyEngine
{
    ScriptClass::ScriptClass() :
        _className(), _classNamespace(), _fields(), _methods(), _class(), _gcHandle()
    {
    }

    ScriptClass::~ScriptClass()
    {
        mono_gchandle_free(_gcHandle);
    }

    void ScriptClass::Create(const String& className, const String& classNamespace, bool isCore)
    {
        _className = className;
        _classNamespace = classNamespace;

        // Get mono class from image.
        {
            _class = mono_class_from_name((isCore ? ScriptEngine::GetCoreAssemblyImage() : ScriptEngine::GetAppAssemblyImage()), classNamespace.CStr(), className.CStr());
            if (not _class)
            {
                DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Class not found! (namespace: " << classNamespace << ", className: " << className << " )";
                DENTY_ASSERT(false, "");
            }
        }

        // Create instance from mono class.
        {
            // Allocate memory.
            MonoObject* instance = mono_object_new(ScriptEngine::GetAppDomain(), _class);
            if (not instance)
            {
                DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Failed to allocate memory of instance!";
                DENTY_ASSERT(false, "");
            }

            // Invoke constructor
            mono_runtime_object_init(instance);

            // Create gcHandle
            _gcHandle = static_cast<uint32_t>(mono_gchandle_new(instance, false));
        }

        // Create fields.
        {
        	CreateFields(isCore);
        }

        // Create methods.
        {
            CreateMethods();
        }
    }

    MonoObject* ScriptClass::InvokeMethod(const String& methodName, void** params, MonoRuntimeException& e)
    {
        auto&& it = _methods.find(methodName.Get());
        if (it == _methods.end())
        {
            DENTY_SET_ERR_COLOR
            DENTY_ERR_LOG << "Method " << methodName << " not found!";

            return nullptr;
        }

        return it->second.Invoke(GetInstance(), params, e);
    }

    MonoObject* ScriptClass::GetInstance() const
    {
        return mono_gchandle_get_target(_gcHandle);
    }

    MonoObject* ScriptClass::Instantiate(MonoClass* monoClass)
    {
        // Allocate memory of instance.
        MonoObject* instance = mono_object_new(ScriptEngine::GetAppDomain(), monoClass);

        // Run constructor.
        mono_runtime_object_init(instance);

        return instance;
    }

    MonoObject* ScriptClass::Instantiate(const String& className, bool isCore)
    {
        MonoClass* monoClass = mono_class_from_name((isCore ? ScriptEngine::GetCoreAssemblyImage() : ScriptEngine::GetAppAssemblyImage()), "DentyEngine", className.CStr());

        // Allocate memory of instance.
        MonoObject* instance = mono_object_new(ScriptEngine::GetAppDomain(), monoClass);

        // Run constructor.
        mono_runtime_object_init(instance);

        return instance;
    }

    MonoClass* ScriptClass::GetClassFromImage(const String& className, bool isCore)
    {
        return mono_class_from_name(
            (isCore ? ScriptEngine::GetCoreAssemblyImage() : ScriptEngine::GetAppAssemblyImage()),
            "DentyEngine", className.CStr()
        );
    }

    void ScriptClass::CreateFields([[maybe_unused]] bool isCore)
    {
        void* iterator = nullptr;

        while (MonoClassField* field = mono_class_get_fields(_class, &iterator))
        {
            bool isHandle = false;

            ScriptField scriptField = {};

            std::string fieldName = mono_field_get_name(field);
            const uint32_t fieldFlags = mono_field_get_flags(field);

            const bool isPublic = (fieldFlags & MONO_FIELD_ATTR_PUBLIC);

            if (isPublic)
            {
                isHandle = true;
            }
        	// Find attributes.
            else
        	{
                MonoClass* parentClass = mono_field_get_parent(field);

                if (not parentClass)
                    continue;

                MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_field(parentClass, field);

            	if (attrInfo)
                {
                    MonoClass* attrClass = mono_class_from_name(
                        ScriptEngine::GetCoreAssemblyImage(),
                        "DentyEngine", "SerializeField"
                    );

                    if (not attrClass)
                        continue;

                    MonoObject* foundAttr = mono_custom_attrs_get_attr(attrInfo, attrClass);
                    mono_custom_attrs_free(attrInfo);

                    if (foundAttr)
                    {
                        isHandle = true;
                    }
                }
            }

            if (not isHandle)
                continue;

            MonoType* type = mono_field_get_type(field);
            const ScriptFieldType scriptFieldType = ScriptField::ScriptFieldTypeFromMonoType(type);

            // Create script field.
            scriptField.Create(fieldName, _className, scriptFieldType, field, GetInstance());

            _fields.try_emplace(fieldName, scriptField);
        }
    }

    void ScriptClass::CreateMethods()
    {
        void* iterator = nullptr;

        while (MonoMethod* method = mono_class_get_methods(_class, &iterator))
        {
            ScriptMethod scriptMethod = {};

            std::string methodName = mono_method_get_name(method);
            scriptMethod.Create(methodName, method);

            _methods.try_emplace(methodName, scriptMethod);
        }
    }

    void ScriptClass::UpdateValue(const Ref<Script>& script)
    {
        _fields.clear();
        for (const auto& field : script->GetFields())
        {
            _fields.try_emplace(field.first, field.second);
        }

        for (auto& field : _fields | std::views::values)
        {
            field.SetValues(GetInstance());
        }
    }

    ComponentScriptType::ID ComponentScriptType::FindComponentScriptID(std::string_view name)
    {
        const auto componentName = std::string(name);

        // Lights
    	if (componentName == std::string(NAMEOF_ENUM(ID::DirectionalLight)).c_str()) return ID::DirectionalLight;
        if (componentName == std::string(NAMEOF_ENUM(ID::PointLight)).c_str()) return ID::PointLight;
        if (componentName == std::string(NAMEOF_ENUM(ID::SpotLight)).c_str()) return ID::SpotLight;

        // Renderers
        if (componentName == std::string(NAMEOF_ENUM(ID::MeshRenderer)).c_str()) return ID::MeshRenderer;
        if (componentName == std::string(NAMEOF_ENUM(ID::SkinnedMeshRenderer)).c_str()) return ID::SkinnedMeshRenderer;

        // Physics
        if (componentName == std::string(NAMEOF_ENUM(ID::BoxCollider)).c_str()) return ID::BoxCollider;
        if (componentName == std::string(NAMEOF_ENUM(ID::SphereCollider)).c_str()) return ID::SphereCollider;
        if (componentName == std::string(NAMEOF_ENUM(ID::CapsuleCollider)).c_str()) return ID::CapsuleCollider;
        if (componentName == std::string(NAMEOF_ENUM(ID::MeshCollider)).c_str()) return ID::MeshCollider;
        if (componentName == std::string(NAMEOF_ENUM(ID::ConvexMeshCollider)).c_str()) return ID::ConvexMeshCollider;
        if (componentName == std::string(NAMEOF_ENUM(ID::ApproximationMeshCollider)).c_str()) return ID::ApproximationMeshCollider;
        if (componentName == std::string(NAMEOF_ENUM(ID::RigidBody)).c_str()) return ID::RigidBody;
        if (componentName == std::string(NAMEOF_ENUM(ID::DynamicCharacterController)).c_str()) return ID::DynamicCharacterController;

        // Animation
        if (componentName == std::string(NAMEOF_ENUM(ID::Animator)).c_str()) return ID::Animator;

        // Script

        // Return always none.
        if (componentName == std::string("ScriptComponent").c_str()) return ID::None;

        DENTY_SET_ERR_COLOR
        DENTY_ERR_LOG << "Unknown component name.";

        return ID::None;
    }
}
