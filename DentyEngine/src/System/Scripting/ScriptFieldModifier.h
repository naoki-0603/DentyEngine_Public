#pragma once

#include "ScriptField.h"

namespace DentyEngine
{
	class Script;
	class ScriptClass;

	class ScriptFieldModifier final
	{
	public:
		ScriptFieldModifier();
		~ScriptFieldModifier() = default;

		static void EnsureConsistency(Ref<Script>& script, Ref<ScriptClass>& scriptClass);

		static void UpdateScriptClassFields(Ref<Script>& script, Ref<ScriptClass>& scriptClass);
	private:
		[[nodiscard]]
		static std::map<std::string, ScriptField> Modify(Ref<Script>& script, Ref<ScriptClass>& scriptClass);
	};
}