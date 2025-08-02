#include "stdafx.h"

#include "ScriptConsistencyChecker.h"

#include "ScriptClass.h"

#include "System/Resource/Script.h"

namespace DentyEngine
{
	ScriptConsistencyChecker::ScriptConsistencyChecker()
	{
	}

	ScriptConsistencyCheckResult ScriptConsistencyChecker::Check(const Ref<Script>& script, const Ref<ScriptClass>& scriptClass)
	{
		const std::map<std::string, ScriptField>& scriptFieldsMap = script->GetFields();
		const std::map<std::string, ScriptField>& scriptClassFieldsMap = scriptClass->GetFields();

		// Convert to vector.
		std::vector<ScriptField> scriptFields = { };
		for (const auto& scriptField : scriptFieldsMap | std::views::values)
		{
			scriptFields.emplace_back(scriptField);
		}

		std::vector<ScriptField> scriptClassFields = { };
		for (const auto& scriptField : scriptClassFieldsMap | std::views::values)
		{
			scriptClassFields.emplace_back(scriptField);
		}

		return CheckFields(scriptFields, scriptClassFields);
	}

	ScriptConsistencyCheckResult ScriptConsistencyChecker::CheckFields(
		const std::vector<ScriptField>& scriptFields,
		const std::vector<ScriptField>& scriptClassFields
	)
	{
		// Check size.
		{
			if (scriptFields.size() != scriptClassFields.size())
				return ScriptConsistencyCheckResult::DifferentSize;
		}

		// Check type and name.
		{
			for (uint32_t index = 0u; index < scriptFields.size(); ++index)
			{
				// Check name.
				if (scriptFields.at(index).GetName().Get() != scriptClassFields.at(index).GetName().Get())
					return ScriptConsistencyCheckResult::DifferentName;

				// Check type.
				if (scriptFields.at(index).GetScriptFieldType() != scriptClassFields.at(index).GetScriptFieldType())
					return ScriptConsistencyCheckResult::DifferentType;
			}
		}

		return ScriptConsistencyCheckResult::Same;
	}
}
