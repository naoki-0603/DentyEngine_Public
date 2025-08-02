#include "stdafx.h"

#include "Camera.h"

#include "Graphics/Buffer/ConstantBuffer.h"

#include "System/Graphics/GraphicsManager.h"

#include "Utilities/MathUtilities.h"

namespace DentyEngine
{
	float Camera::Perspective::GetFovAngleRadian() const
	{
        return MathUtils::ToRadian(fovAngleY);
	}

	float Camera::Perspective::GetFovAngle() const
	{
        return fovAngleY;
	}

    Camera::Camera(ID3D11Device* device) :
        _projectionType(), _perspective(), _orthographic(),
        _view(), _projection(), _viewProjection(), _focusPosition(), _constantBuffer(), _constants()
    {
        _constantBuffer = std::make_shared<ConstantBuffer>(device, sizeof(Constants));
    }

    Camera::Camera(const Camera& source) :
		_projectionType(source._projectionType),
		_perspective(source._perspective), _orthographic(source._orthographic),
		_view(source._view), _projection(source._projection),
		_viewProjection(source._viewProjection), _focusPosition(source._focusPosition),
		_constants(source._constants)
    {
        ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

        _constantBuffer = std::make_shared<ConstantBuffer>(device, sizeof(Constants));
    }

    void Camera::CalculateView(Matrix& world, const Vector3& eyePosition, const Vector3& focusPosition, const Vector3& up)
    {
        _view = DirectX::XMMatrixLookAtLH(
            eyePosition.GetXM(),
            focusPosition.GetXM(),
            up.GetXM()
        );

        world = XMMatrixInverse(nullptr, _view.GetXM());
    }

    void Camera::CalculatePerspectiveProjection(const Perspective& perspective)
    {
        _projection = DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(perspective.fovAngleY),
            perspective.aspectRatio,
            perspective.nearZ, perspective.farZ
        );

        _perspective = perspective;
    }

    void Camera::CalculateOrthographicProjection(const Orthographic& orthographic)
    {
        _projection = DirectX::XMMatrixOrthographicLH(
            orthographic.viewSize.x,
            orthographic.viewSize.y,
            orthographic.nearZ, orthographic.farZ
        );

        _orthographic = orthographic;
    }

    void Camera::OnResize(const Vector2& size)
    {
        const float aspectRatio = size.x / size.y;

        switch (_projectionType)
        {
        case ProjectionType::Perspective:
            _perspective.aspectRatio = aspectRatio;

            CalculatePerspectiveProjection(_perspective);
            break;
        case ProjectionType::Orthographic:
            _orthographic.aspectRatio = aspectRatio;
            _orthographic.viewSize = size;

            CalculateOrthographicProjection(_orthographic);
            break;
        default:
            DENTY_ASSERT(false, "Unknown projection type.");
            break;
        }

        _viewProjection = _view * _projection;
    }
    
    void Camera::OnProjectionChanged(ProjectionType newType)
    {
        switch (newType)
        {
        case ProjectionType::Perspective:
            CalculatePerspectiveProjection(_perspective);
            break;
        case ProjectionType::Orthographic:
            CalculateOrthographicProjection(_orthographic);
            break;
        default:
            DENTY_ASSERT(false, "Unknown projection type.");
            break;
        }
    }

    Matrix Camera::CalculateViewMatrixTakingSphereIn(
        const Vector3& sphereCenter,
        float sphereRadius,
        float fovY,
        float aspectRatio,
        const Vector3& direction,
        const Vector3& up)
    {
        const float theta = (aspectRatio <= 1.0f ? fovY : fovY * aspectRatio);

        const float distance = sphereRadius / std::sinf(theta * 0.5f);

        const Vector3 position = sphereCenter - direction.Normalize() * distance;

        return DirectX::XMMatrixLookAtLH(position.GetXM(), sphereCenter.GetXM(), up.GetXM());
    }
};