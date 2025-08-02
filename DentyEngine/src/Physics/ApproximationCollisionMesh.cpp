#include "stdafx.h"

#include "ApproximationCollisionMesh.h"

#include "System/Physics/CollisionShapeGenerator.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "System/Serializer/CollisionMeshSerializer.h"

namespace DentyEngine
{
	Ref<Texture2D> ApproximationCollisionMesh::_assetTexture = nullptr;

	ApproximationCollisionMesh::ApproximationCollisionMesh() :
		Mesh(), _ghostObject(), _rigidBody(), _motionState(),
		_triangleMesh(), _convexHullShape(), _vertices(), _shapeType(ShapeType::None), _bulletObjectType(ApproximationCollisionMeshBulletObjectType::GhostObject), _root()
	{
	}

	ApproximationCollisionMesh::ApproximationCollisionMesh(const ApproximationCollisionMesh& source) 
	{
		this->CopyOf(&source);
	}

	void ApproximationCollisionMesh::Create(const std::vector<MeshResource::CollisionMeshVertex>& vertices, 
		ShapeType shapeType, ApproximationCollisionMeshBulletObjectType bulletObjectType, 
		bool root
	)
	{
		_vertices = vertices;
		_shapeType = shapeType;
		_bulletObjectType = bulletObjectType;
		_root = root;

		if (!_root)
		{
			CreateBulletPhysicsObjects();
		}
	}

