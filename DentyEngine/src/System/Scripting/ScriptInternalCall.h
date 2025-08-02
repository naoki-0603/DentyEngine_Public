#pragma once

namespace DentyEngine
{
    class ScriptInternalCall final
    {
    public:
        ScriptInternalCall();
        ~ScriptInternalCall() = default;

        static void RegisterInternalCalls();
    };

#define DENTY_ADD_INTERNAL_CALL(Function) mono_add_internal_call("DentyEngine.InternalCalls::" #Function, Function)
}
