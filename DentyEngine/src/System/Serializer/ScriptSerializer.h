#pragma once

namespace DentyEngine
{
	class Script;

	class ScriptSerializer final
	{
	public:
		ScriptSerializer();
		~ScriptSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ScriptSerializer)

		[[nodiscard]]
		static bool SerializeAsBinary(const std::filesystem::path&, const Ref<const Script>&);

		[[nodiscard]]
		static bool DeserializeFromBinary(const std::filesystem::path&, Ref<Script>&);
	};
}