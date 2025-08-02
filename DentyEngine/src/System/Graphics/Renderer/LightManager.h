#pragma once

#include "System/Core/IManager.h"

#include "Component/Graphics/Renderer/DirectionalLight.h"
#include "Component/Graphics/Renderer/HemisphereLight.h"
#include "Component/Graphics/Renderer/PointLight.h"
#include "Component/Graphics/Renderer/SpotLight.h"

namespace DentyEngine
{
	class EventLightCreated;
	class EventLightDestroyed;

	class EventSceneRendering;

	class EventEditorSceneManagerDeserialized;

	class EventOnSceneDeActivated;

	class ConstantBuffer;

	class LightManager final : public IManager
	{
	public:
		struct Constants
		{
			DirectionalLight::Constants directionalLightConstants;
			HemisphereLightConstants hemisphereLightConstants;
			std::array<PointLight::Constants, 10> pointLightConstants;
			std::array<SpotLight::Constants, 10> spotLightConstants;

			int pointLightCount;
			int spotLightCount;

			Vector2 padding;
		};
	public:
		LightManager();
		~LightManager() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(LightManager)

		void Initialize();
		void Finalize();
		void Update();
		void OnEvent(Event* e);
		void OnGui() override;

		void AddLight(Ref<Light> light);
		void RemoveLight(const Ref<Light>& light);

        void Reset();

		//
		// Getter
		//
		[[nodiscard]]
		Ref<DirectionalLight> GetDirectionalLight() const { return _directionalLight; }

		[[nodiscard]]
		bool DirectionalLightExists() const { return (_directionalLight != nullptr); }

		[[nodiscard]]
		bool HemisphereLightExists() const { return (_hemisphereLight != nullptr); }

		[[nodiscard]]
		bool PointLightExists() const { return (!_pointLights.empty()); }

		[[nodiscard]]
		bool SpotLightExists() const { return (!_spotLights.empty()); }

		//
		// Static function.
		//
	public:
		static LightManager& GetInstance()
		{
		    static LightManager instance;
			return instance;
		}

	private:
		void UpdateConstants();

		void OnSceneRendering(EventSceneRendering* e);
		void OnSceneDeActivated(EventOnSceneDeActivated* e);
	private:
		Ref<DirectionalLight> _directionalLight;
		Ref<HemisphereLight> _hemisphereLight;

	    std::vector<Ref<PointLight>> _pointLights;
		std::vector<Ref<SpotLight>> _spotLights;

		Ref<ConstantBuffer> _constantBuffer;
		Constants _constants;
	};
}
