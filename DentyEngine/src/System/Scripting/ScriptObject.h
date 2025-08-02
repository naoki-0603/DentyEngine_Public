#pragma once

#include "System/Core/ID/EntityID.h"

#include "ScriptRuntimeException.h"

namespace DentyEngine
{
	class ScriptClass;

	class ScriptObject final
	{
	public:
		ScriptObject();
		~ScriptObject() = default;

		void Create(Ref<ScriptClass> scriptClass, EntityID id);

		void InvokeOnCreated();
		void InvokeOnUpdate();
		void InvokeOnFixedUpdate();
		void InvokeOnLateUpdate();
		void InvokeOnEnabled();
		void InvokeOnDisabled();
		void InvokeOnDestroy();
		void InvokeOnGui();

		void InvokeOnCollisionEnter(EntityID collisionObjectID);
		void InvokeOnCollisionStay(EntityID collisionObjectID);
		void InvokeOnCollisionExit(EntityID collisionObjectID);

		void InvokeOnTriggerEnter(EntityID collisionObjectID);
		void InvokeOnTriggerStay(EntityID collisionObjectID);
		void InvokeOnTriggerExit(EntityID collisionObjectID);

		void InvokeOnRuntimeSceneChange();
		void InvokeOnRuntimeSceneChanged(std::string_view sceneName);
		void InvokeOnChangeToEditState();
		void InvokeOnChangeToPlayState();
	private:
		//void InvokeConstructor(EntityID id);

		void RuntimeExceptionOccurred(MonoRuntimeException e);
		
		[[nodiscard]]
		bool CheckEntityIsValid() const;
	private:
		Ref<ScriptClass> _scriptClass;

		EntityID _ownerID;
	};
}
