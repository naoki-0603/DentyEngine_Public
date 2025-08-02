#include "stdafx.h"

#include "ScriptComponent.h"

#include "Scene/GameObject.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Resource/Script.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ScriptLibrary.h"

#include "System/Scripting/ScriptObject.h"

#include "System/Events/EventSystem.h"
#include "System/Layer/ApplicationLayer.h"

#include "System/Scripting/ScriptClass.h"
#include "System/Scripting/ScriptEngine.h"

namespace DentyEngine
{
    ScriptComponent::ScriptComponent() :
        MonoBehaviour(), _script(), _scriptFilePathToSerialized(), _scriptObject(), _scriptClass(), _invokedOnCreated()
    {
        _name = "ScriptComponent";
        _multipleAllowed = true;
    }

    ScriptComponent::ScriptComponent(const ScriptComponent& source) :
        MonoBehaviour(source), _scriptFilePathToSerialized(source._scriptFilePathToSerialized)
    {
        _invokedOnCreated = false;

       /* if (source._scriptFilePathToSerialized.IsValid())
        {
            const ScriptLibrary* scriptLibrary = LibraryManager::GetInstance().GetScriptLibrary();

            const std::optional<Ref<Script>> script = scriptLibrary->Find(_scriptFilePathToSerialized);
            if (not script.has_value())
            {
                DENTY_ASSERT(false, "Script not found! (in ScriptComponent::ScriptComponent)");
            }

            SetScript(script.value());

            Create();
        }*/
    }

    void ScriptComponent::Awake()
    {
        MonoBehaviour::Awake();
    }

    void ScriptComponent::Start()
    {
        MonoBehaviour::Start();

        // Add to script engine.
        ScriptEngine::Add(shared_from_this());
    }

    void ScriptComponent::OnUpdate(float deltaTime)
    {
        MonoBehaviour::OnUpdate(deltaTime);
    }

    void ScriptComponent::OnFixedUpdate()
    {
        MonoBehaviour::OnFixedUpdate();
    }

    void ScriptComponent::OnLateUpdate(float deltaTime)
    {
        MonoBehaviour::OnLateUpdate(deltaTime);
    }

    void ScriptComponent::OnEnabled()
    {
        MonoBehaviour::OnEnabled();
    }

    void ScriptComponent::OnDisable()
    {
        MonoBehaviour::OnDisable();
    }

    void ScriptComponent::OnDestroy()
    {
        // Remove from script engine.
        ScriptEngine::Remove(shared_from_this());
    }

    void ScriptComponent::OnEvent(Event* e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<EventBeforeScriptAssemblyReload>(BIND_EVENT_FUNCTION(OnBeforeScriptAssemblyReload));
        dispatcher.Dispatch<EventScriptAssemblyReloaded>(BIND_EVENT_FUNCTION(OnScriptAssemblyReloaded));
    }

    void ScriptComponent::OnGui()
    {
        MonoBehaviour::OnGui();
             
        ReceiveScriptGui();
    }

    void ScriptComponent::OnPrefabGui()
    {
        MonoBehaviour::OnPrefabGui();
    }

    void ScriptComponent::OnSceneActivated()
    {
        ScriptEngine::Add(shared_from_this());
    }

    void ScriptComponent::OnSceneDeActivated()
    {

    }

    void ScriptComponent::OnRuntimeSceneChange()
    {
        InvokeOnRuntimeSceneChange();
    }

    void ScriptComponent::OnRuntimeSceneChanged(std::string_view sceneName)
    {
        InvokeOnRuntimeSceneChanged(sceneName);
    }

    void ScriptComponent::OnSerialized() const
    {
        MonoBehaviour::OnSerialized();
    }

    void ScriptComponent::OnDeserialized()
    {
        MonoBehaviour::OnDeserialized();
    }

