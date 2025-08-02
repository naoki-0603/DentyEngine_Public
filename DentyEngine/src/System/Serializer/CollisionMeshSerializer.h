#pragma once

namespace DentyEngine
{
	class CollisionMesh;
	class ApproximationCollisionMesh;

	class CollisionMeshSerializer final
	{
	public:
		CollisionMeshSerializer();
		~CollisionMeshSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(CollisionMeshSerializer)

		//
		// Binary.
		//
		[[nodiscard]]
		static bool SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const CollisionMesh>& collisionMesh);

		[[nodiscard]]
		static bool SerializeAsBinary(const std::filesystem::path& filePath, const Ref<const ApproximationCollisionMesh>& approximationCollisionMesh);

		[[nodiscard]]
		static bool DeserializeFromBinary(const std::filesystem::path& filePath, Ref<CollisionMesh>& collisionMesh);

		[[nodiscard]]
		static bool DeserializeFromBinary(const std::filesystem::path& filePath, Ref<ApproximationCollisionMesh>& approximationCollisionMesh);
	};
}