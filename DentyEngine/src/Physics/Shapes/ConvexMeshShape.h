#pragma once

#include "IShape.h"
#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
	class CollisionMesh;

	class ConvexMeshShape final : public IShape
	{
	public:
		ConvexMeshShape();
		ConvexMeshShape(const ConvexMeshShape& source);
		~ConvexMeshShape() override = default;

		void Update(const Vector3& size) override;

		void Reset();

		void OnGui() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		//
		// Event function.
		//
		void OnMeshAssetReceived(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);

		//
		// Getter
		//
		[[nodiscard]]
		Ref<btCollisionShape> GetBtCollisionShape() const override { return _shape; }

		[[nodiscard]]
		const char* GetShapeType() const override { return "ConvexMesh"; }

		[[nodiscard]]
		const Vector3& GetSize() const { return _size; }

		[[nodiscard]]
		bool HasCreated() const { return _created; }
	private:
		void Create(Ref<CollisionMesh> collisionMesh);

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(_size),
				CEREAL_NVP(_collisionMeshFilePath)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(_size),
				CEREAL_NVP(_collisionMeshFilePath)
			);

			OnDeserialized();
		}
	private:
		Ref<CollisionMesh> _collisionMesh;

		// Serialize instead of _collisionMesh.
		FilePath _collisionMeshFilePath;

		Ref<btConvexHullShape> _shape;
		Ref<btTriangleMesh> _triangleMesh;

		// Read only.
		Vector3 _size;

		bool _created;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::ConvexMeshShape, 1u)