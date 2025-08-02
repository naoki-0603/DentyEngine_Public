#pragma once

namespace DentyEngine
{
	class LightCamera;
	class SceneBuffer;
	class GaussianBlur;

	struct VarianceShadowMapConstants
	{
	public:
		VarianceShadowMapConstants();
		~VarianceShadowMapConstants() = default;

	public:
		static constexpr float SHADOW_MAP_RESOLUTION = 4096.0f;
		static constexpr float PLAYER_SHADOW_MAP_RESOLUTION = 1024.0f;
	};

	struct VarianceShadowMapParameters
	{
	public:
		VarianceShadowMapParameters();

	public:
		Scope<LightCamera> lightCamera;
		Ref<SceneBuffer> shadowMap;
		Ref<GaussianBlur> gaussianBlur;

		Scope<LightCamera> playerLightCamera;
		Ref<SceneBuffer> playerShadowMap;
		Ref<GaussianBlur> playerShadowGaussianBlur;
	};

	class VarianceShadowMap final
	{
	public:
		VarianceShadowMap();
		~VarianceShadowMap() = default;

		static void Setup();

		static void Begin(ID3D11DeviceContext* context);
		static void End(ID3D11DeviceContext* context);

		static void BeginPlayerShadow(ID3D11DeviceContext* context);
		static void EndPlayerShadow(ID3D11DeviceContext* context);

		static void OnUpdate();
		static void OnGui();

		static void OnEndPreRendering(ID3D11DeviceContext* context);

		//
		// Getter
		//
		[[nodiscard]]
		static VarianceShadowMapParameters& GetParameters();
	private:
		static VarianceShadowMapParameters _parameters;
	};
}