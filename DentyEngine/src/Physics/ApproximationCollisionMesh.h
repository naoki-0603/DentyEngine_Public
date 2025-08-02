#pragma once

#include "System/Core/Object.h"

#include "Graphics/Vertex.h"
#include "Graphics/Mesh/Mesh.h"

#include "System/Resource/Asset.h"

#include "System/Physics/PhysicsData.h"

namespace DentyEngine
{
	class ApproximationCollisionMesh final : public Mesh, public std::enable_shared_from_this<ApproximationCollisionMesh>
	{
	public:
		ApproximationCollisionMesh();
		ApproximationCollisionMesh(const ApproximationCollisionMesh& source);
		~ApproximationCollisionMesh() override = default;

		void Create(const std::vector<MeshResource::CollisionMeshVertex>& vertices, ShapeType shapeType, ApproximationCollisionMeshBulletObjectType bulletObjectType, bool root = false);

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Rename(std::string_view newName, bool deleteOldFile) override;

		void OnReload() override;

		void Clone(const Ref<Asset>& asset) override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void UpdateBulletTransform(const Vector3& offset, const Vector3& rotation, const Vector3& scaling);

		//
		// Getter
		//
		[[nodiscard]]
		AssetType GetAssetType() const override { return AssetType::ApproximationCollisionMesh; }

		[[nodiscard]]
		btCollisionObject* GetBulletObject() const;

		[[nodiscard]]
		Ref<btCollisionShape> GetCollisionShape() const;

		[[nodiscard]]
		bool IsRoot() const { return _root; }
	public:
		static void LoadAssetTexture();

		static Ref<Texture2D> GetAssetTexture() { return _assetTexture; }
	private:
		void CopyOf(const Mesh* source) override;

		void CreateBulletPhysicsObjects();

		void CreateBulletRigidBody(btCollisionShape* collisionShape);
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
			    archive(
				    cereal::base_class<Mesh>(this),
				    CEREAL_NVP(_vertices),
				    CEREAL_NVP(_shapeType),
				    CEREAL_NVP(_bulletObjectType),
				    CEREAL_NVP(_root)
			    );
			}
			
			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
				    cereal::base_class<Mesh>(this),
				    CEREAL_NVP(_vertices),
				    CEREAL_NVP(_shapeType),
				    CEREAL_NVP(_bulletObjectType),
				    CEREAL_NVP(_root)
			    );
			}

			OnDeserialized();
		}
	private:
		// Bullet physics.

		// Doesn't serialize.
		Ref<btGhostObject> _ghostObject;
		Ref<btRigidBody> _rigidBody;
		Ref<btDefaultMotionState> _motionState;

		// Doesn't serialize.
		Ref<btTriangleMesh> _triangleMesh;
		Ref<btBvhTriangleMeshShape> _bvhTriangleMeshShape;
		Ref<btConvexHullShape> _convexHullShape;

		std::vector<MeshResource::CollisionMeshVertex> _vertices;
		ShapeType _shapeType;
		ApproximationCollisionMeshBulletObjectType _bulletObjectType;

		bool _root;
	private:
		static Ref<Texture2D> _assetTexture;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::ApproximationCollisionMesh, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::ApproximationCollisionMesh)