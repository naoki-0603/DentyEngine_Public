#pragma once

#include "Component/ComponentGenerator.h"

#include "Component/MonoBehavior.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

#include "System/Core/DFilePath.h"
#include "System/Core/ID/EntityID.h"
#include "System/Scripting/ScriptField.h"

namespace DentyEngine
{
	class EventBeforeScriptAssemblyReload;
    class EventScriptAssemblyReloaded;

    class ScriptClass;
    class ScriptObject;

	class ScriptComponent final : public MonoBehavior, public std::enable_shared_from_this<ScriptComponent>
	{
	public:
		ScriptComponent();
		ScriptComponent(const ScriptComponent& source);
		~ScriptComponent() override = default;

		void Awake() override;
		void Start() override;

		void OnUpdate(float deltaTime) override;
		void OnFixedUpdate() override;
		void OnLateUpdate(float deltaTime) override;

		void OnEnabled() override;
		void OnDisable() override;

		void OnDestroy() override;

		void OnEvent(Event* e) override;
		void OnGui() override;
		void OnPrefabGui() override;

		void OnSceneActivated();
		void OnSceneDeActivated();
		void OnRuntimeSceneChange();
		void OnRuntimeSceneChanged(std::string_view sceneName);

		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnGameObjectDeserialized() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		void OnBeforeSave() override;

		void OnRecreate();

		void Create();

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

		//
		// Setter
		//
		void SetScript(Ref<Script> script);

		//
		// Getter
		//
		[[nodiscard]]
		bool IsValid() const { return (_scriptClass && _scriptObject); }

		[[nodiscard]]
		bool InvokedOnCreated() const { return _invokedOnCreated; }

		DENTY_REGISTER_BUILD_IN_COMPONENT(ScriptComponent)
	private:
		void ReceiveScriptGui();

		// Drag and drop target callback function.
		void ScriptAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);


		void OnBeforeScriptAssemblyReload(EventBeforeScriptAssemblyReload* e);
		void OnScriptAssemblyReloaded(EventScriptAssemblyReloaded* e);
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<MonoBehavior>(this),
				CEREAL_NVP(_scriptFilePathToSerialized)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<MonoBehavior>(this),
				CEREAL_NVP(_scriptFilePathToSerialized)
			);

			OnDeserialized();
		}
	private:
		Ref<Script> _script;

		// Serialize instead of _script.
		FilePath _scriptFilePathToSerialized;

		Ref<ScriptObject> _scriptObject;
		Ref<ScriptClass> _scriptClass;

		bool _invokedOnCreated;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::ScriptComponent, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::ScriptComponent)