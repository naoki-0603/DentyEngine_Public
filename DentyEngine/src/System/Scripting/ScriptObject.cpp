#include "stdafx.h"

#include "ScriptObject.h"

#include "ScriptClass.h"
#include "ScriptUtils.h"

#include "System/Editor/EditorSceneManager.h"
#include "System/Scene/Scene.h"

#include "Component/Scripts/ScriptComponent.h"

#include "ScriptRuntimeExceptionHandler.h"

#include "ScriptEngine.h"

#include <mono/metadata/object.h>

namespace DentyEngine
{
	ScriptObject::ScriptObject() :
		_scriptClass(), _ownerID()
	{
		_ownerID = 0u;
	}

	void ScriptObject::Create(Ref<ScriptClass> scriptClass, EntityID id)
	{
		_scriptClass = scriptClass;
		_ownerID = id;

		// Initialize.
		// InvokeConstructor(id);
	}

	void ScriptObject::InvokeOnCreated()
    {
        auto e = MonoRuntimeException::None;

		void* params[] =
		{
			&_ownerID
		};

		_scriptClass->InvokeMethod("OnCreated", params, e);

		if (e != MonoRuntimeException::None)
		{
			RuntimeExceptionOccurred(e);
		}
    }

	void ScriptObject::InvokeOnUpdate()
	{
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnUpdate", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
	}

	void ScriptObject::InvokeOnFixedUpdate()
	{
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnFixedUpdate", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
	}

	void ScriptObject::InvokeOnLateUpdate()
	{
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnLateUpdate", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
	}

	void ScriptObject::InvokeOnEnabled()
	{
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnEnabled", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
	}

	void ScriptObject::InvokeOnDisabled()
	{
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnDisabled", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
	}

	void ScriptObject::InvokeOnDestroy()
	{
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnDestroy", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
	}

	void ScriptObject::InvokeOnGui()
	{
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnGui", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
	}

    void ScriptObject::InvokeOnCollisionEnter(EntityID collisionObjectID)
    {
		void* params[] =
		{
			&collisionObjectID
		};

		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnCollisionEnter", params, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
    }

    void ScriptObject::InvokeOnCollisionStay(EntityID collisionObjectID)
    {
		void* params[] =
		{
			&collisionObjectID
		};

		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnCollisionStay", params, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
    }

    void ScriptObject::InvokeOnCollisionExit(EntityID collisionObjectID)
    {
		void* params[] =
		{
			&collisionObjectID
		};

		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnCollisionExit", params, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
    }

    void ScriptObject::InvokeOnTriggerEnter(EntityID collisionObjectID)
    {
		void* params[] =
		{
			&collisionObjectID
		};

		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnTriggerEnter", params, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
    }

    void ScriptObject::InvokeOnTriggerStay(EntityID collisionObjectID)
    {
		void* params[] =
		{
			&collisionObjectID
		};

		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnTriggerStay", params, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
    }

    void ScriptObject::InvokeOnTriggerExit(EntityID collisionObjectID)
    {
		void* params[] =
		{
			&collisionObjectID
		};

		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnTriggerExit", params, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
    }

    void ScriptObject::InvokeOnRuntimeSceneChange()
    {
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnRuntimeSceneChange", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
    }

    void ScriptObject::InvokeOnRuntimeSceneChanged(std::string_view sceneName)
    {
		MonoString* monoString = Utils::CreateMonoString(ScriptEngine::GetAppDomain(), std::string(sceneName).c_str());

        void* params[] =
        {
            monoString
        };

		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnRuntimeSceneChanged", params, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
    }

	void ScriptObject::InvokeOnChangeToEditState()
	{
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnChangeToEditState", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
			RuntimeExceptionOccurred(e);
		}
	}

    void ScriptObject::InvokeOnChangeToPlayState()
    {
		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod("OnChangeToPlayState", nullptr, e);

		if (e != MonoRuntimeException::None)
		{
			RuntimeExceptionOccurred(e);
		}
    }

    /*void ScriptObject::InvokeConstructor(EntityID id)
	{
		void* params[] =
		{
			&id
		};

		auto e = MonoRuntimeException::None;

		_scriptClass->InvokeMethod(".ctor", params, e);

		if (e != MonoRuntimeException::None)
		{
		    RuntimeExceptionOccurred(e);
		}
	}*/

    void ScriptObject::RuntimeExceptionOccurred(MonoRuntimeException e)
    {
		const std::optional<Ref<GameObject>> gameObject = EditorSceneManager::GetActiveScene()->FindByEntityID(_ownerID);
		const auto& scriptComponent = gameObject.value()->GetComponentAsSharedPtr<ScriptComponent>();

		ScriptRuntimeExceptionHandler::OnRuntimeExceptionOccurred(scriptComponent, e);
    }

    bool ScriptObject::CheckEntityIsValid() const
	{
		const std::optional<Ref<GameObject>> gameObject = EditorSceneManager::GetActiveScene()->FindByEntityID(_ownerID);

		return gameObject.has_value();
	}

}
