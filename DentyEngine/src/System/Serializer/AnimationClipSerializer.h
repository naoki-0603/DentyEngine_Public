#pragma once

namespace DentyEngine
{
	namespace MeshResource
	{
		class AnimationClip;
	}

	class AnimationClipSerializer final
	{
	public:
		AnimationClipSerializer();
		~AnimationClipSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(AnimationClipSerializer)

		[[nodiscard]]
		static bool SerializeAsBinary(const std::filesystem::path&, const Ref<const MeshResource::AnimationClip>&);

		[[nodiscard]]
		static bool DeserializeFromBinary(const std::filesystem::path&, Ref<MeshResource::AnimationClip>&);
	};
}