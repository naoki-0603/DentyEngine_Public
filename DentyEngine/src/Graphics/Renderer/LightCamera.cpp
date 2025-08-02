#include "stdafx.h"

#include "LightCamera.h"

#include "Graphics/Buffer/ConstantBuffer.h"

#include "Scene/GameObject.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Graphics/Renderer/LightManager.h"

namespace DentyEngine
{
	LightOrthographic::LightOrthographic() :
		nearZ(), farZ(), aspectRatio(), viewSize(), up(Vector3::Up())
	{
	}

	LightCameraParameters::LightCameraParameters() :
		orthographic(), constantBuffer(), cascadeAreaTable(), position(), targetPosition(), direction(), viewProjection(), orientation()
	{
	}

	LightCameraConstants::LightCameraConstants() :
		position(), viewProjection()
	{

	}

	LightCamera::LightCamera() :
		_parameters()
	{
	}

	void LightCamera::Create()
	{
		// Create constant buffer.
		{
			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			_parameters.constantBuffer = std::make_unique<ConstantBuffer>(device, sizeof(LightCameraConstants));
		}

		// Create orthographic
		{
			_parameters.orthographic.viewSize = INITIAL_VIEW_SIZE;
			_parameters.orthographic.aspectRatio = 1.0f;
			_parameters.orthographic.nearZ = 0.1f;
			_parameters.orthographic.farZ = 32.0f;
			_parameters.orthographic.up = Vector3::Up();
		}

		// Set Parameters.
		{
			_parameters.targetPosition = Vector3::Zero();
		}
	}

	void LightCamera::Update()
	{
		ComputeViewProjection();

		OnUpdate();
	}

	void LightCamera::Bind()
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		_parameters.constantBuffer->UpdateSubresource(context, &_constants);
		_parameters.constantBuffer->BindAll(context, 7u);
	}

	void LightCamera::OnGui()
	{
		ImGui::SliderFloat("ViewSize##LightCamera", &_parameters.orthographic.viewSize, 1.0f, 1024.0f);
		ImGui::SliderFloat("Near##LightCamera", &_parameters.orthographic.nearZ, 0.1f, 100.0f);
		ImGui::SliderFloat("Far##LightCamera", &_parameters.orthographic.farZ, 1.0f, 5000.0f);
		ImGui::DragFloat3("TargetPosition##LightCamera", &_parameters.targetPosition.x);
	}

	const LightCameraParameters& LightCamera::GetParameters() const
	{
		return _parameters;
	}

	LightCameraParameters& LightCamera::GetParameters()
	{
		return _parameters;
	}

	void LightCamera::ComputeViewProjection()
	{
		if (not LightManager::GetInstance().DirectionalLightExists())
		{
			return;
		}

		const Ref<DirectionalLight> directionalLight = LightManager::GetInstance().GetDirectionalLight();
		const Transform& transform = directionalLight->GetParent()->transform;

		_parameters.position = transform.position;
		if (_parameters.position.IsAlmostZero())
		{
			_parameters.position = Vector3(0.0f, 1.0f, 0.0f);
		}

		_parameters.direction = directionalLight->GetDirection().GetAsVector3();

		const Matrix view = DirectX::XMMatrixLookAtLH(
			_parameters.position.GetXM(),
			_parameters.targetPosition.GetXM(),
			_parameters.orthographic.up.GetXM()
		);

		const Matrix projection = DirectX::XMMatrixOrthographicLH(
			_parameters.orthographic.viewSize,
			_parameters.orthographic.viewSize,
			_parameters.orthographic.nearZ,
			_parameters.orthographic.farZ
		);

		_parameters.viewProjection = view * projection;
		_parameters.orientation = view.Inverse();
	}

	void LightCamera::OnUpdate()
	{
		_constants.position = Vector4(_parameters.position, 1.0f);
		_constants.viewProjection = _parameters.viewProjection;
	}
}
