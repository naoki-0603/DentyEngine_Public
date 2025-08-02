#pragma once

namespace DentyEngine
{
	class Prefab;

	class PrefabSerializer final
	{
	public:
		PrefabSerializer() = default;
		~PrefabSerializer() noexcept = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(PrefabSerializer)

		[[nodiscard]]
		static bool SerializeAsJson(const std::filesystem::path&, const Ref<const Prefab>&);

		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<Prefab>&);
	};
}