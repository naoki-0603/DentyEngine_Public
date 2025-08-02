#pragma once

#include <mono/metadata/object-forward.h>

#include "ScriptRuntimeException.h"

#include "System/Core/DString.h"

namespace DentyEngine
{
	class ScriptMethod final
	{
	public:
		ScriptMethod();
		~ScriptMethod() = default;

		void Create(const String& name, MonoMethod* method);

		MonoObject* Invoke(MonoObject* classInstance, void** params, _Out_ MonoRuntimeException& type);
	private:
		String _name;

		MonoMethod* _method;
	};
}
