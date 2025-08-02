#pragma once

namespace DentyEngine
{
	namespace MeshResource
	{
		class Skeleton;
	}

	class SkeletonSerializer final
	{
	public:
		SkeletonSerializer();
		~SkeletonSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SkeletonSerializer)

		[[nodiscard]]
		static bool SerializeAsJson(const std::filesystem::path&, const Ref<const MeshResource::Skeleton>&);

		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<MeshResource::Skeleton>&);
	};
}