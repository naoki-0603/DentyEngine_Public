#pragma once

#include "ScriptRuntimeException.h"

namespace DentyEngine
{
    class ScriptComponent;

    class ScriptRuntimeExceptionHandler final
    {
    public:
        ScriptRuntimeExceptionHandler();
        ~ScriptRuntimeExceptionHandler() = default;

        static void OnRuntimeExceptionOccurred(const Ref<ScriptComponent>& scriptComponent, MonoRuntimeException e);
    private:
        static void HandleReferenceNullException(const Ref<ScriptComponent>& scriptComponent);
    };
}