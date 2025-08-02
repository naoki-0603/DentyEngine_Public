#pragma once

#include "Collider.h"

#include "Component/ComponentGenerator.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

namespace DentyEngine
{
	class ApproximationCollisionMesh;

	// This class doesn't use Collider::_ghostObject
	// Instead use ApproximationCollisionMesh::_ghostObject.
	// And this component can attach to parent object only.
	class ApproximationMeshCollider final : public Collider
	{
	public:
		ApproximationMeshCollider();
		ApproximationMeshCollider(const ApproximationMeshCollider& source);
		~ApproximationMeshCollider() override = default;

		void Awake() override;
		void Start() override;

		void OnUpdate(float deltaTime) override;
		void OnFixedUpdate() override;

		void OnGui() override;
		void OnPrefabGui() override;

		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnGameObjectDeserialized() override;

		void UpdateState(const Vector3& position, const Quaternion& orientation) override;

		void ApplyScaling() override;

		// Call on PhysicsEngine::AddCollisionObject function
		void AddCollisionObjects(btDiscreteDynamicsWorld* world);

		// Call on PhysicsEngine::RemoveCollisionObject function
		void RemoveCollisionObjects(btDiscreteDynamicsWorld* world);

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		void OnSceneActivated() override;
		void OnSceneDeActivated() override;

		void OnGameObjectTagChanged() override;

		//
		// Getter
		//
		[[nodiscard]]
		Ref<IShape> GetCollisionShape() const override { return nullptr; }

		[[nodiscard]]
		bool HasApproximationMesh() const { return (_approximationCollisionMesh != nullptr); }

		DENTY_REGISTER_BUILD_IN_COMPONENT(ApproximationMeshCollider)
	private:
		void SetChildrenUserIndex();

		void ApplyTriggerCollisionStateToChildren();
		void RemoveTriggerCollisionStateFromChildren();

		void UpdateBulletWorldTransform();

		//
		// Drag and drop target callback functions.
		//
		void ApproximationCollisionMeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Collider>(this),
					CEREAL_NVP(_approximationCollisionMeshFilePath),
					CEREAL_NVP(_size)
				);    
			}

			if (version == 2u)
			{
			    archive(
					cereal::base_class<Collider>(this),
					CEREAL_NVP(_approximationCollisionMeshFilePath),
					CEREAL_NVP(_offset),
					CEREAL_NVP(_rotation),
					CEREAL_NVP(_size)
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
					cereal::base_class<Collider>(this),
					CEREAL_NVP(_approximationCollisionMeshFilePath),
					CEREAL_NVP(_size)
				);    
			}

			if (version == 2u)
			{
			    archive(
					cereal::base_class<Collider>(this),
					CEREAL_NVP(_approximationCollisionMeshFilePath),
					CEREAL_NVP(_offset),
					CEREAL_NVP(_rotation),
					CEREAL_NVP(_size)
				); 
			}

			OnDeserialized();
		}
	private:
		// Doesn't serialize.
		Ref<ApproximationCollisionMesh> _approximationCollisionMesh;

		// Instead of serialize _approximationCollisionMesh.
		FilePath _approximationCollisionMeshFilePath;

		Vector3 _offset;
		Vector3 _rotation;
		Vector3 _size;
	};
}

//CEREAL_CLASS_VERSION(DentyEngine::ApproximationMeshCollider, 1u)
CEREAL_CLASS_VERSION(DentyEngine::ApproximationMeshCollider, 2u)

CEREAL_REGISTER_TYPE(DentyEngine::ApproximationMeshCollider)