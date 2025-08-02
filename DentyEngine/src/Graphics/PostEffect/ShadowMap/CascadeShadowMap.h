#pragma once

namespace DentyEngine
{
	class LightCamera;
	class FrameBuffer;
	class ShadowCamera;

	class Camera;

	struct CascadeShadowMapConstants
	{
	public:
		CascadeShadowMapConstants();
		~CascadeShadowMapConstants() = default;

	public:
		static constexpr uint8_t NUM_SHADOW_MAP = 3u;
		static constexpr std::array<uint8_t, NUM_SHADOW_MAP> SHADOW_MAP_BIND_SLOTS =
		{
			20u, 21u, 22u
		};

		static constexpr std::array<float, NUM_SHADOW_MAP> SHADOW_MAP_RESOLUTIONS =
		{
			4096.0f, 2048.0f, 1024.0f
		};
	};

	struct CascadeShadowMapParameters
	{
	public:
		CascadeShadowMapParameters();
		~CascadeShadowMapParameters() = default;

	public:
		Scope<LightCamera> lightCamera;

		std::array<Scope<FrameBuffer>, CascadeShadowMapConstants::NUM_SHADOW_MAP> shadowMaps;
		std::array<float, CascadeShadowMapConstants::NUM_SHADOW_MAP> cascadeAreaTable;
		std::array<Matrix, CascadeShadowMapConstants::NUM_SHADOW_MAP> lightViewProjectionClopMatrices;
	};

	class CascadeShadowMap final
	{
	public:
		CascadeShadowMap();
		~CascadeShadowMap();

		static void Setup();

		static void Begin(ID3D11DeviceContext* context, uint32_t areaNo);
		static void End(ID3D11DeviceContext* context, uint32_t areaNo);

		static void OnUpdate();
		static void OnGui();

		static void CalculateLightViewProjections(const Camera* camera);

		static void OnEndPreRendering(ID3D11DeviceContext* context);

		//
		// Getter
		//
		[[nodiscard]]
		static CascadeShadowMapParameters& GetParameters();

	private:
		static CascadeShadowMapParameters _parameters;
	};
}