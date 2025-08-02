#include "stdafx.h"

#include "ScriptEngine.h"

#include "ScriptInternalCall.h"
#include "ScriptLoader.h"

#include "Component/Scripts/ScriptComponent.h"

#include "System/Layer/ApplicationLayer.h"

#include "System/Editor/EditorSceneManager.h"

#include "System/Events/EventScene.h"
#include "System/Events/EventSystem.h"

#include "Scene/GameObject.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>

#include <format>

namespace DentyEngine
{
    // data
    ScriptEngineData ScriptEngine::_data = { };

    // Assembly file paths.
#if DENTY_DEBUG
    const FilePath ScriptEngine::CORE_ASSEMBLY_FILE_PATH = "DentyEngine-ScriptCore\\ScriptCore\\bin\\Debug\\ScriptCore.dll";
    const FilePath ScriptEngine::APP_ASSEMBLY_FILE_PATH = "DentyEngine-ScriptApp\\ScriptApp\\bin\\Debug\\ScriptApp.dll";
#else // DENTY_DEBUG
    const FilePath ScriptEngine::CORE_ASSEMBLY_FILE_PATH = "DentyEngine-ScriptCore\\ScriptCore\\bin\\Release\\ScriptCore.dll";
    const FilePath ScriptEngine::APP_ASSEMBLY_FILE_PATH = "DentyEngine-ScriptApp\\ScriptApp\\bin\\Release\\ScriptApp.dll";
#endif // DENTY_RELEASE || DENTY_DIST 
    const FilePath ScriptEngine::SCRIPT_SAVE_FOLDER_PATH = "DentyEngine-ScriptApp\\ScriptApp\\Scripts\\";

    ScriptEngine::ScriptEngine()
    {
    }

    void ScriptEngine::OnAppAssemblyFileSystemEvent([[maybe_unused]] const std::string& filePath, const filewatch::Event changeType)
    {
        if (not _data.requestedReload && changeType == filewatch::Event::modified && ApplicationLayer::IsEdit())
        {
            _data.requestedReload = true;
        }
    }

    void ScriptEngine::OnHandleReferenceNullException([[maybe_unused]] const Ref<ScriptComponent>& script)
    {
        
    }