    void ScriptComponent::OnGameObjectDeserialized()
    {
        MonoBehaviour::OnGameObjectDeserialized();

        //if (_scriptFilePathToSerialized.IsValid())
        //{
        //    const ScriptLibrary* scriptLibrary = LibraryManager::GetInstance().GetScriptLibrary();

        //    const std::optional<Ref<Script>> script = scriptLibrary->Find(_scriptFilePathToSerialized);
        //    if (not script.has_value())
        //    {
        //        // Error
        //        DENTY_ASSERT(false, "Script not found! (in ScriptComponent::OnDeserialized)");
        //    }

        //    SetScript(script.value());

        //    // Create script
        //    {
        //        _scriptClass = std::make_shared<ScriptClass>();
        //        _scriptClass->Create(_script->GetName(), "DentyEngine", false);

        //        _scriptObject = std::make_shared<ScriptObject>();
        //        _scriptObject->Create(_scriptClass, GetParent()->GetEntityID());

        //        // Update fields.
        //        ScriptFieldModifier::EnsureConsistency(_script, _scriptClass);

        //        // Invoke if active scene exists.
        //        if (ScriptEngine::GetActiveScene())
        //        {
        //            _scriptObject->InvokeOnCreated();
        //            _invokedOnCreated = true;
        //        }
        //    }
        //}
    }

    Ref<Object> ScriptComponent::MakeCopy() const
    {
        return std::make_shared<ScriptComponent>(*shared_from_this());
    }

    void ScriptComponent::OnBeforeSave()
    {
        //// Get fields from C# script side.
        //std::map<std::string, ScriptField> scriptFields = _scriptClass->GetFields();
        //{
        //    for (auto& field : scriptFields | std::views::values)
        //    {
        //        field.GetValues(_scriptClass->GetInstance());
        //    }
        //}

        //// Update script asset fields.
        //{
        //    ScriptLibrary* scriptLibrary = LibraryManager::GetInstance().GetScriptLibrary();
        //    scriptLibrary->ReloadAt(_scriptFilePathToSerialized, scriptFields);
        //}

        //// Update C# script fields.
        //{
        //    ScriptFieldModifier::UpdateScriptClassFields(_script, _scriptClass);
        //}
    }

    void ScriptComponent::OnRecreate()
    {
        _scriptClass.reset();
        _scriptObject.reset();

        // Find script from library.
        const ScriptLibrary* scriptLibrary = LibraryManager::GetInstance().GetScriptLibrary();

    	const std::optional<Ref<Script>> script = scriptLibrary->Find(_scriptFilePathToSerialized);
        if (not script.has_value())
        {
            DENTY_ERR_CONSOLE_LOG("Script not found! (FilePath: %s)", _scriptFilePathToSerialized.GetAsString().c_str());

        	return;
        }

        _script = script.value();

        // ReCreate script class.
        _scriptClass = std::make_shared<ScriptClass>();
        _scriptClass->Create(_script->GetName(), "DentyEngine", false);

        // ReCreate script object.
        _scriptObject = std::make_shared<ScriptObject>();
        _scriptObject->Create(_scriptClass, GetParent()->GetEntityID());
    }

    void ScriptComponent::InvokeOnCreated()
    {
        _scriptObject->InvokeOnCreated();

        _invokedOnCreated = true;
    }

    void ScriptComponent::InvokeOnUpdate()
    {
        _scriptObject->InvokeOnUpdate();
    }

    void ScriptComponent::InvokeOnFixedUpdate()
    {
        _scriptObject->InvokeOnFixedUpdate();
    }

    void ScriptComponent::InvokeOnLateUpdate()
    {
        _scriptObject->InvokeOnLateUpdate();
    }

    void ScriptComponent::InvokeOnEnabled()
    {
        _scriptObject->InvokeOnEnabled();
    }

    void ScriptComponent::InvokeOnDisabled()
    {
        _scriptObject->InvokeOnDisabled();
    }

    void ScriptComponent::InvokeOnDestroy()
    {
        if (ApplicationLayer::IsPlay())
        {
            _scriptObject->InvokeOnDestroy();
        }
    }

    void ScriptComponent::InvokeOnGui()
    {
        _scriptObject->InvokeOnGui();
    }

    void ScriptComponent::InvokeOnCollisionEnter(EntityID collisionObjectID)
    {
        _scriptObject->InvokeOnCollisionEnter(collisionObjectID);
    }

    void ScriptComponent::InvokeOnCollisionStay(EntityID collisionObjectID)
    {
        _scriptObject->InvokeOnCollisionStay(collisionObjectID);
    }

    void ScriptComponent::InvokeOnCollisionExit(EntityID collisionObjectID)
    {
        _scriptObject->InvokeOnCollisionExit(collisionObjectID);
    }

    void ScriptComponent::InvokeOnTriggerEnter(EntityID collisionObjectID)
    {
        _scriptObject->InvokeOnTriggerEnter(collisionObjectID); 
    }

    void ScriptComponent::InvokeOnTriggerStay(EntityID collisionObjectID)
    {
        _scriptObject->InvokeOnTriggerStay(collisionObjectID);
    }

