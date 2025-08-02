#include "stdafx.h"

#include "ScriptFieldModifier.h"

#include "ScriptConsistencyChecker.h"

#include "ScriptClass.h"

#include "System/Resource/Script.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ScriptLibrary.h"

namespace DentyEngine
{
	ScriptFieldModifier::ScriptFieldModifier()
	{
	}

	void ScriptFieldModifier::EnsureConsistency(Ref<Script>& script, Ref<ScriptClass>& scriptClass)
	{
		DENTY_CHECK_NULL(script)
		DENTY_CHECK_NULL(scriptClass)

		const ScriptConsistencyCheckResult result = ScriptConsistencyChecker::Check(script, scriptClass);

		std::map<std::string, ScriptField> newScriptFields = { };
		switch (result)
		{
		case ScriptConsistencyCheckResult::Same:
			// Nothing to do
			break;
		case ScriptConsistencyCheckResult::DifferentSize:
		case ScriptConsistencyCheckResult::DifferentType:
		case ScriptConsistencyCheckResult::DifferentName:
			newScriptFields = Modify(script, scriptClass);
			break;
		default:
			DENTY_ASSERT(false, "Unknown result type! (in ScriptFieldModifier::EnsureConsistency)");
			break;
		}

		// Update script asset fields.
		{
			ScriptLibrary* scriptLibrary = LibraryManager::GetInstance().GetScriptLibrary();
			scriptLibrary->ReloadAt(script->GetFilePathToSerialized(), newScriptFields);
		}

		// Update c# script side fields.
		{
			scriptClass->UpdateValue(script);
		}
	}

	void ScriptFieldModifier::UpdateScriptClassFields(Ref<Script>& script, Ref<ScriptClass>& scriptClass)
	{
		scriptClass->UpdateValue(script);
	}

	std::map<std::string, ScriptField> ScriptFieldModifier::Modify(Ref<Script>& script, Ref<ScriptClass>& scriptClass)
	{
		const std::map<std::string, ScriptField> scriptFields = script->GetFields();
		const std::map<std::string, ScriptField> scriptClassFields = scriptClass->GetFields();

		std::map<std::string, ScriptField> newScriptFields = {};
		for (const auto& scriptField : scriptClassFields)
		{
			// Is exists script asset side?
			auto&& it = scriptFields.find(scriptField.first);
			if (it != scriptFields.end())
			{
				// Add to new script fields.
				newScriptFields.try_emplace(it->first, it->second);
			}
			else
			{
				// Otherwise add as new field.
				newScriptFields.try_emplace(scriptField.first, scriptField.second);
			}
		}

		return newScriptFields;
	}
}
