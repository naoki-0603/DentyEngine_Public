#pragma once

namespace DentyEngine
{
	class StaticMesh;
	class SkinnedMesh;

	class MeshSerializer final
	{
	public:
		MeshSerializer();
		~MeshSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(MeshSerializer)

		//
		// Binary.
		//
		[[nodiscard]]
		static bool SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const StaticMesh>& staticMesh);

		[[nodiscard]]
		static bool SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const SkinnedMesh>& skinnedMesh);

		[[nodiscard]]
		static bool DeserializeFromBinary(const std::filesystem::path& filePath, Ref<StaticMesh>& staticMesh);

		[[nodiscard]]
		static bool DeserializeFromBinary(const std::filesystem::path& filePath, Ref<SkinnedMesh>& skinnedMesh);
	};
}
