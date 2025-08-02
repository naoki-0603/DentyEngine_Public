#pragma once

#include "ScriptField.h"

namespace DentyEngine
{
	class Script;
	class ScriptClass;

	enum class ScriptConsistencyCheckResult : int32_t
	{
		None = -1,
		Same = 0,
		DifferentSize,
		DifferentType,
		DifferentName,

		Max
	};

	class ScriptConsistencyChecker final
	{
	public:
		ScriptConsistencyChecker();
		~ScriptConsistencyChecker() = default;

		[[nodiscard]]
		static ScriptConsistencyCheckResult Check(const Ref<Script>& script, const Ref<ScriptClass>& scriptClass);
	private:

		[[nodiscard]]
		static ScriptConsistencyCheckResult CheckFields(
			const std::vector<ScriptField>& scriptFields,
			const std::vector<ScriptField>& scriptClassFields
		);
	};
}