	void ApproximationCollisionMesh::Save(bool isUpdate) const
	{
		const auto approximationCollisionMesh = shared_from_this();

		// If failed
		if (!CollisionMeshSerializer::SerializeAsBinary(_filePathToSerialized.Get(), approximationCollisionMesh))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " approximation collision mesh!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " approximation collision mesh!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " approximation collision mesh!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " approximation collision mesh!";
		}
	}

	void ApproximationCollisionMesh::Load(const std::filesystem::path& filePath)
	{
		auto approximationCollisionMesh = std::make_shared<ApproximationCollisionMesh>();

		// If failed
		if (!CollisionMeshSerializer::DeserializeFromBinary(filePath, approximationCollisionMesh))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " approximation collision mesh!";

			return;
		}

		approximationCollisionMesh->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " approximation collision mesh!";
	}

	void ApproximationCollisionMesh::Rename([[maybe_unused]] std::string_view newName, [[maybe_unused]] bool deleteOldFile)
	{

	}

	void ApproximationCollisionMesh::OnReload()
	{

	}

	void ApproximationCollisionMesh::Clone(const Ref<Asset>& asset)
	{
		DENTY_CHECK_CONDITION(asset->GetAssetType() == AssetType::ApproximationCollisionMesh);

		auto&& target = std::static_pointer_cast<ApproximationCollisionMesh>(asset);

		target->CopyOf(this);
	}

	void ApproximationCollisionMesh::OnSerialized() const
	{

	}

	void ApproximationCollisionMesh::OnDeserialized()
	{
		if (!_root)
		{
			if (_shapeType == ShapeType::Mesh || _shapeType == ShapeType::ConvexMesh)
			{
				CreateBulletPhysicsObjects();
			}
		}
	}

    void ApproximationCollisionMesh::UpdateBulletTransform(
		const Vector3& offset, 
		const Vector3& rotation,
        const Vector3& scaling
	)
    {
		if (_root)
			return;

		const Matrix rotationMatrix = 
			DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

		btTransform btTransform = rotationMatrix.ToBtTransform();
		btTransform.setOrigin(offset.ToBtVector3());

		Ref<btCollisionShape> shape = GetCollisionShape();
		if (shape)
		{
		    shape->setLocalScaling(scaling.ToBtVector3());
		}

		btCollisionObject* object = GetBulletObject();
		if (object)
		{
		    object->setWorldTransform(btTransform);
		}
    }

    btCollisionObject* ApproximationCollisionMesh::GetBulletObject() const
	{
		switch (_bulletObjectType)
		{
		case ApproximationCollisionMeshBulletObjectType::GhostObject: return _ghostObject.get();
		case ApproximationCollisionMeshBulletObjectType::RigidBody: return _rigidBody.get();
		}

		return nullptr;
	}

	Ref<btCollisionShape> ApproximationCollisionMesh::GetCollisionShape() const
	{
		switch (_shapeType)
		{
			case ShapeType::Mesh: return _bvhTriangleMeshShape;
			case ShapeType::ConvexMesh: return _convexHullShape;
		}

		return nullptr;
	}

	void ApproximationCollisionMesh::LoadAssetTexture()
	{
		DENTY_ASSERT(not _assetTexture, "Approximation collision mesh asset texture already loaded!");

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		_assetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::APPROXIMATION_COLLISION_MESH_THUMBNAIL_FILE_PATH).value();

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Approximation collision mesh asset texture successfully loaded!";
	}

	void ApproximationCollisionMesh::CopyOf(const Mesh* source)
	{
		Mesh::CopyOf(source);

		const auto sourceApproximationCollisionMesh = static_cast<const ApproximationCollisionMesh*>(source);

		// This class
		{
			_vertices = sourceApproximationCollisionMesh->_vertices;
			_shapeType = sourceApproximationCollisionMesh->_shapeType;
			_bulletObjectType = sourceApproximationCollisionMesh->_bulletObjectType;
			_root = sourceApproximationCollisionMesh->_root;
		}

		// Sub meshes.
		{
			for (int subMeshIndex = 0; subMeshIndex < sourceApproximationCollisionMesh->_subMeshes.size(); ++subMeshIndex)
			{
				const auto subMesh = std::static_pointer_cast<ApproximationCollisionMesh>(sourceApproximationCollisionMesh->_subMeshes.at(subMeshIndex));
				const auto collisionMesh = std::make_shared<ApproximationCollisionMesh>();

				subMesh->Clone(collisionMesh);

				_subMeshes.emplace_back(collisionMesh);
			}
		}

		// Create collision object.
		if (!_root)
		{
			if (_shapeType == ShapeType::Mesh || _shapeType == ShapeType::ConvexMesh)
			{
				CreateBulletPhysicsObjects();
			}
		}
	}

	void ApproximationCollisionMesh::CreateBulletPhysicsObjects()
	{
		switch (_shapeType)
		{
		case ShapeType::Mesh:
		{
			CollisionShapeGenerator::GenerateBvhCollisionShape(
				_vertices, _triangleMesh, _bvhTriangleMeshShape
			);
		}
		break;
		case ShapeType::ConvexMesh:
		{
			CollisionShapeGenerator::GenerateConvexCollisionShape(
				_vertices, _triangleMesh, _convexHullShape
			);
		}
		break;
		default:
			DENTY_ASSERT(false, "Unknown shape type! (in ApproximationCollisionMesh::CreateShape");
			break;
		}

		switch (_bulletObjectType)
		{
		case ApproximationCollisionMeshBulletObjectType::GhostObject:
			_ghostObject = std::make_shared<btGhostObject>();
			break;
		case ApproximationCollisionMeshBulletObjectType::RigidBody:
			{
				if (_shapeType == ShapeType::Mesh)
				{
					CreateBulletRigidBody(_bvhTriangleMeshShape.get());
				}
				else if (_shapeType == ShapeType::ConvexMesh)
				{
					CreateBulletRigidBody(_convexHullShape.get());
				}
			}
			break;
		default:
			DENTY_ASSERT(false, "Unknown bullet object type! (in ApproximationCollisionMesh::CreateBulletPhysicsObjects)");
			break;
		}
	}

	void ApproximationCollisionMesh::CreateBulletRigidBody(btCollisionShape* collisionShape)
	{
		constexpr float mass = 0.0f;

		_motionState = std::make_shared<btDefaultMotionState>();

		btRigidBody::btRigidBodyConstructionInfo info = { mass, _motionState.get(), collisionShape };
		_rigidBody = std::make_shared<btRigidBody>(info);
	}
}
