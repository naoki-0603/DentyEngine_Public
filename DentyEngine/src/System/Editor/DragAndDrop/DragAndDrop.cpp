#include "stdafx.h"

#include "DragAndDrop.h"

#include "System/Resource/Script.h"

namespace DentyEngine
{
	namespace Utils
	{
		DragAndDropMeshData::DragAndDropMeshData() :
			_mesh(), _set()
		{
		}

		void DragAndDropMeshData::Set(Ref<Mesh> mesh)
		{
			_set = true;

			_mesh = mesh;
		}

		Ref<Mesh> DragAndDropMeshData::Get() const
		{
			if (!_set)
			{
				DENTY_ASSERT(false, "Mesh data not set! (in DragAndDropMeshData::Get)");
			}

			return std::static_pointer_cast<Mesh>(_mesh);
		}

		DragAndDropCollisionMeshData::DragAndDropCollisionMeshData() :
			_collisionMesh(), _set()
		{
		}

		void DragAndDropCollisionMeshData::Set(Ref<CollisionMesh> collisionMesh)
		{
			_collisionMesh = collisionMesh;

			_set = true;
		}

		Ref<CollisionMesh> DragAndDropCollisionMeshData::Get() const
		{
			if (!_set)
			{
				DENTY_ASSERT(false, "Collision mesh data not set! (in DragAndDropCollisionMeshData::Get)");
			}

			return std::static_pointer_cast<CollisionMesh>(_collisionMesh);
		}

		DragAndDropApproximationCollisionMeshData::DragAndDropApproximationCollisionMeshData() :
			_approximationCollisionMesh(), _set()
		{
		}

		void DragAndDropApproximationCollisionMeshData::Set(Ref<ApproximationCollisionMesh> approximationCollisionMesh)
		{
			_set = true;

			_approximationCollisionMesh = approximationCollisionMesh;
		}

		Ref<ApproximationCollisionMesh> DragAndDropApproximationCollisionMeshData::Get() const
		{
			if (!_set)
			{
				DENTY_ASSERT(false, "Approximation collision mesh data not set! (in DragAndDropApproximationCollisionMeshData::Get)");
			}

			return std::static_pointer_cast<ApproximationCollisionMesh>(_approximationCollisionMesh);
		}

		DragAndDropAnimationClipData::DragAndDropAnimationClipData() :
			_clip(), _set()
		{
		}

		void DragAndDropAnimationClipData::Set(Ref<MeshResource::AnimationClip> clip)
		{
			_set = true;

			_clip = clip;
		}

		Ref<MeshResource::AnimationClip> DragAndDropAnimationClipData::Get() const
		{
			if (!_set)
			{
				DENTY_ASSERT(false, "Animation clip data not set! (in DragAndDropAnimationClipData::Get)");
			}

			return std::static_pointer_cast<MeshResource::AnimationClip>(_clip);
		}

		DragAndDropAnimatorStateData::DragAndDropAnimatorStateData() :
			_state(), _set()
		{
		}

		void DragAndDropAnimatorStateData::Set(Ref<BaseState> state)
		{
			_set = true;

			_state = state;
		}

		Ref<BaseState> DragAndDropAnimatorStateData::Get() const
		{
			if (!_set)
			{
				DENTY_ASSERT(false, "Animator state data not set! (in DragAndDropAnimatorStateData::Get)");
			}

			return std::static_pointer_cast<BaseState>(_state);
		}

		DragAndDropSkyboxData::DragAndDropSkyboxData() :
			_skybox(), _set()
		{
		}

		void DragAndDropSkyboxData::Set(Ref<Skybox> skybox)
		{
			_skybox = skybox;

			_set = true;
		}

		Ref<Skybox> DragAndDropSkyboxData::Get() const
		{
			if (!_set)
			{
				DENTY_ASSERT(false, "Skybox data not set! (in DragAndDropSkyboxData::Get)");
			}

			return std::static_pointer_cast<Skybox>(_skybox);
		}

        DragAndDropScriptData::DragAndDropScriptData() :
		    _script(), _set()
        {
        }

		void DragAndDropScriptData::Set(Ref<Script> script)
        {
            _script = script;

			_set = true;
        }

		Ref<Script> DragAndDropScriptData::Get() const
        {
            if (not _set)
            {
                DENTY_ASSERT(false, "Script data not set! (in DragAndDropScriptData::Get");
            }

			return std::static_pointer_cast<Script>(_script);
        }


    }

	DragAndDrop::DragAndDrop(const DragAndDropSpecification& specification) :
		_specification(specification)
	{

	}
}