    void ScriptEngine::GenerateCSharpFile(const char* filePath)
    {
		auto f = std::string(filePath);
		f += ".cs";

        std::ofstream outputStream(f);

		// Using.
		outputStream << "using System;\n";
		outputStream << "using System.Collections.Generic;\n";
		outputStream << "using System.Linq;\n";
		outputStream << "using System.Text;\n";
		outputStream << "using System.Threading.Tasks;\n";

        outputStream << "\n";

		// Main
		outputStream << "namespace DentyEngine\n";
        outputStream << "{";

        outputStream << "\n";

        outputStream << std::format("\tpublic class {} : MonoBehaviour\n", FilePath(filePath).RemoveExtension().Filename().GetAsString());
		outputStream << "\t{\n";

        // Constructor
        outputStream << std::format("\t\tpublic {}()\n", FilePath(filePath).RemoveExtension().Filename().GetAsString());
        outputStream << "\t\t{\n";
        outputStream << "\t\t}\n";
        outputStream << "\n";

		// OnCreated
		outputStream << ("\t\tpublic override void OnCreated(uint entityID)\n");
		outputStream << ("\t\t{\n");
        outputStream << ("\t\t\tbase.OnCreated(entityID);\n");
        outputStream << std::format("\t\t\tAddMonoBehaviour(this, {}{}{}); \n", '"', FilePath(filePath).RemoveExtension().Filename().GetAsString(), '"');
		outputStream << ("\t\t}\n");

		outputStream << ("\n");

		// OnUpdate
		outputStream << ("\t\tpublic override void OnUpdate()\n");
		outputStream << ("\t\t{\n");
		outputStream << ("\t\t}\n");

		outputStream << ("\n");

		// OnFixedUpdate
		outputStream << ("\t\tpublic override void OnFixedUpdate()\n");
		outputStream << ("\t\t{\n");
		outputStream << ("\t\t}\n");

		outputStream << ("\n");

		// OnLateUpdate
		outputStream << ("\t\tpublic override void OnLateUpdate()\n");
		outputStream << ("\t\t{\n");
		outputStream << ("\t\t}\n");

		outputStream << ("\n");

		// OnEnabled
		outputStream << ("\t\tpublic override void OnEnabled()\n");
		outputStream << ("\t\t{\n");
		outputStream << ("\t\t}\n");

		outputStream << ("\n");

		// OnDisabled
		outputStream << ("\t\tpublic override void OnDisabled()\n");
		outputStream << ("\t\t{\n");
		outputStream << ("\t\t}\n");

		outputStream << ("\n");

		// OnGui
		outputStream << ("\t\tpublic override void OnGui()\n");
		outputStream << ("\t\t{\n");
		outputStream << ("\t\t}\n");

		outputStream << ("\n");

		// OnDestroy
		outputStream << ("\t\tpublic override void OnDestroy()\n");
		outputStream << ("\t\t{\n");
		outputStream << ("\t\t}\n");

        outputStream << ("\n");

        // OnCollisionEnter
        outputStream << ("\t\tpublic override void OnCollisionEnter(uint collisionObjectID)\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

        outputStream << ("\n");

        // OnCollisionStay
        outputStream << ("\t\tpublic override void OnCollisionStay(uint collisionObjectID)\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

        outputStream << ("\n");

        // OnCollisionExit
        outputStream << ("\t\tpublic override void OnCollisionExit(uint collisionObjectID)\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

        outputStream << ("\n");

        // OnTriggerEnter
        outputStream << ("\t\tpublic override void OnTriggerEnter(uint collisionObjectID)\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

        outputStream << ("\n");

        // OnTriggerStay
        outputStream << ("\t\tpublic override void OnTriggerStay(uint collisionObjectID)\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

        outputStream << ("\n");

        // OnTriggerExit
        outputStream << ("\t\tpublic override void OnTriggerExit(uint collisionObjectID)\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

        // OnRuntimeSceneChange
        outputStream << ("\t\tpublic override void OnRuntimeSceneChange()\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

        // OnRuntimeSceneChanged
        outputStream << ("\t\tpublic override void OnRuntimeSceneChanged(string sceneName)\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

        // OnChangeToPlayState
        outputStream << ("\t\tpublic override void OnChangeToPlayState()\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

        // OnChangeToEditState
        outputStream << ("\t\tpublic override void OnChangeToEditState()\n");
        outputStream << ("\t\t{\n");
        outputStream << ("\t\t}\n");

		// End
		outputStream << ("\t}\n");
		outputStream << ("}");

		outputStream.close();
    }

    void ScriptEngine::Initialize()
    {
        InitializeMono();
        ScriptInternalCall::RegisterInternalCalls();

        LoadAssemblies();
    }

    void ScriptEngine::OnUpdate()
    {
        for (auto&& script : _data.scripts)
        {
            if (script->InvokedOnCreated() || (not script->IsValid()))
                continue;

            script->InvokeOnCreated();
        }

        if (ApplicationLayer::IsPlay())
        {
            for (auto&& script : _data.scripts)
            {
                if (not script->Enabled())
                    continue;

                script->InvokeOnUpdate();
            }
        }
    }

    void ScriptEngine::OnFixedUpdate()
    {
        if (ApplicationLayer::IsPlay())
        {
            for (auto&& script : _data.scripts)
            {
                if (not script->Enabled())
                    continue;

                script->InvokeOnFixedUpdate();
            }    
        }
        
    }

    void ScriptEngine::OnLateUpdate()
    {
        if (ApplicationLayer::IsPlay())
        {
            for (auto&& script : _data.scripts)
            {
                if (not script->Enabled())
                    continue;

                script->InvokeOnLateUpdate();
            }    
        }
        
    }

    void ScriptEngine::OnEnabled(EventGameObjectEnabled* e)
    {
        if (ApplicationLayer::IsPlay())
        {
            if (e->GetGameObject()->HasComponent<ScriptComponent>())
            {
                const auto& scriptComponent = e->GetGameObject()->GetComponentAsSharedPtr<ScriptComponent>();

                auto&& it = std::ranges::find(_data.scripts, scriptComponent);
                if (it != _data.scripts.end())
                {
                    (*it)->InvokeOnEnabled();
                }
            }
        }
    }

    void ScriptEngine::OnDisabled(EventGameObjectDisabled* e)
    {
        if (ApplicationLayer::IsPlay())
        {
            if (e->GetGameObject()->HasComponent<ScriptComponent>())
            {
                const auto& scriptComponent = e->GetGameObject()->GetComponentAsSharedPtr<ScriptComponent>();

                auto&& it = std::ranges::find(_data.scripts, scriptComponent);
                if (it != _data.scripts.end())
                {
                    (*it)->InvokeOnDisabled();
                }
            }
        }
    }

    void ScriptEngine::OnGui()
    {
        for (auto&& script : _data.scripts)
        {
            if (not script->Enabled() ||
                not script->IsValid() ||
                script->InvokedOnCreated())
            {
                continue;
            }

            script->InvokeOnGui();
        }
    }

    void ScriptEngine::OnRequestedReload()
    {
        ReloadAssembly();
    }

    void ScriptEngine::Finalize()
    {
        FinalizeMono();
    }

    void ScriptEngine::ReloadAssembly()
    {
        EventBeforeScriptAssemblyReload eventBeforeScriptAssemblyReload;
        NOTIFY_EVENT(eventBeforeScriptAssemblyReload);

        mono_domain_set(mono_get_root_domain(), 0);
        mono_domain_unload(_data.appDomain);

        _data.requestedReload = false;
        _data.appAssemblyFileWatcher.reset();

    	LoadAssemblies();

        // Recreate all script components.
        RecreateScripts();

        // Fire event.
        EventScriptAssemblyReloaded eventScriptAssemblyReloaded;
        NOTIFY_EVENT(eventScriptAssemblyReloaded);
    }

    void ScriptEngine::OnEvent(Event* e)
    {
        switch (const EventType type = e->GetEventType())
        {
        case EventType::SceneActivated:
            OnSceneActivated(static_cast<EventOnSceneActivated*>(e));
            break;
        case EventType::SceneDeActivated:
            OnSceneDeActivated(static_cast<EventOnSceneDeActivated*>(e));
            break;
        case EventType::RuntimeSceneChange:
            OnRuntimeSceneChange(static_cast<EventRuntimeSceneChange*>(e));
            break;
        case EventType::RuntimeSceneChanged:
            OnRuntimeSceneChanged(static_cast<EventRuntimeSceneChanged*>(e));
            break;
        case EventType::OnApplicationStateChangedToEdit:
            OnApplicationStateChangedToEdit(static_cast<EventOnApplicationStateChangedToEdit*>(e));
            break;
        case EventType::OnApplicationStateChangedToPlay:
            OnApplicationStateChangedToPlay(static_cast<EventOnApplicationStateChangedToPlay*>(e));
            break;
        case EventType::GameObjectEnabled:
            OnEnabled(static_cast<EventGameObjectEnabled*>(e));
            break;
        case EventType::GameObjectDisabled:
            OnDisabled(static_cast<EventGameObjectDisabled*>(e));
            break;
		default:
            break;
        }
    }

    void ScriptEngine::OnReceivedScript(const Ref<ScriptComponent>& script)
    {
        auto&& it = std::ranges::find(_data.scripts, script);
        if (it != _data.scripts.end())
        {
            (*it)->Create();
            (*it)->InvokeOnCreated();
        }
    }

    void ScriptEngine::OnScenePlayButtonPressed()
    {
        RecreateScripts();
    }

    void ScriptEngine::Add(Ref<ScriptComponent> script)
    {
        _data.scripts.emplace_back(script);
    }

    void ScriptEngine::Remove(const Ref<ScriptComponent>& script)
    {
        auto&& it = std::ranges::find(_data.scripts, script);
        if (it != _data.scripts.end())
        {
            (*it)->InvokeOnDestroy();

            _data.scripts.erase(it);
        }
    }

    void ScriptEngine::InitializeMono()
    {
        mono_set_dirs("./Vendor/bin/MonoAssembly/bin/", "./Vendor/bin/MonoAssembly/etc/");

        // Enable debugging.
        {
            if (_data.enableDebugging)
            {
                const char* args[1] =
                {
				    "--soft-breakpoints"
                };

                mono_jit_parse_options(1, const_cast<char**>(args));
                mono_debug_init(MONO_DEBUG_FORMAT_MONO);
            }
        }

        // Get root domain.
        {
            MonoDomain* rootDomain = mono_jit_init("DentyScriptJITRuntime");
            if (not rootDomain)
            {
                DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Failed to init root domain!";
                DENTY_ASSERT(false, "");
            }

            // Store pointer.
            _data.rootDomain = rootDomain;

            if (_data.enableDebugging)
                mono_debug_domain_create(_data.rootDomain);
        }

        mono_thread_set_main(mono_thread_current());
    }

    void ScriptEngine::FinalizeMono()
    {
        mono_domain_set(mono_get_root_domain(), false);

        mono_domain_unload(_data.appDomain);
        _data.appDomain = nullptr;

        mono_jit_cleanup(_data.rootDomain);
        _data.rootDomain = nullptr;
    }

    void ScriptEngine::LoadAssemblies()
    {
        // Core
        {
            // Create app domain
            _data.appDomain = mono_domain_create_appdomain(const_cast<char*>("DentyScriptRuntime"), nullptr);
            mono_domain_set(_data.appDomain, true);

            bool loadSuccessfully = false;
            _data.coreAssembly = ScriptLoader::LoadCSharpAssembly(CORE_ASSEMBLY_FILE_PATH, loadSuccessfully, true);

            if (not loadSuccessfully)
            {
                DENTY_ASSERT(false, "Failed to load core assembly! (in ScriptEngine::Initialize)");
            }

            // Set file path.
            _data.coreAssemblyFilePath = CORE_ASSEMBLY_FILE_PATH;

            // Get assembly image.
            _data.coreAssemblyImage = mono_assembly_get_image(_data.coreAssembly);
        }

        // App
        {
            bool loadSuccessfully = false;
            _data.appAssembly = ScriptLoader::LoadCSharpAssembly(APP_ASSEMBLY_FILE_PATH, loadSuccessfully, true);

            if (not loadSuccessfully)
            {
                DENTY_ASSERT(false, "Failed to load app assembly! (in ScriptEngine::Initialize)");
            }

            // Set file path.
            _data.appAssemblyFilePath = APP_ASSEMBLY_FILE_PATH;

            // Get assembly image.
            _data.appAssemblyImage = mono_assembly_get_image(_data.appAssembly);

            // Create file watcher.
            _data.appAssemblyFileWatcher = std::make_unique<filewatch::FileWatch<std::string>>(_data.appAssemblyFilePath.GetAsString(), OnAppAssemblyFileSystemEvent);
        }
    }

    void ScriptEngine::RecreateScripts()
    {
        for (auto&& script : _data.scripts)
        {
            script->OnRecreate();
            script->InvokeOnCreated();
        }
    }

    void ScriptEngine::OnSceneActivated([[maybe_unused]] EventOnSceneActivated* e)
    {
        _data.activeScene = EditorSceneManager::GetActiveScene();
    }

    void ScriptEngine::OnSceneDeActivated([[maybe_unused]] EventOnSceneDeActivated* e)
    {
        _data.activeScene = nullptr;
        _data.scripts.clear();
    }

    void ScriptEngine::OnRuntimeSceneChange([[maybe_unused]] EventRuntimeSceneChange* e)
    {
        for (auto&& script : _data.scripts)
        {
            if (not script->Enabled())
                continue;

            script->InvokeOnRuntimeSceneChange();
        }
    }

    void ScriptEngine::OnRuntimeSceneChanged(EventRuntimeSceneChanged* e)
    {
        RecreateScripts();

        for (auto&& script : _data.scripts)
        {
            if (not script->Enabled())
                continue;

            script->InvokeOnRuntimeSceneChanged(e->GetSceneName());
        }
    }

    void ScriptEngine::OnApplicationStateChangedToPlay([[maybe_unused]] EventOnApplicationStateChangedToPlay* e)
    {
        for (auto&& script : _data.scripts)
        {
            if (not script->Enabled())
                continue;

            script->InvokeOnChangeToPlayState();
        }
    }

    void ScriptEngine::OnApplicationStateChangedToEdit([[maybe_unused]] EventOnApplicationStateChangedToEdit* e)
    {
        for (auto&& script : _data.scripts)
        {
            if (not script->Enabled())
                continue;

            script->InvokeOnChangeToEditState();
        }
    }
}
