#include "stdafx.h"

#include "ShadowCamera.h"

#include "Component/Graphics/Renderer/DirectionalLight.h"

#include "Graphics/Buffer/ConstantBuffer.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
    ShadowCamera::ShadowCamera() :
        Camera(), _constants(), _directionalLight(), _direction(Vector3::Down()),
        _viewSize(32.0f), _distance(100.0f), _near(0.1f), _far(10000.0f), _bias(0.00001f)
    {
    }

    void ShadowCamera::Create()
    {
        ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

        _constantBuffer = std::make_shared<ConstantBuffer>(device, sizeof(ShadowCameraConstants));
    }

    void ShadowCamera::OnUpdate()
    {
        if (not HasDirectionalLight())
            return;

        // Create orthographic camera.
        {
            Orthographic orthographic = { };
            orthographic.viewSize = Vector2(_viewSize, _viewSize);
            orthographic.nearZ = _near;
            orthographic.farZ = _far;
            orthographic.aspectRatio = orthographic.viewSize.x / orthographic.viewSize.y;
            orthographic.up = { 0, 1, 0 };
            orthographic.distance = _distance;

            CalculateOrthographicProjection(orthographic);

            // Calculate eye position.
            {
                const Vector3 lightDirection = _directionalLight->_direction.GetAsVector3().Normalize();

                _eyePosition = (lightDirection * _distance) - _focusPosition;
            }

            // Calculate view.
            Matrix world = { };
            CalculateView(world, _eyePosition, _focusPosition, orthographic.up);

            // Calculate view projection.
            _viewProjection = _view * _projection;
        }
    }

    void ShadowCamera::UpdateConstants(const std::array<Matrix, 3>& lightViewProjections)
    {
        _constants.view = _view;
        _constants.projection = _projection;
        _constants.viewProjections = lightViewProjections;
        _constants.cameraPosition = _eyePosition;
        _constants.bias = _bias;
    }

    void ShadowCamera::OnEvent([[maybe_unused]] Event* e)
    {
       
    }

    void ShadowCamera::OnGui()
    {
        ImGui::DragFloat("Bias##ShadowCamera", &_bias);
        ImGui::DragFloat("Near##ShadowCamera", &_near);
        ImGui::DragFloat("Far##ShadowCamera", &_far);

        ImGui::DragFloat("ViewSize##ShadowCamera", &_viewSize);

        ImGui::DragFloat("Distance##ShadowCamera", &_distance);
        ImGui::DragFloat3("Direction##ShadowCamera", &_directionalLight->_direction.x, -1.0f, 1.0f);

        ImGui::Text("EyePosition: %f, %f, %f", _eyePosition.x, _eyePosition.y, _eyePosition.z);
    }

    void ShadowCamera::Bind(ID3D11DeviceContext* context)
    {
        _constantBuffer->UpdateSubresource(context, &_constants);
        _constantBuffer->BindAll(context, static_cast<UINT>(ConstantBuffer::ReservedBindSlot::Shadow));
    }

    void ShadowCamera::SetDirectionalLight(Ref<DirectionalLight> directionalLight)
    {
        _directionalLight = directionalLight;
    }
}
