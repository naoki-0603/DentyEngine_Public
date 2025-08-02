#pragma once

#include "Component/Behaviour.h"
#include "Component/ComponentGenerator.h"

#include "Graphics/Renderer/Camera.h"

namespace DentyEngine
{
	class Sprite3D;
	class EventGameBeginRenderingProcess;
	class EventGameEndRenderingProcess;
	class EventOnGameViewSizeChanged;

	class EventOnSceneActivated;
	class EventOnSceneDeActivated;

	class GameCamera final : public Camera, public Behaviour, public std::enable_shared_from_this<GameCamera>
	{
	public:
		GameCamera();
		GameCamera(const GameCamera&);
		~GameCamera() override = default;

		void Awake() override;
		void Start() override;
		void OnUpdate(const float deltaTime) override;
		void OnFixedUpdate() override;
		void OnEvent(Event* e) override;
		void OnGui() override;

		void OnGizmoRender() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void OnDestroy() override;

		void OnGameObjectDeserialized() override;


		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		//
		// Event functions.
		//
		void OnGameBeginRenderingProcess(EventGameBeginRenderingProcess* e);
		void OnGameEndRenderingProcess(EventGameEndRenderingProcess* e);
		void OnViewSizeChanged(EventOnGameViewSizeChanged* e);

		//
		// Setter
		//

		//
		// Callable from c# side.
		//
		void SetFocusPosition(const Vector3& focusPosition) { _focusPosition = focusPosition; }
		void SetViewProjection(const Matrix& viewProjection) { _viewProjection = viewProjection; }
		void SetView(const Matrix& view) { _view = view; }
		void SetProjection(const Matrix& projection) { _projection = projection; }

		DENTY_REGISTER_BUILD_IN_COMPONENT(GameCamera)
	public:
		static void SetMainCamera(Ref<GameCamera> camera);
	    static void ResetMainCamera();

		static void UpdateMainCameraConstants(const Constants& constants);

		[[nodiscard]]
		static Ref<GameCamera> GetMainCamera() { return _mainCamera; }
	private:
		void CreateGizmoSprite();

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] uint32_t version) const
		{
			archive(
				cereal::base_class<Camera>(this),
				cereal::base_class<Component>(this)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] uint32_t version)
		{
			archive(
				cereal::base_class<Camera>(this),
				cereal::base_class<Component>(this)
			);

			OnDeserialized();
		}
	private:
		Ref<Sprite3D> _gizmoSprite;

		ProjectionType _oldProjectionType;

	private:

		//
		// Static variables.
		//
		static Ref<GameCamera> _mainCamera;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::GameCamera, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::GameCamera)