    void ScriptComponent::InvokeOnTriggerExit(EntityID collisionObjectID)
    {
        _scriptObject->InvokeOnTriggerExit(collisionObjectID);
    }

    void ScriptComponent::InvokeOnRuntimeSceneChange()
    {
        _scriptObject->InvokeOnRuntimeSceneChange();
    }

    void ScriptComponent::InvokeOnRuntimeSceneChanged(std::string_view sceneName)
    {
        _scriptObject->InvokeOnRuntimeSceneChanged(sceneName);
    }

    void ScriptComponent::InvokeOnChangeToEditState()
    {
        _scriptObject->InvokeOnChangeToEditState();
    }

    void ScriptComponent::InvokeOnChangeToPlayState()
    {
        _scriptObject->InvokeOnChangeToPlayState();
    }

    void ScriptComponent::SetScript(Ref<Script> script)
    {
        _script = script;
        _scriptFilePathToSerialized = script->GetFilePathToSerialized();
    }

    void ScriptComponent::ReceiveScriptGui()
    {
        if (ImGui::CollapsingHeader("ScriptInfo##ScriptComponent", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (_script)
            {
                _script->OnGui();
            }
            else
            {
                ImGui::Text("Script is not set");
            }
        }

        // Receive drag and drop.
        if (ImGui::CollapsingHeader("DragAndDrop##ScriptComponent", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("DragAndDropHere!");

            ImVec2 size = ImGui::GetContentRegionAvail();
            size.y = 32.0f;
            ImGui::InvisibleButton("##ScriptDragAndDropArea", size);

            const DragAndDropSpecification specification =
            {
                "ScriptItem", sizeof(Utils::DragAndDropScriptData),
                nullptr, DragAndDropDataType::Script,
                DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(ScriptAssetDragAndDropTargetCallback),
                DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
            };
            DragAndDropTarget dragAndDropTarget(specification);
        }
    }

    void ScriptComponent::ScriptAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
    {
        DENTY_ASSERT(dataType == DragAndDropDataType::Script, "Data type must be script!");

        const Utils::DragAndDropScriptData scriptData = *(static_cast<Utils::DragAndDropScriptData*>(data));

        const Ref<Script> script = scriptData.Get();

        _script = script;
        _scriptFilePathToSerialized = script->GetFilePathToSerialized();

        // Script Engine
        ScriptEngine::OnReceivedScript(shared_from_this());
    }


    void ScriptComponent::OnBeforeScriptAssemblyReload([[maybe_unused]] EventBeforeScriptAssemblyReload* e)
    {
        //// Get fields from C# script side.
        //std::map<std::string, ScriptField> scriptFields = _scriptClass->GetFields();
        //{
        //    for (auto& field : scriptFields | std::views::values)
        //    {
        //        field.GetValues(_scriptClass->GetInstance());
        //    }
        //}

        //// Update script asset fields.
        //{
        //    ScriptLibrary* scriptLibrary = LibraryManager::GetInstance().GetScriptLibrary();
        //    scriptLibrary->ReloadAt(_scriptFilePathToSerialized, scriptFields);
        //}

        //// Update C# script fields.
        //{
        //    ScriptFieldModifier::UpdateScriptClassFields(_script, _scriptClass);
        //}
    }

    void ScriptComponent::Create()
    {
        _scriptClass.reset();
        _scriptObject.reset();

        // ReCreate script class.
        _scriptClass = std::make_shared<ScriptClass>();
        _scriptClass->Create(_script->GetName(), "DentyEngine", false);

        // ReCreate script object.
        _scriptObject = std::make_shared<ScriptObject>();
        _scriptObject->Create(_scriptClass, GetParent()->GetEntityID());
    }

    void ScriptComponent::OnScriptAssemblyReloaded([[maybe_unused]] EventScriptAssemblyReloaded* e)
    {
     //   _scriptClass.reset();
     //   _scriptObject.reset();

    	//// ReCreate script class.
     //   _scriptClass = std::make_shared<ScriptClass>();
     //   _scriptClass->Create(_script->GetName(), "DentyEngine", false);

     //   // ReCreate script object.
     //   _scriptObject = std::make_shared<ScriptObject>();
     //   _scriptObject->Create(_scriptClass, GetParent()->GetEntityID());
     //   _scriptObject->InvokeOnCreated();
     //   _invokedOnCreated = true;
    }
}
