#pragma once

namespace DentyEngine
{
	class AnimatorController;

	class AnimatorControllerSerializer final
	{
	public:
		AnimatorControllerSerializer();
		~AnimatorControllerSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(AnimatorControllerSerializer)

		[[nodiscard]]
		static bool SerializeAsJson(const std::filesystem::path&, const Ref<const AnimatorController>&);

		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<AnimatorController>&);
	};
}