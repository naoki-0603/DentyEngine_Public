#include "stdafx.h"

#include "ScriptMethod.h"
#include "ScriptUtils.h"

#include <mono/metadata/object.h>
#include <mono/metadata/exception.h>

namespace DentyEngine
{
	ScriptMethod::ScriptMethod() :
		_name(), _method()
	{
	}

	void ScriptMethod::Create(const String& name, MonoMethod* method)
	{
		if (not method)
			DENTY_ASSERT(false, "Method is null! (in ScriptMethod::Create)");

		_name = name;
		_method = method;
	}

	MonoObject* ScriptMethod::Invoke(MonoObject* classInstance, void** params, MonoRuntimeException& type)
	{
		MonoObject* excObject = nullptr;
		MonoObject* returnValue = nullptr;

		// Invoke method with parameters.
		returnValue = mono_runtime_invoke(_method, classInstance, params, &excObject);

		if (excObject)
		{
			MonoObject* toStringExc = nullptr;
			MonoString* monoString = mono_object_to_string(excObject, &toStringExc);
			
			const std::string exception = Utils::MonoStringToUTF8(monoString);

			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Exception: " << exception;

			DENTY_ERR_CONSOLE_LOG("Exception: %s", exception.c_str());

			if (mono_get_exception_null_reference())
			{
				type = MonoRuntimeException::NullReference;
			}

			return nullptr;
		}

		return returnValue;
	}
}
