#include "stdafx.h"

#include "ScriptRuntimeExceptionHandler.h"

#include "ScriptEngine.h"

namespace DentyEngine
{
    ScriptRuntimeExceptionHandler::ScriptRuntimeExceptionHandler()
    {
    }

    void ScriptRuntimeExceptionHandler::OnRuntimeExceptionOccurred(const Ref<ScriptComponent>& scriptComponent, MonoRuntimeException e)
    {
        switch (e)
        {
        case MonoRuntimeException::NullReference:
            HandleReferenceNullException(scriptComponent);
            break;
        default:
            // Unknown mono runtime exception.
            break;
        }
    }

    void ScriptRuntimeExceptionHandler::HandleReferenceNullException(const Ref<ScriptComponent>& scriptComponent)
    {
        ScriptEngine::OnHandleReferenceNullException(scriptComponent);
    }
}
