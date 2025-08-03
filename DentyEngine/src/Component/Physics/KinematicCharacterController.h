#pragma once

#include "Component/ComponentGenerator.h"

#include "Component/PhysicsBehavior.h"

#include "System/Physics/PhysicsData.h"

class btKinematicCharacterController;

namespace DentyEngine
{
	class ConvexMeshShape;
	class CapsuleShape;
	class SphereShape;
	class BoxShape;
	class IShape;

	//
	// Unused class. (2024/01/09)
	//
	class KinematicCharacterController final : public PhysicsBehavior, public std::enable_shared_from_this<KinematicCharacterController>
	{
	public:
		KinematicCharacterController();
		KinematicCharacterController(const KinematicCharacterController& source);
		~KinematicCharacterController() override = default;

		void Awake() override;
		void Start() override;
		void OnUpdate(float deltaTime) override;
		void OnFixedUpdate() override;
		void OnGui() override;
		void OnPrefabGui() override;
		void OnEvent(Event* e) override;
		void OnDestroy() override;
		void Destroy() override;
		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnGameObjectDeserialized() override;

		void OnSceneActivated();
		void OnSceneDeActivated();

		void Restore();

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		//
		// Setter
		//
		void SetGravity(const Vector3& gravity);
		void SetWalkDirection(const Vector3& direction);
		void SetFallSpeed(float fallSpeed);

		//
		// Getter
		//
		[[nodiscard]]
		Ref<btPairCachingGhostObject> GetPairCachingGhostObject() const { return _pairCachingGhostObject; }

		[[nodiscard]]
		bool HasControllerAndGhostObject() const;

		DENTY_REGISTER_BUILD_IN_COMPONENT(KinematicCharacterController)
	private:
		void CreateShapes();
		void Create();

		void ShapeChanged();
		void OnShapeChanged();
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive) const
		{
			archive(
				cereal::base_class<PhysicsBehavior>(this)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive)
		{
			archive(
				cereal::base_class<PhysicsBehavior>(this)
			);

			OnDeserialized();
		}
	private:
		Ref<btKinematicCharacterController> _controller;
		Ref<btPairCachingGhostObject> _pairCachingGhostObject;

		// Shapes
		Ref<IShape> _currentShape;
		Ref<BoxShape> _boxShape;
		Ref<SphereShape> _sphereShape;
		Ref<CapsuleShape> _capsuleShape;
		Ref<ConvexMeshShape> _convexMeshShape;

		ShapeType _currentShapeType;
		ShapeType _prevShapeType;
	};
}

CEREAL_REGISTER_TYPE(DentyEngine::KinematicCharacterController)