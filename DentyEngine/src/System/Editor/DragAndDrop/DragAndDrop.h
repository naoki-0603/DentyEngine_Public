#pragma once

#include "System/Core/DString.h"

namespace DentyEngine
{
    class ApproximationCollisionMesh;
	class Mesh;
	class CollisionMesh;

	namespace MeshResource
	{
		class AnimationClip;
	}

	class BaseState;

	class Skybox;

    class Script;

	namespace Utils
	{
		struct DragAndDropMeshData
		{
			DragAndDropMeshData();

			void Set(Ref<Mesh> mesh);

			[[nodiscard]]
			Ref<Mesh> Get() const;
		private:
			Ref<void> _mesh;

			bool _set;
		};

		struct DragAndDropCollisionMeshData
		{
			DragAndDropCollisionMeshData();

			void Set(Ref<CollisionMesh> collisionMesh);

			[[nodiscard]]
			Ref<CollisionMesh> Get() const;
		private:
			Ref<void> _collisionMesh;

			bool _set;
		};

		struct DragAndDropApproximationCollisionMeshData
		{
			DragAndDropApproximationCollisionMeshData();

			void Set(Ref<ApproximationCollisionMesh> approximationCollisionMesh);

			[[nodiscard]]
			Ref<ApproximationCollisionMesh> Get() const;
		private:
			Ref<void> _approximationCollisionMesh;

			bool _set;
		};

		struct DragAndDropAnimationClipData
		{
			DragAndDropAnimationClipData();

			void Set(Ref<MeshResource::AnimationClip> clip);

			[[nodiscard]]
			Ref<MeshResource::AnimationClip> Get() const;
		private:
			Ref<void> _clip;

			bool _set;
		};

		struct DragAndDropAnimatorStateData
		{
			DragAndDropAnimatorStateData();

			void Set(Ref<BaseState> state);

			[[nodiscard]]
			Ref<BaseState> Get() const;

			[[nodiscard]]
			bool Exists() const { return (_state != nullptr); }
		private:
			Ref<void> _state;

			bool _set;
		};

		struct DragAndDropSkyboxData
		{
			DragAndDropSkyboxData();

			void Set(Ref<Skybox> skybox);

			[[nodiscard]]
			Ref<Skybox> Get() const;

			[[nodiscard]]
			bool Exists() const { return (_skybox != nullptr); }
		private:
			Ref<void> _skybox;

			bool _set;
		};

		struct DragAndDropScriptData
		{
		    DragAndDropScriptData();

			void Set(Ref<Script> script);

			[[nodiscard]]
			Ref<Script> Get() const;

			[[nodiscard]]
			bool Exists() const { return (_script != nullptr); }
		private:
			Ref<void> _script;

			bool _set;
		};
	}

	enum class DragAndDropDataType : int32_t
	{
		None = -1,
		Scene = 0,
		Mesh,
		CollisionMesh,
		ApproximationCollisionMesh,
		Prefab,
		Texture,
		Material,
		AnimationClip,
		AnimatorController,
		AnimatorState,
		GameObject,
		Skybox,
		Script,

		Max
	};

	struct DragAndDropSpecification
	{
		DragAndDropSpecification(
			const char* type,
			size_t dataSize,
			void* data,
			DragAndDropDataType dragAndDropDataType,
			const std::function<void(void*, DragAndDropDataType, const Ref<void>&)>& callback,
			const std::function<void(void*, DragAndDropDataType, const Ref<void>&)>& callbackInDestructor,
			ImGuiDragDropFlags flags = 0, const Ref<void>& userData = nullptr
		) : type(type), dataSize(dataSize), data(data), dragAndDropDataType(dragAndDropDataType),
			callback(callback), callbackInDestructor(callbackInDestructor),
	        flags(flags), userData(userData) {}

		const char* type;
		size_t dataSize;

		void* data;
		DragAndDropDataType dragAndDropDataType;

		std::function<void(void*, DragAndDropDataType, const Ref<void>&)> callback;
		std::function<void(void*, DragAndDropDataType, const Ref<void>&)> callbackInDestructor;

		ImGuiDragDropFlags flags;

		// If you want to use variables in callback function, use this.
		Ref<void> userData;
	};

	class DragAndDrop
	{
	public:
		DragAndDrop(const DragAndDropSpecification&);
		virtual ~DragAndDrop() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(DragAndDrop)

		[[nodiscard]]
		const DragAndDropSpecification& GetSpecification() const { return _specification; }
	private:
		DragAndDropSpecification _specification;
	};

	// Nothing to do.
#define DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION() [](void*, DragAndDropDataType, const Ref<void>&) -> void { return; }
#define DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(function) [this](void* data, DragAndDropDataType dragAndDropDataType, const Ref<void>& userData) -> void { this->function(data, dragAndDropDataType, userData); }
}
