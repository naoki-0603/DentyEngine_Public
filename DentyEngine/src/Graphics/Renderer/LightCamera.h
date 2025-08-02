#pragma once

namespace DentyEngine
{
	class ConstantBuffer;

	struct LightOrthographic
	{
	public:
		LightOrthographic();

	public:
		float nearZ;
		float farZ;
		float aspectRatio;

		float viewSize;
		Vector3 up;
	};

	struct LightCameraParameters
	{
	public:
		LightCameraParameters();

	public:
		LightOrthographic orthographic;

		Scope<ConstantBuffer> constantBuffer;

		std::array<float, 3u> cascadeAreaTable;

		Vector3 position;
		Vector3 targetPosition;
		Vector3 direction;

		Matrix viewProjection;
		Matrix orientation;
	};

	struct LightCameraConstants
	{
	public:
		LightCameraConstants();

	public:
		Vector4 position;

		Matrix viewProjection;
		/*std::array<Matrix, 3u> viewProjections;*/
	};

	// Does not inherited Camera class.
	class LightCamera final
	{
	public:
		LightCamera();
		~LightCamera() = default;

		void Create();

		void Update();

		void Bind();

		void OnGui();

		//
		// Getter
		//
		[[nodiscard]]
		const LightCameraParameters& GetParameters() const;

		[[nodiscard]]
		LightCameraParameters& GetParameters();
	private:
		void ComputeViewProjection();
		void OnUpdate();

	private:
		LightCameraParameters _parameters;
		LightCameraConstants _constants;

	private:
		static constexpr float INITIAL_VIEW_SIZE = 32.0f;
	};
}