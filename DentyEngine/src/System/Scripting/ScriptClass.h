#pragma once

#include "System/Core/DString.h"

#include "ScriptField.h"
#include "ScriptMethod.h"

namespace DentyEngine
{
    class Script;

    class ScriptClass final
    {
        friend class ScriptFieldModifier;
    public:
        ScriptClass();
        ~ScriptClass();

        void Create(const String& className, const String& classNamespace, bool isCore);

        MonoObject* InvokeMethod(const String& methodName, void** params, MonoRuntimeException& e);

        //
        // Getter
        //
        [[nodiscard]]
        const String& GetName() const { return _className; }

        [[nodiscard]]
        const String& GetClassNamespace() const { return _classNamespace; }

        [[nodiscard]]
        const std::map<std::string, ScriptField>& GetFields() const { return _fields; }

        [[nodiscard]]
        const std::map<std::string, ScriptMethod>& GetMethods() const { return _methods; }

        [[nodiscard]]
        MonoObject* GetInstance() const;

    	//
        // Static function.
        //
    public:
        static MonoObject* Instantiate(MonoClass* monoClass);
        static MonoObject* Instantiate(const String& className, bool isCore);

        static MonoClass* GetClassFromImage(const String& className, bool isCore);
    private:
        void CreateFields(bool isCore);
        void CreateMethods();

        // Call on ScriptFieldModifier::EnsureConsistency function.
        void UpdateValue(const Ref<Script>& script);
    private:
        String _className;
        String _classNamespace;

        std::map<std::string, ScriptField> _fields;
        std::map<std::string, ScriptMethod> _methods;

        MonoClass* _class;

        // Hold instead of MonoObject pointer.
        uint32_t _gcHandle;
    };

    class ComponentScriptType final
    {
    public:
        enum class ID : int32_t
        {
	        //
            // Graphics
            //
            None = -1,
            DirectionalLight = 0,
            PointLight,
            SpotLight,

            MeshRenderer,
            SkinnedMeshRenderer,

            //
            // Physics
            //
            BoxCollider,
            SphereCollider,
            CapsuleCollider,
            MeshCollider,
            ConvexMeshCollider,
            ApproximationMeshCollider,
            RigidBody,
            DynamicCharacterController,

            //
            // Animation
            //
            Animator
        };
    public:
        ComponentScriptType() = default;
        ~ComponentScriptType() = default;

        [[nodiscard]]
        static ID FindComponentScriptID(std::string_view name);
    };
}
